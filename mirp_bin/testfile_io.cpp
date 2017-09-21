/*! \file
 *
 * \brief Helper functions for reading/writing test files
 */

#include "mirp_bin/testfile_io.hpp"
#include "mirp_bin/data_entry.hpp"
#include "mirp_bin/test_common.hpp"
#include <mirp/shell.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <limits>

// Maximum line length
const std::streamsize max_length = std::numeric_limits<std::streamsize>::max();

namespace mirp {

integral_single_data testfile_read_integral_single(const std::string & filepath, int n, bool is_input)
{
    using std::ifstream;

    ifstream infile(filepath, ifstream::in);
    //infile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if(!infile.is_open())
        throw std::runtime_error(std::string("Unable to open file \"") + filepath + "\" for reading");

    integral_single_data data;

    // read in the header
    while(infile.peek() == '#')
    {
        std::string line;
        std::getline(infile, line);
        data.header += line + "\n";
    }

    // Read in the number of digits
    if(!is_input)
        infile >> data.ndigits >> data.working_prec;

    file_skip_comments(infile, '#');

    while(true)
    {
        // We will fill in the shells here, then add to the data
        integral_single_data_entry ent;

        file_skip_comments(infile, '#');
        if(!infile.good())
            break;

        // read in n gaussians
        for(int i = 0; i < n; i++)
        {
            gaussian_single g;
            infile >> g.lmn[0] >> g.lmn[1] >> g.lmn[2]
                   >> g.xyz[0] >> g.xyz[1] >> g.xyz[2]
                   >> g.alpha;
            ent.g.push_back(std::move(g));
        }

        // If this is not an input file, read the integral also
        if(!is_input)
        {
            // the first one reads in the rest of the line (which should
            // be empty). The second will contain the integral
            infile.ignore(max_length, '\n');//std::getline(infile, ent.integral);
            std::getline(infile, ent.integral);
        }

        // did we reach eof in reading the above?
        // if so, don't add what the entry is
        if(!infile.good())
            break;

        // add the entry
        data.entries.push_back(std::move(ent));
    }

    std::cout << "Read " << data.entries.size() << " values from " << filepath << "\n";
    return data;
}


void testfile_write_integral_single(const std::string & filepath, const integral_single_data & data)
{
    using std::ofstream;

    ofstream outfile;
    outfile.open(filepath, ofstream::out | ofstream::trunc);

    if(!outfile.is_open())
        throw std::runtime_error(std::string("Unable to open file \"") + filepath + "\" for writing");

    outfile.exceptions(ofstream::badbit | ofstream::failbit);

    outfile << data.header;
    outfile << data.ndigits << " " << data.working_prec << "\n";
    for(const auto & ent : data.entries)
    {
        for(const auto & g : ent.g)
        {
            outfile << g.lmn[0] << " " << g.lmn[1] << " " << g.lmn[2] << " "
                    << g.xyz[0] << " " << g.xyz[1] << " " << g.xyz[2] << " "
                    << g.alpha << "\n";
        }

        outfile << ent.integral << "\n\n";
    }
}



integral_data testfile_read_integral(const std::string & filepath,
                                     int n,
                                     bool is_input)
{
    using std::ifstream;

    ifstream infile(filepath, ifstream::in);
    if(!infile.is_open())
        throw std::runtime_error(std::string("Unable to open file \"") + filepath + "\" for reading");

    integral_data data;

    // read in the header
    while(infile.peek() == '#')
    {
        std::string line;
        std::getline(infile, line);
        data.header += line + "\n";
    }

    // Read in the number of digits
    if(!is_input)
        infile >> data.ndigits >> data.working_prec;

    file_skip_comments(infile, '#');

    while(true)
    {
        // We will fill in the shells here, then add to the data
        integral_data_entry ent;

        file_skip_comments(infile, '#');
        if(!infile.good())
            break;

        // read in n gaussians
        for(int i = 0; i < n; i++)
        {
            gaussian_shell_str g;
            infile >> g.Z >> g.am >> g.nprim >> g.ngeneral
                   >> g.xyz[0] >> g.xyz[1] >> g.xyz[2];

            g.alpha.resize(g.nprim);
            g.coeff.resize(g.nprim*g.ngeneral);

            for(int j = 0; j < g.nprim; j++)
            {
                infile >> g.alpha[j];
                for(int k = 0; k < g.ngeneral; k++)
                    infile >> g.coeff[k*g.nprim+j];
            }

            ent.g.push_back(std::move(g));
        }

        // read in the integral values 
        if(!is_input)
        {
            // Read and discard the rest of the line
            infile.ignore(max_length, '\n');

            // number of integrals we should be reading
            size_t nintegral = 1;
            for(const auto & it : ent.g)
                nintegral *= MIRP_NCART(it.am)*it.ngeneral;

            // If this isn't an input file, read the integrals
            int dummy; // integral index (not needed)

            ent.integrals.resize(nintegral);

            for(size_t i = 0; i < nintegral; i++)
            {
                infile >> dummy;
                std::getline(infile, ent.integrals[i]);
            }
        }

        // did we reach eof in reading the above?
        // if so, don't add what the entry is
        if(!infile.good())
            break;

        // add to the entries
        data.entries.push_back(std::move(ent));
    }

    std::cout << "Read " << data.entries.size() << " values from " << filepath << "\n";
    return data;
}


void testfile_write_integral(const std::string & filepath, const integral_data & data)
{
    using std::ofstream;

    ofstream outfile;
    outfile.open(filepath, ofstream::out | ofstream::trunc);

    if(!outfile.is_open())
        throw std::runtime_error(std::string("Unable to open file \"") + filepath + "\" for writing");

    outfile.exceptions(ofstream::badbit | ofstream::failbit);

    outfile << data.header;
    outfile << data.ndigits << " " << data.working_prec << "\n";
    for(const auto & ent : data.entries)
    {
        for(const auto & g : ent.g)
        {
            outfile << g.Z << " " << g.am << " " << g.nprim << " " << g.ngeneral << " "
                    << g.xyz[0] << " " << g.xyz[1] << " " << g.xyz[2] << "\n";

            for(int i = 0; i < g.nprim; i++)
            {
                outfile << g.alpha[i];
                for(int j = 0; j < g.ngeneral; j++)
                    outfile << " " << g.coeff[j*g.nprim + i];
                outfile << "\n";
            }
        }


        size_t ncart = 1;
        for(const auto & it : ent.g)
            ncart *= MIRP_NCART(it.am) * it.ngeneral;

        for(size_t i = 0; i < ncart; i++)
            outfile << i << "  " << ent.integrals[i] << "\n";

        outfile << "\n";
    }
}


} // closing namespace mirp

