/*
 * dt_int.c: Checked integers for Unit 5, Section A.
 *
 * In C, signed integer overflow has undefined behavior.
 * The compiler can assume that signed overflow does not occur.
 * An optimizer can remove a guarded check after the arithmetic.
 *
 *     long long sum = a + b
 *     if (b > 0 && sum < a) return DT_ERR_OVERFLOW // optimizer may remove this branch
 *
 * Check before the operation. Use comparison values that cannot overflow.
 * A positive b overflows when a > LLONG_MAX - b.
 * A negative b produces a result below LLONG_MIN when a < LLONG_MIN - b.
 * These comparison subtractions are safe.
 *
 * Multiplication has more cases. LLONG_MIN multiplied by -1 overflows.
 * LLONG_MIN divided by -1 also has undefined behavior.
 *
 * These stubs report overflow for every input. The normal cases fail until you implement them.
 */

#include "dt.h"

#include <limits.h>

/*
 * dt_int_add computes a + b.
 * It returns DT_ERR_OVERFLOW and does not change *out for an overflow.
 */
dt_status dt_int_add(long long a, long long b, long long *out)
{
    /* TODO: Check for overflow. Then write the sum to *out.
       dt_int_add(2, 3, &out)          -> DT_OK, out = 5
       dt_int_add(LLONG_MAX, 1, &out)  -> DT_ERR_OVERFLOW, out untouched
       cases/normal/int_arithmetic.case, cases/boundary/int_overflow_add.case */
    //(void)a;
    //(void)b;
    //(void)out;
    //return DT_ERR_OVERFLOW;

    // i checked LLONG_MAX - b because b is positive, and adding it
    // can make the sum go past the max value of long long
    if (b > 0 && a > LLONG_MAX - b) {
        return DT_ERR_OVERFLOW;
    }

    // i l checked LLONG_MIN - b because b is negative, and again 
    // if you add it to a it be go below the min value of long long
    if (b < 0 && a < LLONG_MIN - b) {
        return DT_ERR_OVERFLOW;
    }

    // otherwise, its fine to add them
    *out = a + b ;
    return DT_OK;
}

/*
 * dt_int_sub computes a - b.
 * It returns DT_ERR_OVERFLOW and does not change *out for an overflow.
 */
dt_status dt_int_sub(long long a, long long b, long long *out)
{
    /* TODO: Check subtraction directly.
       The value -LLONG_MIN does not exist in long long.
       Therefore, dt_int_add(a, -b, out) fails when b is LLONG_MIN.
       dt_int_sub(10, 4, &out)                 -> DT_OK, out = 6
       dt_int_sub(LLONG_MIN + 1, 2, &out)      -> DT_ERR_OVERFLOW, out untouched
       cases/normal/int_arithmetic.case, cases/boundary/int_overflow_sub_min.case */
    // b is positive, so a - b moves down
    // when a < LLONG_MIN + b (LLONG_MIN + b can't overflow)
    if (b > 0 && a < LLONG_MIN + b) {
        return DT_ERR_OVERFLOW;
    }

    // b is negative, so a - b moves up
    // when a > LLONG_MAX + b (LLONG_MAX + b can't overflow)
    // covers b == LLONG_MIN without ever computing -b
    if (b < 0 && a > LLONG_MAX + b) {
        return DT_ERR_OVERFLOW;
    }

    *out = a - b;
    return DT_OK;
}

/*
 * dt_int_mul computes a * b.
 * It returns DT_ERR_OVERFLOW and does not change *out for an overflow.
 */
dt_status dt_int_mul(long long a, long long b, long long *out)
{
    /* TODO: Handle zero first. Then handle LLONG_MIN with -1.
       Finally, handle the remaining values.
       dt_int_mul(6, 7, &out)            -> DT_OK, out = 42
       dt_int_mul(LLONG_MIN, 0, &out)    -> DT_OK, out = 0
       dt_int_mul(LLONG_MIN, -1, &out)   -> DT_ERR_OVERFLOW, out untouched
       cases/normal/int_arithmetic.case,
       cases/boundary/int_mul_min_by_negative_one.case */
    //(void)a;
    //(void)b;
    //(void)out;
    //return DT_ERR_OVERFLOW;

    // handling the zero; zero property of multiplication
    if (a == 0 || b == 0){
        *out = 0;
        return DT_OK;
    }

    // long long value = -9223372036854775808 to 9223372036854775807
    // if i multiply LLONG_MIN by - 1, it will exceed the LLONG_MAX by 1
    if (( a == -1 && b == LLONG_MIN) || (a == LLONG_MIN && b == -1)){
        return DT_ERR_OVERFLOW;
    }

    // both are positive, and checking if it will exceed LLONG_MAX
    // divided LLONG_MAX by b because a * b > LLONG_MAX means a > LLONG_MAX / b
    if (a > 0 && b > 0 && a > LLONG_MAX / b) {
        return DT_ERR_OVERFLOW;
    }

    // * different signs; checking if the product goes below LLONG_MIN
    // b is neg; a * b < LLONG_MIN means that b < LLONG_MIN / a
    // a is postive so  dividing it by LLONG_MIN retains sign
    if (a > 0 && b < 0 && b < LLONG_MIN / a) {
        return DT_ERR_OVERFLOW;
    }

    // a is neg; a * b < LLONG_MIN means that a < LLONG_MIN / b
    // b is postive so  dividing it by LLONG_MIN retains sign
    if (a < 0 && b > 0 && a < LLONG_MIN / b) {
        return DT_ERR_OVERFLOW;
    }

    // * same signs will result to positive

    // both postive; checking if it will exceed LLONG_MAX
    if (a > 0 && b > 0 && a > LLONG_MAX / b) {
        return DT_ERR_OVERFLOW;
    }

    // both negative; checking if the product exceeds LLONG_MAX
    if (a < 0 && b < 0 && a < LLONG_MAX / b) {
        return DT_ERR_OVERFLOW;
    }

    *out = a * b;
    return DT_OK;

}
