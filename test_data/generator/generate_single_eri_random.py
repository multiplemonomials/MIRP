#!/usr/bin/env python3

import argparse
import random
import sys
from mpmath import mp

parser = argparse.ArgumentParser()
parser.add_argument("--filename",    type=str, required=True, help="Output file name")
parser.add_argument("--max-am",      type=int, required=True, help="Maximum AM of the basis functions")
parser.add_argument("--alpha-power", type=int, required=True, help="Maximum power of the exponent (range will be 1e-x to 1e+x)")
parser.add_argument("--xyz-power",   type=int, required=True, help="Maximum power of the coordinates (range will be -1e+x to 1e+x)")
parser.add_argument("--seed",        type=int, required=True, help="Seed to use for the pseudo-random number generator")
parser.add_argument("--ndigits",     type=int, required=True, help="Number of digits for the value of the eri")
parser.add_argument("--ntest",       type=int, required=True, help="Number of tests to generate")
args = parser.parse_args()

random.seed(args.seed, version=2)


def generate_basis_function():
    with mp.workdps(args.ndigits+4):
            l = random.randint(0, args.max_am)
            m = random.randint(0, args.max_am)
            n = random.randint(0, args.max_am)

            while (l+m+n) > args.max_am:
                l = random.randint(0, args.max_am)
                m = random.randint(0, args.max_am)
                n = random.randint(0, args.max_am)

            alpha = mp.power(mp.mpf(10), random.uniform(-args.alpha_power, args.alpha_power))
            x = mp.power(mp.mpf(10), random.uniform(-args.xyz_power, args.xyz_power))
            y = mp.power(mp.mpf(10), random.uniform(-args.xyz_power, args.xyz_power))
            z = mp.power(mp.mpf(10), random.uniform(-args.xyz_power, args.xyz_power))
            x *= mp.power(-1, random.randint(1, 2))
            y *= mp.power(-1, random.randint(1, 2))
            z *= mp.power(-1, random.randint(1, 2))

            alpha = mp.nstr(alpha, args.ndigits, min_fixed=1, max_fixed=0)
            x = mp.nstr(x, args.ndigits, min_fixed=1, max_fixed=0)
            y = mp.nstr(y, args.ndigits, min_fixed=1, max_fixed=0)
            z = mp.nstr(z, args.ndigits, min_fixed=1, max_fixed=0)

    return (l, m, n, x, y, z, alpha)


# Write out the file
with open(args.filename, 'w') as f:
    f.write("# THIS FILE IS GENERATED VIA A SCRIPT. DO NOT EDIT\n")
    f.write("#\n")
    f.write("# Input parameters for ERI generated with:\n")
    f.write("#   " + " ".join(sys.argv[:]) + "\n")
    f.write("#\n")

    for i in range(0, args.ntest):
        bf1 = generate_basis_function()
        bf2 = generate_basis_function()
        bf3 = generate_basis_function()
        bf4 = generate_basis_function()
     
        f.write("{} {} {} {} {} {} {}\n".format(*bf1))  
        f.write("{} {} {} {} {} {} {}\n".format(*bf2))  
        f.write("{} {} {} {} {} {} {}\n".format(*bf3))  
        f.write("{} {} {} {} {} {} {}\n".format(*bf4))  
        f.write("\n")
