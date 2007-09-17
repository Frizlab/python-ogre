INSTALL_DIR=`pwd`
ROOT=$INSTALL_DIR/root
PREFIX=$ROOT/usr
DOWNLOADS=$INSTALL_DIR/downloads
WGETARGS="-c -nc --tries=3 --random-wait"
WGET="`which wget` $WGETARGS"

TEMPFILE=`mktemp`
cat << EOF > $TEMPFILE
#!/usr/bin/python
import sys
version = sys.version.split('.')
print "%s.%s" % (version[0], version[1])
EOF
PYTHONVERSION=`python $TEMPFILE`
rm $TEMPFILE

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$PREFIX/lib/pkgconfig
export LD_LIBRARY_PATH=$PREFIX/lib
export CFLAGS="-I$PREFIX/include -L$PREFIX/lib"
export CXXFLAGS=$CFLAGS
export PATH=$PATH:$PREFIX/bin
export PYTHONPATH=$PREFIX/lib/python$PYTHONVERSION/site-packages

mkdir -p $PREFIX
mkdir -p $PREFIX/lib
mkdir -p $PREFIX/include
mkdir -p $DOWNLOADS
