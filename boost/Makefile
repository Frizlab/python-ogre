
PREFIX := '/usr'
GCC_VERSION=`gcc --version | head -1 | sed -e's/.*) \([0-9]\)\.\([0-9]\).*/\1\2/'`
PYTHON_VERSION=`python -V 2>&1 | sed -e's/Python \(2\.[45]\).*/\1/'`
MT_VERSION=`if [ x${MT} = xTrue ]; then echo "mt-"; fi`
MT_FLAGS=`if [ x${MT} = xTrue ]; then echo "-D_MT"; fi`

CXXFLAGS=-g \
       -I.  \
       -I/usr/include/python${PYTHON_VERSION} \
       -I${PREFIX}/include/ \
       -I${PREFIX}/include/boost-${BOOST_VERSION_PATH} \
       -shared \
		-Wl,-soname,libboost_python_index-gcc${GCC_VERSION}-${BOOST_VERSION}.so \
		-o libboost_python_index-gcc${GCC_VERSION}-${MT_VERSION}${BOOST_VERSION}.so -fPIC \
       -lboost_python-gcc${GCC_VERSION}-${MT_VERSION}${BOOST_VERSION} \
       -L${PREFIX}/lib/ \
       -lpython${PYTHON_VERSION} \
		$(MT_FLAGS)


all: libs/python/src/indexing/indexing_slice.cpp libs/python/src/indexing/python_iterator.cpp
	@make version
	@echo "Current CXXFLAGS: ${CXXFLAGS}"
	@echo "Running g++ ${CXXFLAGS} $?"
	@g++ ${CXXFLAGS} $?

version:
	@if [ x${BOOST_VERSION} = x ]; then 								\
		echo "You have not specified a BOOST_VERSION, please do so!"; 	\
		exit 1; 														\
	fi

install:
	@make version
	mkdir -p ${PREFIX}/lib
	@echo cp libboost_python_index-gcc${GCC_VERSION}-${MT_VERSION}${BOOST_VERSION}.so ${PREFIX}/lib
	@cp libboost_python_index-gcc${GCC_VERSION}-${MT_VERSION}${BOOST_VERSION}.so ${PREFIX}/lib
	@echo ln -sf /usr/lib/libboost_python_index-gcc${GCC_VERSION}-${MT_VERSION}${BOOST_VERSION}.so ${PREFIX}/lib/libboost_python_index.so
	@ln -sf /usr/lib/libboost_python_index-gcc${GCC_VERSION}-${MT_VERSION}${BOOST_VERSION}.so ${PREFIX}/lib/libboost_python_index.so
	
	mkdir -p ${PREFIX}/include
	cp -rvf boost ${PREFIX}/include
	rm -rf `find ${PREFIX}/include -name .svn -type d`

clean:
	rm *.so

.PHONY: install clean version
