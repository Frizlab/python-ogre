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
	python-ogre/boost								\
	python-ogre/debs								\
	python-ogre/debian								\
	python-ogre/demos								\
	python-ogre/docs								\
	python-ogre/packages_2.6						\
	python-ogre/ogre_cvs.sh							\
	python-ogre/scripts								\
	python-ogre/SourceSnapShots						\
	python-ogre/ThirdParty/extra					\
	python-ogre/ThirdParty/dshow					\
	python-ogre/ThirdParty/History					\
	python-ogre/ThirdParty/vcredist_x86.exe			\
	python-ogre/convertonew.sed						\
	python-ogre/convert.py							\
	python-ogre/convert.sed							\
	python-ogre/test.nsi							\
	python-ogre/postinstall.rtf						\
	python-ogre/installWarning.rtf					\
	python-ogre/makehelp.bat						\
	python-ogre/config.ini							\
	python-ogre/PythonOgreInstallCreator.iss		\


find python-ogre -name .svn | xargs rm -rf

tar -czvf python-ogre.tar.gz python-ogre
md5sum python-ogre.tar.gz
du -k python-ogre.tar.gz
du -h python-ogre.tar.gz

cp python-ogre-pristine/debian/rules ./debian.rules
cp python-ogre-pristine/debian/control ./debian.control
cp python-ogre-pristine/debian/changelog ./debian.changelog
cp python-ogre-pristine/debian/dsc ./python-ogre.dsc
