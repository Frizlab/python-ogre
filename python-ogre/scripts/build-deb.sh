#! /bin/sh

# Run sudo so that we have the permissions to do things
sudo true

# The following modules are provided by the operating system
sudo apt-get install \
	cmake \
	scons \
	nvidia-cg-toolkit \
	zziplib-bin \
	libboost-dev \
	libboost-python-dev \
	libfreeimage-dev \
	libois-dev \
	libogre-dev \
	libceguiogre-dev

# Things we need to build ourself

# The extra boost support module
python python-ogre/BuildModule.py -r --build-deb boost_python_index || exit
sudo true
python python-ogre/BuildModule.py --install-deb boost_python_index || exit
# The gccxml version is important to us (but only during build)
python python-ogre/BuildModule.py -r --build-deb gccxml || exit
sudo true
python python-ogre/BuildModule.py --install-deb gccxml || exit
# The pygccxml is dependent on the gccxml version we just built
python python-ogre/BuildModule.py -r --build-deb pygccxml  || exit
sudo true
python python-ogre/BuildModule.py --install-deb pygccxml  || exit

#python python-ogre/BuildModule.py -r --build-deb boost || exit

# Build our custom extension to boost-indexing which pyplusplus needs
python python-ogre/BuildModule.py -r --build-deb pyplusplus || exit
sudo true
python python-ogre/BuildModule.py --install-deb pyplusplus || exit

# Generate the python-ogre package
cd python-ogre
dpkg-checkbuilddeps || exit
fakeroot ./debian/rules binary
