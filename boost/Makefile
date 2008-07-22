
export GCC_VERSION=`gcc --version | head -1 | sed -e's/.*) \([0-9]\)\.\([0-9]\).*/\1\2/'`
export PYTHON_VERSION=`python -V 2>&1 | sed -e's/Python \(2\.[45]\).*/\1/'`

all: libs/python/src/indexing/indexing_slice.cpp libs/python/src/indexing/python_iterator.cpp
	g++ $? \
		-I . \
		-I /usr/include/python${PYTHON_VERSION} \
		-shared -Wl,-soname,libboost_python_index-gcc${GCC_VERSION}-1_34_1.so -o libboost_python_index-gcc${GCC_VERSION}-1_34_1.so -fPIC \
		-lboost_python \
		-lpython${PYTHON_VERSION}

install:
	mkdir -p ${PREFIX}/usr/lib
	cp libboost_python_index-gcc${GCC_VERSION}-1_34_1.so ${PREFIX}/usr/lib
	ln -sf /usr/lib/libboost_python_index-gcc${GCC_VERSION}-1_34_1.so ${PREFIX}/usr/lib/libboost_python_index.so
	
	mkdir -p ${PREFIX}/usr/include
	cp -rvf boost ${PREFIX}/usr/include
	rm -rf `find ${PREFIX}/usr/include -name .svn -type d`

clean:
	rm *.so

.PHONY: install clean
