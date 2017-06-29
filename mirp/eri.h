#pragma once

#include <gmp.h>
#include <mpfr.h>
#include <arb.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void mirp_single_eri_double(double * result,
                            int l1, int m1, int n1, double alpha1, const double A[3],
                            int l2, int m2, int n2, double alpha2, const double B[3],
                            int l3, int m3, int n3, double alpha3, const double C[3],
                            int l4, int m4, int n4, double alpha4, const double D[3]);

size_t mirp_prim_eri_double(double * result,
                            int am1, double alpha1, const double A[3],
                            int am2, double alpha2, const double B[3],
                            int am3, double alpha3, const double C[3],
                            int am4, double alpha4, const double D[3]);

size_t mirp_eri_double(double * result,
                       int am1, const double A[3], int nprim1, int ngeneral1, const double * alpha1, const double * coeff1,
                       int am2, const double B[3], int nprim2, int ngeneral2, const double * alpha2, const double * coeff2,
                       int am3, const double C[3], int nprim3, int ngeneral3, const double * alpha3, const double * coeff3,
                       int am4, const double D[3], int nprim4, int ngeneral4, const double * alpha4, const double * coeff4);

void mirp_single_eri_mp(mpfr_t result,
                        int l1, int m1, int n1, const mpfr_t alpha1, const mpfr_t A[3],
                        int l2, int m2, int n2, const mpfr_t alpha2, const mpfr_t B[3],
                        int l3, int m3, int n3, const mpfr_t alpha3, const mpfr_t C[3],
                        int l4, int m4, int n4, const mpfr_t alpha4, const mpfr_t D[3],
                        mpfr_prec_t working_precision);

size_t mirp_prim_eri_mp(mpfr_t * result,
                        int am1, const mpfr_t alpha1, const mpfr_t A[3],
                        int am2, const mpfr_t alpha2, const mpfr_t B[3],
                        int am3, const mpfr_t alpha3, const mpfr_t C[3],
                        int am4, const mpfr_t alpha4, const mpfr_t D[3]);

size_t mirp_eri_mp(mpfr_t * result,
                   int am1, const mpfr_t A[3], int nprim1, int ngeneral1, const mpfr_t * alpha1, const mpfr_t * coeff1,
                   int am2, const mpfr_t B[3], int nprim2, int ngeneral2, const mpfr_t * alpha2, const mpfr_t * coeff2,
                   int am3, const mpfr_t C[3], int nprim3, int ngeneral3, const mpfr_t * alpha3, const mpfr_t * coeff3,
                   int am4, const mpfr_t D[3], int nprim4, int ngeneral4, const mpfr_t * alpha4, const mpfr_t * coeff4,
                   mpfr_prec_t working_prec);

void mirp_single_eri_interval(arb_t result,
                              int l1, int m1, int n1, const arb_t alpha1, const arb_t A[3],
                              int l2, int m2, int n2, const arb_t alpha2, const arb_t B[3],
                              int l3, int m3, int n3, const arb_t alpha3, const arb_t C[3],
                              int l4, int m4, int n4, const arb_t alpha4, const arb_t D[3],
                              slong working_prec);

size_t mirp_prim_eri_interval(arb_t * result,
                        int am1, const arb_t alpha1, const arb_t A[3],
                        int am2, const arb_t alpha2, const arb_t B[3],
                        int am3, const arb_t alpha3, const arb_t C[3],
                        int am4, const arb_t alpha4, const arb_t D[3]);

size_t mirp_eri_interval(arb_t * result,
                   int am1, const arb_t A[3], int nprim1, int ngeneral1, const arb_t * alpha1, const arb_t * coeff1,
                   int am2, const arb_t B[3], int nprim2, int ngeneral2, const arb_t * alpha2, const arb_t * coeff2,
                   int am3, const arb_t C[3], int nprim3, int ngeneral3, const arb_t * alpha3, const arb_t * coeff3,
                   int am4, const arb_t D[3], int nprim4, int ngeneral4, const arb_t * alpha4, const arb_t * coeff4,
                   slong working_prec);

#ifdef __cplusplus
}
#endif

