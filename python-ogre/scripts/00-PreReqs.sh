#!/bin/bash
# Part of the Python-Ogre installation 
#		python-celementtree 	\
#		python2.6-clementtree \
if [ -x /usr/bin/apt-get ]; then
	sudo apt-get install 		\
		g++ 					\
		libfreetype6-dev 		\
		libpcre3-dev 			\
		autoconf 				\
		automake1.9 			\
		libgl1-mesa-dev 		\
		libx11-dev 				\
		libglut3-dev 			\
		libtool 				\
		libxaw7-dev 			\
		libxxf86vm-dev 			\
		patch 					\
		libxrandr-dev 			\
		cvs 					\
		subversion 				\
		libxt-dev 				\
		libexpat1-dev 			\
		python 					\
		python-dev 				\
		libglew-dev 			\
		jam                     \
	cmake libboost-thread-dev libboost-date-time-dev scons \
	libfreeimage-dev libfreeimage3 libzzip-dev bjam \

fi

# 
if [ -x /usr/bin/zypper ]; then
	sudo zypper install 		\
		gcc-c++ 				\
		freetype2-devel 		\
		pcre-devel 				\
		autoconf 				\
		automake 				\
		Mesa-devel 				\
		xorg-x11-libX11-devel 	\
		freeglut-devel 			\
		libtool 				\
		xaw3d-devel 			\
		patch 					\
		cvs 					\
		subversion 				\
		xorg-x11-libXt-devel	\
		libexpat1	 			\
		python 					\
		python-devel			\
		glew-devel	 			\
		jam                     \

fi
