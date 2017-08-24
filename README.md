# CSE-521-Operating-System
This is my solution to Pintos Project 1.

## COURSE INFO
CSE 521: Introduction to Operating Systems
Computer Science and Engineering Department, University at Buffalo
Instructor: Tevfik Kosar  tkosar@buffalo.edu 

## Description
Concepts of operating systems described in terms of function, structure, and implementation; particular emphasis on multiprogramming. Example concepts are: concurrent programming, virtual memory, scheduling policies for CPU and secondary storage, deadlocks, file systems, and protection. Concepts will be illustrated with examples from existing operating systems.

## NOTICE
This is my solution of the assignment, not the standard solution. The algorithm used in this project may not be the best.
Documents are not provided in this repo.

## Grade of Project 1
TOTAL TESTING SCORE: 100.0%
ALL TESTED PASSED -- PERFECT SCORE

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

SUMMARY BY TEST SET

Test Set                                      Pts Max  % Ttl  % Max
--------------------------------------------- --- --- ------ ------
tests/threads/Rubric.alarm                     18/ 18  20.0%/ 20.0%
tests/threads/Rubric.priority                  38/ 38  40.0%/ 40.0%
tests/threads/Rubric.mlfqs                     37/ 37  40.0%/ 40.0%
--------------------------------------------- --- --- ------ ------
Total                                                 100.0%/100.0%

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

SUMMARY OF INDIVIDUAL TESTS

Functionality and robustness of alarm clock (tests/threads/Rubric.alarm):
	     4/ 4 tests/threads/alarm-single
	     4/ 4 tests/threads/alarm-multiple
	     4/ 4 tests/threads/alarm-simultaneous
	     4/ 4 tests/threads/alarm-priority

	     1/ 1 tests/threads/alarm-zero
	     1/ 1 tests/threads/alarm-negative

	- Section summary.
	      6/  6 tests passed
	     18/ 18 points subtotal

Functionality of priority scheduler (tests/threads/Rubric.priority):
	     3/ 3 tests/threads/priority-change
	     3/ 3 tests/threads/priority-preempt

	     3/ 3 tests/threads/priority-fifo
	     3/ 3 tests/threads/priority-sema
	     3/ 3 tests/threads/priority-condvar

	     3/ 3 tests/threads/priority-donate-one
	     3/ 3 tests/threads/priority-donate-multiple
	     3/ 3 tests/threads/priority-donate-multiple2
	     3/ 3 tests/threads/priority-donate-nest
	     5/ 5 tests/threads/priority-donate-chain
	     3/ 3 tests/threads/priority-donate-sema
	     3/ 3 tests/threads/priority-donate-lower

	- Section summary.
	     12/ 12 tests passed
	     38/ 38 points subtotal

Functionality of advanced scheduler (tests/threads/Rubric.mlfqs):
	     5/ 5 tests/threads/mlfqs-load-1
	     5/ 5 tests/threads/mlfqs-load-60
	     3/ 3 tests/threads/mlfqs-load-avg

	     5/ 5 tests/threads/mlfqs-recent-1

	     5/ 5 tests/threads/mlfqs-fair-2
	     3/ 3 tests/threads/mlfqs-fair-20

	     4/ 4 tests/threads/mlfqs-nice-2
	     2/ 2 tests/threads/mlfqs-nice-10

	     5/ 5 tests/threads/mlfqs-block

	- Section summary.
	      9/  9 tests passed
	     37/ 37 points subtotal

## Grade of Project 2
TOTAL TESTING SCORE: 88.1%

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

SUMMARY BY TEST SET

Test Set                                      Pts Max  % Ttl  % Max
--------------------------------------------- --- --- ------ ------
tests/userprog/Rubric.functionality           102/108  33.1%/ 35.0%
tests/userprog/Rubric.robustness               88/ 88  25.0%/ 25.0%
tests/userprog/no-vm/Rubric                     0/  1   0.0%/ 10.0%
tests/filesys/base/Rubric                      30/ 30  30.0%/ 30.0%
--------------------------------------------- --- --- ------ ------
Total                                                  88.1%/100.0%

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

SUMMARY OF INDIVIDUAL TESTS

Functionality of system calls (tests/userprog/Rubric.functionality):
	- Test argument passing on Pintos command line.
	     3/ 3 tests/userprog/args-none
	     3/ 3 tests/userprog/args-single
	     3/ 3 tests/userprog/args-multiple
	     3/ 3 tests/userprog/args-many
	     3/ 3 tests/userprog/args-dbl-space

	- Test "create" system call.
	     3/ 3 tests/userprog/create-empty
	     3/ 3 tests/userprog/create-long
	     3/ 3 tests/userprog/create-normal
	     3/ 3 tests/userprog/create-exists

	- Test "open" system call.
	     3/ 3 tests/userprog/open-missing
	     3/ 3 tests/userprog/open-normal
	     3/ 3 tests/userprog/open-twice

	- Test "read" system call.
	     3/ 3 tests/userprog/read-normal
	     3/ 3 tests/userprog/read-zero

	- Test "write" system call.
	     3/ 3 tests/userprog/write-normal
	     3/ 3 tests/userprog/write-zero

	- Test "close" system call.
	     3/ 3 tests/userprog/close-normal

	- Test "exec" system call.
	     5/ 5 tests/userprog/exec-once
	     5/ 5 tests/userprog/exec-multiple
	     5/ 5 tests/userprog/exec-arg

	- Test "wait" system call.
	     5/ 5 tests/userprog/wait-simple
	     5/ 5 tests/userprog/wait-twice

	- Test "exit" system call.
	     5/ 5 tests/userprog/exit

	- Test "halt" system call.
	     3/ 3 tests/userprog/halt

	- Test recursive execution of user programs.
	    15/15 tests/userprog/multi-recurse

	- Test read-only executable feature.
	     3/ 3 tests/userprog/rox-simple
	  ** 0/ 3 tests/userprog/rox-child
	  ** 0/ 3 tests/userprog/rox-multichild

	- Section summary.
	     26/ 28 tests passed
	    102/108 points subtotal

Robustness of system calls (tests/userprog/Rubric.robustness):
	- Test robustness of file descriptor handling.
	     2/ 2 tests/userprog/close-stdin
	     2/ 2 tests/userprog/close-stdout
	     2/ 2 tests/userprog/close-bad-fd
	     2/ 2 tests/userprog/close-twice
	     2/ 2 tests/userprog/read-bad-fd
	     2/ 2 tests/userprog/read-stdout
	     2/ 2 tests/userprog/write-bad-fd
	     2/ 2 tests/userprog/write-stdin
	     2/ 2 tests/userprog/multi-child-fd

	- Test robustness of pointer handling.
	     3/ 3 tests/userprog/create-bad-ptr
	     3/ 3 tests/userprog/exec-bad-ptr
	     3/ 3 tests/userprog/open-bad-ptr
	     3/ 3 tests/userprog/read-bad-ptr
	     3/ 3 tests/userprog/write-bad-ptr

	- Test robustness of buffer copying across page boundaries.
	     3/ 3 tests/userprog/create-bound
	     3/ 3 tests/userprog/open-boundary
	     3/ 3 tests/userprog/read-boundary
	     3/ 3 tests/userprog/write-boundary

	- Test handling of null pointer and empty strings.
	     2/ 2 tests/userprog/create-null
	     2/ 2 tests/userprog/open-null
	     2/ 2 tests/userprog/open-empty

	- Test robustness of system call implementation.
	     3/ 3 tests/userprog/sc-bad-arg
	     3/ 3 tests/userprog/sc-bad-sp
	     5/ 5 tests/userprog/sc-boundary
	     5/ 5 tests/userprog/sc-boundary-2

	- Test robustness of "exec" and "wait" system calls.
	     5/ 5 tests/userprog/exec-missing
	     5/ 5 tests/userprog/wait-bad-pid
	     5/ 5 tests/userprog/wait-killed

	- Test robustness of exception handling.
	     1/ 1 tests/userprog/bad-read
	     1/ 1 tests/userprog/bad-write
	     1/ 1 tests/userprog/bad-jump
	     1/ 1 tests/userprog/bad-read2
	     1/ 1 tests/userprog/bad-write2
	     1/ 1 tests/userprog/bad-jump2

	- Section summary.
	     34/ 34 tests passed
	     88/ 88 points subtotal

Functionality of features that VM might break (tests/userprog/no-vm/Rubric):

	  ** 0/ 1 tests/userprog/no-vm/multi-oom

	- Section summary.
	      0/  1 tests passed
	      0/  1 points subtotal

Functionality of base file system (tests/filesys/base/Rubric):
	- Test basic support for small files.
	     1/ 1 tests/filesys/base/sm-create
	     2/ 2 tests/filesys/base/sm-full
	     2/ 2 tests/filesys/base/sm-random
	     2/ 2 tests/filesys/base/sm-seq-block
	     3/ 3 tests/filesys/base/sm-seq-random

	- Test basic support for large files.
	     1/ 1 tests/filesys/base/lg-create
	     2/ 2 tests/filesys/base/lg-full
	     2/ 2 tests/filesys/base/lg-random
	     2/ 2 tests/filesys/base/lg-seq-block
	     3/ 3 tests/filesys/base/lg-seq-random

	- Test synchronized multiprogram access to files.
	     4/ 4 tests/filesys/base/syn-read
	     4/ 4 tests/filesys/base/syn-write
	     2/ 2 tests/filesys/base/syn-remove

	- Section summary.
	     13/ 13 tests passed
	     30/ 30 points subtotal
