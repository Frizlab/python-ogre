#! /bin/sh

set -e

VERSION=0.7.0
DATE=`date +%Y%m%d`

cvsdirname=`pwd`
dirname=gccxml-${VERSION}+cvs${DATE}.orig
tarname=gccxml_${VERSION}+cvs${DATE}.orig.tar.gz

cvs -z3 export -D ${DATE} -d ${dirname} gccxml

# Remove files with (a) no license, or (b) non-free license.
#
rm -rf ${dirname}/GCC_XML/Support/Borland
rm -f  ${dirname}/GCC/libiberty/xatexit.c

# Build tarball
#
rm -rf ../${dirname}
mv ${dirname} ..
cd ..
tar czf ${tarname} ${dirname}

