/* This file is derived from source code for the Nachos
   instructional operating system.  The Nachos copyright notice
   is reproduced in full below. */

/* Copyright (c) 1992-1996 The Regents of the University of California.
   All rights reserved.

   Permission to use, copy, modify, and distribute this software
   and its documentation for any purpose, without fee, and
   without written agreement is hereby granted, provided that the
   above copyright notice and the following two paragraphs appear
   in all copies of this software.

   IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
   ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
   CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
   AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
   HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
   PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
   BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
   MODIFICATIONS.
*/

#include "threads/synch.h"
#include <stdio.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

/* Initializes semaphore SEMA to VALUE.  A semaphore is a
   nonnegative integer along with two atomic operators for
   manipulating it:

   - down or "P": wait for the value to become positive, then
     decrement it.

   - up or "V": increment the value (and wake up one waiting
     thread, if any). */
void
sema_init (struct semaphore *sema, unsigned value) 
{
  ASSERT (sema != NULL);

  sema->value = value;
  list_init (&sema->waiters);
}

/* Down or "P" operation on a semaphore.  Waits for SEMA's value
   to become positive and then atomically decrements it.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but if it sleeps then the next scheduled
   thread will probably turn interrupts back on. */
void
sema_down (struct semaphore *sema) 
{
  enum intr_level old_level;

  ASSERT (sema != NULL);
  ASSERT (!intr_context ());

  old_level = intr_disable ();
  while (sema->value == 0) 
    {
        //chanege the list_push_back to list_insert_ordered in sema_down()
      list_insert_ordered (&sema->waiters, &thread_current ()->elem, priority_less, NULL);
      thread_block ();
    }
  sema->value--;
  intr_set_level (old_level);
}

/* Down or "P" operation on a semaphore, but only if the
   semaphore is not already 0.  Returns true if the semaphore is
   decremented, false otherwise.

   This function may be called from an interrupt handler. */
bool
sema_try_down (struct semaphore *sema) 
{
  enum intr_level old_level;
  bool success;

  ASSERT (sema != NULL);

  old_level = intr_disable ();
  if (sema->value > 0) 
    {
      sema->value--;
      success = true; 
    }
  else
    success = false;
  intr_set_level (old_level);

  return success;
}

/* Up or "V" operation on a semaphore.  Increments SEMA's value
   and wakes up one thread of those waiting for SEMA, if any.

   This function may be called from an interrupt handler. */
void
sema_up (struct semaphore *sema)
{
    enum intr_level old_level;

    ASSERT (sema != NULL);

    old_level = intr_disable ();
    if (!list_empty (&sema->waiters)){
        //sort the list and make sure we can unblock the top one of the list
        list_sort (&sema->waiters, priority_less, NULL);
        thread_unblock(list_entry(list_pop_front(&sema->waiters), struct thread, elem));
    }
    sema->value++;
    thread_yield();     //once a thread is unblocked, we put it into the ready queue and let the CPU reschedule
    intr_set_level (old_level);
}

static void sema_test_helper (void *sema_);

/* Self-test for semaphores that makes control "ping-pong"
   between a pair of threads.  Insert calls to printf() to see
   what's going on. */
void
sema_self_test (void) 
{
  struct semaphore sema[2];
  int i;

  printf ("Testing semaphores...");
  sema_init (&sema[0], 0);
  sema_init (&sema[1], 0);
  thread_create ("sema-test", PRI_DEFAULT, sema_test_helper, &sema);
  for (i = 0; i < 10; i++) 
    {
      sema_up (&sema[0]);
      sema_down (&sema[1]);
    }
  printf ("done.\n");
}

/* Thread function used by sema_self_test(). */
static void
sema_test_helper (void *sema_) 
{
  struct semaphore *sema = sema_;
  int i;

  for (i = 0; i < 10; i++) 
    {
      sema_down (&sema[0]);
      sema_up (&sema[1]);
    }
}

/* Initializes LOCK.  A lock can be held by at most a single
   thread at any given time.  Our locks are not "recursive", that
   is, it is an error for the thread currently holding a lock to
   try to acquire that lock.

   A lock is a specialization of a semaphore with an initial
   value of 1.  The difference between a lock and such a
   semaphore is twofold.  First, a semaphore can have a value
   greater than 1, but a lock can only be owned by a single
   thread at a time.  Second, a semaphore does not have an owner,
   meaning that one thread can "down" the semaphore and then
   another one "up" it, but with a lock the same thread must both
   acquire and release it.  When these restrictions prove
   onerous, it's a good sign that a semaphore should be used,
   instead of a lock. */
void
lock_init (struct lock *lock)
{
  ASSERT (lock != NULL);

  lock->holder = NULL;
  sema_init (&lock->semaphore, 1);
}

/* Acquires LOCK, sleeping until it becomes available if
   necessary.  The lock must not already be held by the current
   thread.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but interrupts will be turned back on if
   we need to sleep. */
void
lock_acquire (struct lock *lock)
{
    ASSERT (lock != NULL);
    ASSERT (!intr_context ());
    ASSERT (!lock_held_by_current_thread (lock));

    if(lock->holder != NULL && !thread_mlfqs) {
        //because the lock already has a holder, so the lock has to wait until the holder release the lock
        thread_current()->wait_which_lock = lock;
        //give the lock value to lock_ref as a temp variable so it can use a while loop to do recursion
        struct lock *lock_ref = lock;
        //if the lock_ref exits and it has a holder, do recursion
        while(lock_ref !=  NULL && lock_ref->holder != NULL){
            //and the current_thread's priority should be larger than the lock_ref's priority, otherwise there is no donation occurring
            if(thread_current()->priority > lock_ref->priority) {
                //set the priority value of lock_ref to the priority of thread_current() if current thread's priority is bigger than lock_ref's priority
                lock_ref->priority = thread_current()->priority;
                //next part is for checking if our list already has that lock or not
                bool flag = false;
                if (!list_empty(&(lock_ref->holder->lock_list))) {
                    struct list_elem *e;
                    for (e = list_begin(&(lock_ref->holder->lock_list)); e != list_end(&(lock_ref->holder->lock_list)); e = list_next(e)) {
                        struct lock *lock_elem_from_list = list_entry(e, struct lock, elem);
                        if (lock_elem_from_list == lock_ref) {
                            flag = true;        //if the list has the lock, return the flag's value as true, otherwise, flase
                            break;
                        }
                    }
                }
                //if the list has the lock, we update the list order, and let the priority of the top one of the list to be lock_ref's priority
                if (flag) {

                    list_sort(&(lock_ref->holder->lock_list), lock_priority_less, NULL);
                    lock_ref->holder->priority = list_entry(list_front(&(lock_ref->holder->lock_list)), struct lock, elem)->priority;
                    //if the list doesn't have the lock, we insert the lock, and let the priority of the top one of the list to be lock_ref's priority
                } else {

                    list_insert_ordered(&(lock_ref->holder->lock_list), &lock_ref->elem, lock_priority_less, NULL); //in thread_yeild()
                    lock_ref->holder->priority = lock_ref->priority;

                }
                //Here we let the lock_ref be the lock which the holder is waitting, do the recursion
                // so that we can donate the priority to the lock which the holder is waititng
                lock_ref = lock_ref->holder->wait_which_lock;

            }
        }
    }

    sema_down (&lock->semaphore);

    //the current_thread does not need any lock now
    lock->holder = thread_current();
    lock->priority = thread_current()->priority; //restore the lock's priority back to current_thread's priority
}

/* Tries to acquires LOCK and returns true if successful or false
   on failure.  The lock must not already be held by the current
   thread.

   This function will not sleep, so it may be called within an
   interrupt handler. */
bool
lock_try_acquire (struct lock *lock)
{
  bool success;

  ASSERT (lock != NULL);
  ASSERT (!lock_held_by_current_thread (lock));

  success = sema_try_down (&lock->semaphore);
  if (success)
    lock->holder = thread_current ();
  return success;
}

/* Releases LOCK, which must be owned by the current thread.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to release a lock within an interrupt
   handler. */
//void lock_release();
void
lock_release (struct lock *lock)
{
    ASSERT (lock != NULL);
    ASSERT (lock_held_by_current_thread (lock));
    if(!thread_mlfqs) {
        //first check if the lock_list of the holder is empty or not
        if (!list_empty(&(lock->holder->lock_list))) {      //if it is not empty
            //release the lock form the lock_list if the lock is in the lock_list
            struct list_elem *e;
            for (e = list_begin(&(lock->holder->lock_list));
                 e != list_end(&(lock->holder->lock_list)); e = list_next(e)) {
                struct lock *lock1 = list_entry(e,
                struct lock, elem);
                if (lock1 == lock) {
                    list_remove(e);
                    break;
                }
            }
            //after removing the lock from the list, check again see if the lock_list now is empty or not
            if (!list_empty(&(lock->holder->lock_list))) {   //if it is not empty
                //update the order of the list and and let the priority of the top one of the list to be lock_ref's priority.
                list_sort(&(lock->holder->lock_list), lock_priority_less, NULL);
                lock->holder->priority = list_entry(list_front(&(lock->holder->lock_list)),
                struct lock, elem)->priority;
            } else {
                //if it is empty, let the original_priroity of lock_ref to be its priority.
                lock->holder->priority = lock->holder->original_priority;
            }
        } else {
            //if it is empty, let the original_priroity of lock_ref to be its priority.
            lock->holder->priority = lock->holder->original_priority;
        }
    }
    lock->holder = NULL;
    sema_up(&lock->semaphore);
}


/* Returns true if the current thread holds LOCK, false
   otherwise.  (Note that testing whether some other thread holds
   a lock would be racy.) */
bool
lock_held_by_current_thread (const struct lock *lock) 
{
  ASSERT (lock != NULL);

  return lock->holder == thread_current ();
}

/* One semaphore in a list. */
struct semaphore_elem 
  {
    struct list_elem elem;              /* List element. */
    struct semaphore semaphore;         /* This semaphore. */
  };

/* Initializes condition variable COND.  A condition variable
   allows one piece of code to signal a condition and cooperating
   code to receive the signal and act upon it. */
void
cond_init (struct condition *cond)
{
  ASSERT (cond != NULL);

  list_init (&cond->waiters);
}

/* Atomically releases LOCK and waits for COND to be signaled by
   some other piece of code.  After COND is signaled, LOCK is
   reacquired before returning.  LOCK must be held before calling
   this function.

   The monitor implemented by this function is "Mesa" style, not
   "Hoare" style, that is, sending and receiving a signal are not
   an atomic operation.  Thus, typically the caller must recheck
   the condition after the wait completes and, if necessary, wait
   again.

   A given condition variable is associated with only a single
   lock, but one lock may be associated with any number of
   condition variables.  That is, there is a one-to-many mapping
   from locks to condition variables.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but interrupts will be turned back on if
   we need to sleep. */
void
cond_wait (struct condition *cond, struct lock *lock) 
{
  struct semaphore_elem waiter;

  ASSERT (cond != NULL);
  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (lock_held_by_current_thread (lock));
  
  sema_init (&waiter.semaphore, 0);
  list_push_back (&cond->waiters, &waiter.elem);
  lock_release (lock);
  sema_down (&waiter.semaphore);
  lock_acquire (lock);
}

/* If any threads are waiting on COND (protected by LOCK), then
   this function signals one of them to wake up from its wait.
   LOCK must be held before calling this function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to signal a condition variable within an
   interrupt handler. */
void
cond_signal (struct condition *cond, struct lock *lock UNUSED)
{
    ASSERT (cond != NULL);
    ASSERT (lock != NULL);
    ASSERT (!intr_context ());
    ASSERT (lock_held_by_current_thread (lock));

    if (!list_empty (&cond->waiters)) {
        list_sort(&cond->waiters, cond_sema_priority_less, NULL);
        sema_up(&list_entry(list_pop_front(&cond->waiters), struct semaphore_elem, elem)->semaphore);
    }
}

/* Wakes up all threads, if any, waiting on COND (protected by
   LOCK).  LOCK must be held before calling this function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to signal a condition variable within an
   interrupt handler. */
void
cond_broadcast (struct condition *cond, struct lock *lock) 
{
  ASSERT (cond != NULL);
  ASSERT (lock != NULL);

  while (!list_empty (&cond->waiters))
    cond_signal (cond, lock);
}

//#synch.c
bool
cond_sema_priority_less (const struct list_elem *sema1, const struct list_elem *sema2, void *aux UNUSED)
{
    return list_entry(list_front(&(list_entry (sema1, struct semaphore_elem, elem)->semaphore.waiters)), struct thread, elem)->priority > list_entry(list_front(&(list_entry (sema2, struct semaphore_elem, elem)->semaphore.waiters)), struct thread, elem)->priority;
}
