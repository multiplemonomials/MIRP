/*! \file
 *
 * \brief Helper functions for reading/writing test files
 */

#pragma once

#include "mirp_bin/data_entry.hpp"

namespace mirp {

// forward declarations
struct integral_single_data;
struct integral_data;

/*! \brief Read generic single integral test data from a file
 *
 * If \p is_input is set to true, then the returned data
 * does not have the integral_single_data_entry::integral member
 * populated.
 *
 * \throw std::runtime_error if there is a problem opening or
 *        writing to the file
 *
 * \param [in] filepath  Path to the file to read from
 * \param [in] n         Number of centers in the integral (2 center, 4 center, etc)
 * \param [in] is_input  True if the file is a test input file, false if it is a data file
 * \return Data read from the file
 */
integral_single_data testfile_read_integral_single(const std::string & filepath,
                                                   int n,
                                                   bool is_input);


/*! \brief Write generic single integral test data to a file
 *
 * Any existing file at \p filepath will be overwritten
 *
 * \throw std::runtime_error if there is a problem opening or
 *        writing to the file
 *
 * \param [in] filepath  Path to the file to read from
 * \param [in] data      The data to write to the file
 */
void testfile_write_integral_single(const std::string & filepath,
                                    const integral_single_data & data);



/*! \brief Read generic contracted integral test data from a file
 *
 * If \p is_input is set to true, then the returned data
 * does not have the integral_data_entry::integral member
 * populated.
 *
 * \throw std::runtime_error if there is a problem opening or
 *        writing to the file
 *
 * \param [in] filepath  Path to the file to read from
 * \param [in] n         Number of centers in the integral (2 center, 4 center, etc)
 * \param [in] is_input  True if the file is a test input file, false if it is a data file
 * \return Data read from the file
 */
integral_data testfile_read_integral(const std::string & filepath,
                                     int n,
                                     bool is_input);


/*! \brief Write generic contracted integral test data to a file
 *
 * Any existing file at \p filepath will be overwritten
 *
 * \throw std::runtime_error if there is a problem opening or
 *        writing to the file
 *
 * \param [in] filepath  Path to the file to read from
 * \param [in] data      The data to write to the file
 */
void testfile_write_integral(const std::string & filepath,
                             const integral_data & data);


} // closing namespace mirp

