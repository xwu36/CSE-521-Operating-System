#ifndef THREADS_FIXED_POINT_H
#define THREADS_FIXED_POINT_H

typedef int fixed_point_number;

#define F (1 << 16)
//Convert n to fixed point:
#define INT_TO_FP(A) (A * F)

#define FP_TO_INT(A) (A / F)

//Convert x to integer (rounding to nearest):
#define FP_TO_INT_ROUND(A) (A >= 0 ? (( A + F / 2) / F ) :(( A - F / 2) / F ))

//Add x and y:
#define FP_ADD(A,B) (A + B)

//Subtract y from x:
#define FP_SUBTRACT(A,B) (A - B)

//Add x and n:
#define FP_ADD_INT(A,B) (A + INT_TO_FP(B))

//Subtract n from x:
#define FP_SUBTRACT_INT(A,B) (A - INT_TO_FP(B))

//Multiply x by y:
#define FP_MULTIPLY(A,B) (((int64_t) A) * B / F)

//Multiply x by n:
#define FP_TIMES_INT(A,B) (A * B)

//Divide x by n:
#define FP_OVER_INT(A,B) (A / B)

//Divide x by y:
#define FP_DIVIDE(A,B) (((int64_t) A) * F / B)

#endif /* thread/fixed_point.h */
