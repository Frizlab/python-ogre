#! /bin/sh

if [ ! -e python-ogre-pristine ]; then
  echo "I need a clean checkout of python-ogre in python-ogre-pristine"
  exit 1
fi

rm -rvf python-ogre

cd python-ogre-pristine
svn update
cd ..
cp -rvf python-ogre-pristine/ python-ogre/

rm -rvf \
	python-ogre/boost 		 						\
	python-ogre/debs 		 						\
	python-ogre/debian 		 						\
	python-ogre/demos 								\
	python-ogre/docs 								\
	python-ogre/packages_2.6 						\
	python-ogre/patch 								\
	python-ogre/ogre_cvs.sh 						\
	python-ogre/scripts 							\
	python-ogre/SourceSnapShots 					\
	python-ogre/ThirdParty/extra 					\
	python-ogre/ThirdParty/dshow 					\
	python-ogre/ThirdParty/History 					\
	python-ogre/ThirdParty/vcredist_x86.exe 		\


find python-ogre -name .svn | xargs rm -rf

touch python-ogre/STABLE

tar -czvf python-ogre.tar.gz python-ogre
md5sum python-ogre.tar.gz
du -k python-ogre.tar.gz
du -h python-ogre.tar.gz

cp python-ogre.tar.gz ..
cp python-ogre-pristine/debian/rules ../debian.rules
cp python-ogre-pristine/debian/control ../debian.control
cp python-ogre-pristine/debian/changelog ../debian.changelog
