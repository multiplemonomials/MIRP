/*! \file
 *
 * \brief Calculation of the boys function using interval arithmetic
 */

#include "mirp/kernels/boys.h"
#include <assert.h>

void mirp_boys_interval(arb_t *F, int m, const arb_t t, slong working_prec)
{
    assert(m >= 0);
    assert(working_prec > 0);

    int i;

    arb_t t2, et, sum, term, lastterm, test;
    arb_t tmp1, tmp2, tmp3;
    arb_init(t2);
    arb_init(et);
    arb_init(sum);
    arb_init(term);
    arb_init(lastterm);
    arb_init(test);
    arb_init(tmp1);
    arb_init(tmp2);
    arb_init(tmp3);

    /* t2 = 2*t */
    arb_mul_ui(t2, t, 2, working_prec);

    /* et = exp(-x)
       Note: x is always positive, so we can use arb_neg
     */
    arb_neg(et, t);
    arb_exp(et, et, working_prec);

    int do_short = 0;

    /* The short-range formula converges much better for
     * t < (m + 3/2)
     * So skip the long range if that happens
     * Note that this is a conservative bound, and could probably be increased
     */
    arb_set_si(test, 2*m+3);
    arb_div_si(test, test, 2, working_prec);

    if(arb_lt(t, test))
        do_short = 1;

    if(!do_short)
    {
        /* Attempt the long-range approximation */
        arb_const_pi(tmp1, working_prec);
        arb_div(tmp1, tmp1, t, working_prec);
        arb_sqrt(tmp1, tmp1, working_prec);
        arb_div_si(tmp1, tmp1, 2, working_prec);

        for(i = 1; i <= m; i++)
        {
            arb_set_si(tmp2, 2*i-1);
            arb_div(tmp2, tmp2, t2, working_prec);
            arb_mul(tmp1, tmp1, tmp2, working_prec);
        }

        arb_set(F[m], tmp1);

        /* Determine the error associated with the long-range approximation */
        arb_zero(sum);
        arb_set_ui(term, 1);

        i = 0;
        do {
            i++;
            arb_abs(lastterm, term);
            arb_mul_si(term, term, 2*m - 2*i + 1, working_prec);
            arb_div(term, term, t2, working_prec);
            arb_abs(test, term);

            if(arb_gt(test, lastterm))
                break;

            arb_set(test, sum);
            arb_add(sum, sum, term, working_prec);
        } while(!arb_contains(sum, test)); /* Is the old term contained completely
                                              within the new term */

        //printf("Done with long-range test in %d cycles\n", i);

        arb_mul(sum, sum, et, working_prec);
        arb_div(sum, sum, t2, working_prec);

        /*
         * Determine if this error is satisfactory
         * If not, mark that we have to do the short-range version
         */
        arb_sub(test, F[m], sum, working_prec);
        if(!arb_overlaps(F[m], test))
            do_short = 1;
    }

    if(do_short)
    {
        arb_set_ui(sum, 1);
        arb_set_ui(term, 1);
    
        i = 0;
        do
        {
            i++;
            arb_mul(term, term, t2, working_prec);
            arb_div_si(term, term, 2*m + 2*i + 1, working_prec);

            /* store the old term, then update and calculate the difference */
            arb_set(test, sum);
            arb_add(sum, sum, term, working_prec);
        } while(!arb_contains(sum, test)); /* Is the old term contained completely
                                              within the new term */

        arb_mul(F[m], sum, et, working_prec);
        arb_div_si(F[m], F[m], 2*m+1, working_prec);
        //printf("Done with short-range approximation in %d cycles\n", i);
    }
    
    /* Now do downwards recursion */
    for(i = m - 1; i >= 0; i--)
    {
        /* F[m] = (t2 * F[m + 1] + et) / (2 * m + 1) */
        arb_mul(F[i], t2, F[i + 1], working_prec);
        arb_add(F[i], F[i], et, working_prec);
        arb_div_ui(F[i], F[i], 2 * i + 1, working_prec);
    }

    arb_clear(t2);
    arb_clear(et);
    arb_clear(sum);
    arb_clear(term);
    arb_clear(lastterm);
    arb_clear(test);
    arb_clear(tmp1);
    arb_clear(tmp2);
    arb_clear(tmp3);
}

