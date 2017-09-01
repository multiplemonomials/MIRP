/*! \file
 *
 * \brief Functions related to testing four-center integrals
 */

#pragma once

#include <mirp/typedefs.h>
#include <string>

namespace mirp {


/************************************************
 * Testing Single Integrals
 ************************************************/

/*! \brief Creates a test of single cartesian integrals
 *
 * Any existing output file (given by \p output_filepath) will be overwritten.
 *
 * \throw std::runtime_error if there is a problem opening the file or there
 *        there is a problem reading or writing the data
 *
 * \param [in] input_filepath  The input file to use for integral parameters
 * \param [in] output_filepath The output file to write the computed integrals to
 * \param [in] ndigits         Number of digits to calculate the integral to
 * \param [in] header          Header information to add to the file
 *                             (appended to the input file header)
 * \param [in] cb              Function that computes single cartesian integrals
 *                             to a target precision using string inputs
 */
void integral4_single_create_test(const std::string & input_filepath,
                                  const std::string & output_filepath,
                                  long ndigits, const std::string & header,
                                  cb_integral4_single_target_str cb);


/*! \brief Runs a test of single cartesian integrals using interval math
 *
 * \throw std::runtime_error if there is a problem opening the file or there
 *        there is a problem reading or writing the data
 *
 * \param [in] filepath    Path to the file with the reference data
 * \param [in] target_prec The precision (binary/bits) to test to
 * \param [in] cb          Function that computes single cartesian integrals
 *                         to a target precision using string inputs
 * \return Number of failed tests
 */
long integral4_single_run_test(const std::string & filepath,
                               long target_prec,
                               cb_integral4_single_target_str cb);


/*! \brief Test single cartesian integrals in double precision
 *
 * The integrals are tested to a lower relative accuracy to account
 * for rounding error in the double-precision integral kernels.
 *
 * \param [in] filepath  Path to the file with the reference data
 * \param [in] cb        Function that computes single cartesian integrals
 *                       in double precision
 * \return Number of failed tests
 */
long integral4_single_run_test_d(const std::string & filepath,
                                 cb_integral4_single_d cb);


/*! \brief Test single cartesian integrals in exact double precision
 *
 * The integrals are tested to be exactly equal to the reference data
 * or to integral computed with very large accuracy.
 *
 * \param [in] filepath  Path to the file with the reference data
 * \param [in] cb        Function that computes single cartesian integrals
 *                       in exact double precision
 * \param [in] cb_mp     Function that computes single cartesian integrals
 *                       to a target precision
 * \return Number of failed tests
 */
long integral4_single_run_test_exact(const std::string & filepath,
                                     cb_integral4_single_exact cb,
                                     cb_integral4_single_target cb_mp);


/************************************************
 * Testing Contracted Integrals
 ************************************************/

/*! \brief Creates a test of contracted integrals
 *
 * Any existing output file (given by \p output_filepath) will be overwritten.
 *
 * \throw std::runtime_error if there is a problem opening the file or there
 *        there is a problem reading or writing the data
 *
 * \param [in] input_filepath  The input file to use for integral parameters
 * \param [in] output_filepath The output file to write the computed integrals to
 * \param [in] ndigits         Number of digits to calculate the integral to
 * \param [in] header          Header information to add to the file
 *                             (appended to the input file header)
 * \param [in] cb              Function that computes contracted integrals
 *                             to a target precision using string inputs
 */
void integral4_create_test(const std::string & input_filepath,
                           const std::string & output_filepath,
                           long ndigits, const std::string & header,
                           cb_integral4_target_str cb);


/*! \brief Runs a test of single cartesian integrals
 *
 * \throw std::runtime_error if there is a problem opening the file or there
 *        there is a problem reading or writing the data
 *
 * \param [in] filepath    Path to the file with the reference data
 * \param [in] target_prec The precision (binary/bits) to test to
 * \param [in] cb          Function that computes contracted integrals
 *                         to a target precision using string inputs
 * \return Number of failed tests
 */
long integral4_run_test(const std::string & filepath,
                        long target_prec,
                        cb_integral4_target_str cb);


/*! \brief Test contracted integrals in double precision
 *
 * The integrals are tested to a lower relative accuracy to account
 * for rounding error in the double-precision integral kernels.
 *
 * \param [in] filepath  Path to the file with the reference data
 * \param [in] cb        Function that computes contracted integrals
 *                       in double precision
 * \return Number of failed tests
 */
long integral4_run_test_d(const std::string & filepath,
                          cb_integral4_d cb);


/*! \brief Test contracted integrals in exact double precision
 *
 * The integrals are tested to be exactly equal to the reference data
 * or to integral computed with very large accuracy.
 *
 * \param [in] filepath  Path to the file with the reference data
 * \param [in] cb        Function that computes contracted integrals
 *                       in exact double precision
 * \param [in] cb_mp     Function that computes contracted integrals
 *                       to a target precision
 * \return Number of failed tests
 */
long integral4_run_test_exact(const std::string & filepath,
                              cb_integral4_exact cb,
                              cb_integral4_target cb_mp);



} // close namespace mirp
