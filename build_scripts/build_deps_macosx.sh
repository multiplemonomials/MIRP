#!/bin/bash

set -eu

MYDIR="$(cd "$(dirname "$0")" && pwd)"
MIRP_DIR="$(dirname ${MYDIR})"

ARCH=$1
MIRP_VER=$(cat "${MIRP_DIR}/VERSION")
PARALLEL=4 # Number of concurrent compilations (use make -j${PARALLEL})

GMP_VER=6.1.2
MPFR_VER=3.1.5
FLINT_VER=2.5.2
ARB_VER=2.12.0

GMP_DIR="gmp-${GMP_VER}"
MPFR_DIR="mpfr-${MPFR_VER}"
FLINT_DIR="flint-${FLINT_VER}"
ARB_DIR="arb-${ARB_VER}"

GMP_FILE="gmp-${GMP_VER}.tar.bz2"
MPFR_FILE="mpfr-${MPFR_VER}.tar.bz2"
FLINT_FILE="flint-${FLINT_VER}.tar.gz"
ARB_FILE="arb-${ARB_VER}.tar.gz"

GMP_URL="https://ftp.gnu.org/gnu/gmp/${GMP_FILE}"
MPFR_URL="https://ftp.gnu.org/gnu/mpfr/${MPFR_FILE}"
FLINT_URL="http://flintlib.org/${FLINT_FILE}"
ARB_URL="https://github.com/fredrik-johansson/arb/archive/${ARB_VER}.tar.gz"

BUILD_DIR="deps_build_${ARCH}"
PREFIX="$(pwd)/mirp_deps_v${MIRP_VER}_macosx_${ARCH}"

rm -Rf ${BUILD_DIR}
rm -Rf ${PREFIX}

mkdir -p ${BUILD_DIR}

# Download the files (if they don't already exist)
if [[ ! -f "${GMP_FILE}" ]];   then wget -O "${GMP_FILE}"   "${GMP_URL}";   fi
if [[ ! -f "${MPFR_FILE}" ]];  then wget -O "${MPFR_FILE}"  "${MPFR_URL}";  fi
if [[ ! -f "${FLINT_FILE}" ]]; then wget -O "${FLINT_FILE}" "${FLINT_URL}"; fi
if [[ ! -f "${ARB_FILE}" ]];   then wget -O "${ARB_FILE}"   "${ARB_URL}";   fi

cd ${BUILD_DIR}

rm -Rf "${GMP_DIR}"
rm -Rf "${MPFR_DIR}"
rm -Rf "${FLINT_DIR}"
rm -Rf "${ARB_DIR}"
tar -xf "../${GMP_FILE}"
tar -xf "../${MPFR_FILE}"
tar -xf "../${FLINT_FILE}"
tar -xf "../${ARB_FILE}"

mkdir -p "${GMP_DIR}/build"
mkdir -p "${MPFR_DIR}/build"

###################
# Common flags
###################
CFLAGS="-O2 -march=${ARCH}"
CC=`which clang`

###################
# Build GMP
###################
cd "${GMP_DIR}/build"
../configure CFLAGS="${CFLAGS}" CC="${CC}" \
             --build=x86_64-apple-darwin \
             --disable-static \
             --prefix=${PREFIX}

make -j${PARALLEL}
#make check
make install
cd ../../

###################
# Build MPFR
###################
cd "${MPFR_DIR}/build"
../configure --with-gmp=${PREFIX} \
             CFLAGS="${CFLAGS}" CC="${CC}" \
             --build=x86_64-apple-darwin \
             --disable-static \
             --prefix=${PREFIX}

make -j${PARALLEL}
#make check
make install
cd ../../

###################
# Build FLINT
###################
cd "${FLINT_DIR}"
./configure --with-gmp=${PREFIX} --with-mpfr=${PREFIX} \
            CFLAGS="${CFLAGS}" CC="${CC}" \
            --disable-static \
            --prefix=${PREFIX}

make -j${PARALLEL}
#make check
make install
cd ../

###################
# Build ARB
###################
cd "${ARB_DIR}"
./configure --with-gmp=${PREFIX} --with-mpfr=${PREFIX} --with-flint=${PREFIX} \
            CFLAGS="${CFLAGS}" CC="${CC}" \
            --disable-static \
            --prefix=${PREFIX}

make -j${PARALLEL}
#make check
make install
cd ../


# Cleanup
rm -Rf "${GMP_DIR}"   "${GMP_FILE}"
rm -Rf "${MPFR_DIR}"  "${MPFR_FILE}"
rm -Rf "${FLINT_DIR}" "${FLINT_FILE}"
rm -Rf "${ARB_DIR}"   "${ARB_FILE}"

cd ../
rm -Rf ${BUILD_DIR}

# Remove unneeded .la files
rm ${PREFIX}/lib/*.la

# Fix the rpaths (if we have install_name_tool)
if [[ $(command -v install_name_tool 2>&1) ]]
then
    for I in ${PREFIX}/lib/*
    do
        # If this isn't a symlink
        if [[ ! -L "$I" ]]
        then 
            RP1=`otool -l $I | grep -A3 LC_RPATH | tail -n 1 | awk '{print $2}'`
            install_name_tool -add_rpath '@loader_path' "$I"
            RP2=`otool -l $I | grep -A3 LC_RPATH | tail -n 1 | awk '{print $2}'`
            echo "${I}: RPATH changed from \"${RP1}\" to \"${RP2}\""

            # Change all the dependency entries to use rpath
            NEWID="@rpath/$(basename ${I})"
            install_name_tool -id "${NEWID}" "${I}"
            otool -L ${I} | tail -n +2 | grep ${PREFIX} | awk '{print $1}' | while read F
            do
                FNAME=`basename $F`
                NEWF="@rpath/${FNAME}"
                echo "${I}: Fixing path from ${F} to ${NEWF}"
                install_name_tool -change "$F" "${NEWF}" "$I"
            done
            echo "----------------------"
            otool -L ${I}
            echo "----------------------"
        fi
    done
else
    echo
    echo "!!! install_name_tool not installed. Skipping fixing RPATHS !!!"
    echo
fi

# Create the readme file
COMPILER_VER=$(${CC} --version | head -n 1)
BUILD_DATE=$(date +%Y-%m-%d)
cp "${MYDIR}/deps_README.in"              "${PREFIX}/README"
sed -i "" "s/MIRP_VER/${MIRP_VER}/g"         "${PREFIX}/README"
sed -i "" "s/COMPILER_VER/${COMPILER_VER}/g" "${PREFIX}/README"
sed -i "" "s/GMP_VER/${GMP_VER}/g"           "${PREFIX}/README"
sed -i "" "s/MPFR_VER/${MPFR_VER}/g"         "${PREFIX}/README"
sed -i "" "s/FLINT_VER/${FLINT_VER}/g"       "${PREFIX}/README"
sed -i "" "s/ARB_VER/${ARB_VER}/g"           "${PREFIX}/README"
sed -i "" "s/ARCH/${ARCH}/g"                 "${PREFIX}/README"
sed -i "" "s/BUILD_DATE/${BUILD_DATE}/g"     "${PREFIX}/README"
