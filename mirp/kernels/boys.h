/*! \file
 *
 * \brief Calculation of the boys function
 */

#pragma once

#include <arb.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Computes the Boys function using double precision
 *
 * See \ref boys_function
 *
 * \warning \p F must be large enough to hold (\p m + 1) values, since
 *             this is computing from zero to m.
 *
 * \param [out] F The computed values of the Boys function
 * \param [in] m The maximum order to calculate
 * \param [in] t The value at which to evaluate
 */
void mirp_boys_double(double *F, int m, double t);


/*! \brief Computes the Boys function using interval arithmetic
 *
 * \copydetails mirp_boys_double
 * \param [in] working_prec The working precision (binary digits/bits) to use in the calculation
 */
void mirp_boys_interval(arb_ptr F, int m, const arb_t t, slong working_prec);

#ifdef __cplusplus
}
#endif

