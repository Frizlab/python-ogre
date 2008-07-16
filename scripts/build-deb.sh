#! /bin/sh

if [ ! -x python-ogre ]; then
	echo "The python-ogre directory does not exist,"
	echo "please make sure you are running this script"
	echo "from the directory you check out the code."
	exit
fi

# Run sudo so that we have the permissions to do things
sudo true

. python-ogre/scripts/00-PreReqs.sh

# The following modules are provided by the operating system
sudo apt-get install -y \
	debhelper \
	fakeroot \
	chrpath \
	python-all \
	python-setuptools \
	\
	cmake \
	scons \
	nvidia-cg-toolkit \
	zziplib-bin \
	libboost-dev \
	libboost-python-dev \
	libfreeimage-dev \
	libois-dev \
	libogre-dev \
	libopenal-dev \
	libogg-dev \
	libvorbis-dev

DISTRO_CODENAME=`lsb_release -c | sed -e's/.*:\t*//'`
if [ "x$DISTRO_CODENAME" = "xhardy" ]; then
	sudo apt-get install libceguiogre-dev
fi

# Make sure we are building the stable version
cd python-ogre
touch STABLE
cd ..

# Things we need to build ourself

# The extra boost support module
python python-ogre/BuildModule.py --failhard -r --build-deb boost_python_index || exit
sudo true
python python-ogre/BuildModule.py --failhard --install-deb boost_python_index || exit
# The gccxml version is important to us (but only during build)
python python-ogre/BuildModule.py --failhard -r --build-deb gccxml || exit
sudo true
python python-ogre/BuildModule.py --failhard --install-deb gccxml || exit
# The pygccxml is dependent on the gccxml version we just built
python python-ogre/BuildModule.py --failhard -r --build-deb pygccxml  || exit
sudo true
python python-ogre/BuildModule.py --failhard --install-deb pygccxml  || exit

# Build our custom extension to boost-indexing which pyplusplus needs
python python-ogre/BuildModule.py --failhard -r --build-deb pyplusplus || exit
sudo true
python python-ogre/BuildModule.py --failhard --install-deb pyplusplus || exit

# Generate the python-ogre package
cd python-ogre
dpkg-checkbuilddeps || exit
chmod a+x ./debian/rules
fakeroot ./debian/rules binary
