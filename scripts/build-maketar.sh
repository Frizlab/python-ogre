#! /bin/sh

if [ ! -e python-ogre-pristine ]; then
  echo "I need a clean checkout of python-ogre in python-ogre-pristine"
  exit 1
fi

CHANGELOG_DATE=`date +'%a, %d %b %Y %H:%m:%S %z'`

rm -rvf python-ogre

cd python-ogre-pristine
svn update

CHANGELOG_VERSION=`svnversion`

cd ..
cp -rvf python-ogre-pristine/ python-ogre/

rm -rvf \
	python-ogre/boost								\
	python-ogre/debs								\
	python-ogre/debian								\
	python-ogre/demos								\
	python-ogre/docs								\
	python-ogre/ogre_cvs.sh							\
	python-ogre/scripts								\
	python-ogre/SourceSnapShots						\
	python-ogre/ThirdParty/extra					\
	python-ogre/ThirdParty/dshow					\
	python-ogre/ThirdParty/History					\
	python-ogre/ThirdParty/vcredist_x86.exe			\
	python-ogre/ThirdParty/dxwebsetup.exe \
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
cp python-ogre-pristine/debian/dsc ./python-ogre.dsc

# Update the changelog with the subversion information.
echo "Updating the changelog to $CHANGELOG_VERSION"
cat > ./debian.changelog <<EOF
python-ogre (1.6.0+$CHANGELOG_VERSION-0) unstable; urgency=low

  * Updating for Subversion revision $CHANGELOG_VERSION.

 -- Tim 'Mithro' Ansell <mithro@mithis.com>  $CHANGELOG_DATE

EOF
cat python-ogre-pristine/debian/changelog >> ./debian.changelog

