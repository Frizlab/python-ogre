#! /bin/sh

if [ ! -e python-ogre-pristine ]; then
  echo "I need a clean checkout of python-ogre in python-ogre-pristine"
  exit 1
fi

rm -rvf python-ogre

CHANGELOG_DATE=`date +'%a, %d %b %Y %H:%m:%S %z'`
cd python-ogre-pristine
svn update
CHANGELOG_VERSION=`svnversion`
cd ..

##########################################################################
# Create the python-ogre package
##########################################################################

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
	python-ogre/ThirdParty/dxwebsetup.exe			\
	python-ogre/convertonew.sed						\
	python-ogre/convert.py							\
	python-ogre/convert.sed							\
	python-ogre/test.nsi							\
	python-ogre/postinstall.rtf						\
	python-ogre/installWarning.rtf					\
	python-ogre/makehelp.bat						\
	python-ogre/config.ini							\
	python-ogre/PythonOgreInstallCreator.iss

find python-ogre -name .svn | xargs rm -rf

tar -czvf python-ogre.tar.gz python-ogre
md5sum python-ogre.tar.gz
du -k python-ogre.tar.gz
du -h python-ogre.tar.gz

cp python-ogre-pristine/debian/rules ./debian.rules
cp python-ogre-pristine/debian/control ./debian.control
cp python-ogre-pristine/debian/dsc ./python-ogre.dsc

# Update the changelog with the subversion information.
cat > ./debian.changelog <<EOF
python-ogre (1.6.1+svn$CHANGELOG_VERSION-0) unstable; urgency=low

  * Updating for Subversion revision $CHANGELOG_VERSION.

 -- Tim 'Mithro' Ansell <mithro@mithis.com>  $CHANGELOG_DATE

EOF
cat python-ogre-pristine/debian/changelog >> ./debian.changelog

##########################################################################
# Create the python-ogre-demos package package
##########################################################################
rm -rf python-ogre-demos
mkdir python-ogre-demos

cp -rvf python-ogre-pristine/demos/* python-ogre-demos/
# Clean up any svn directories
find python-ogre-demos -name .svn | xargs rm -rf
# Remove any plugins.cfg files
find python-ogre-demos -name plugins.cfg | xargs rm
# Remove any yet to work demos
find python-ogre-demos -name tofix | xargs rm -rf
# Remove windows only demos
find python-ogre-demos -name dshow | xargs rm -rf
# Remove any log files
find python-ogre-demos -name \*.log | xargs rm -rf

cd python-ogre-demos
python ../python-ogre-pristine/scripts/python-cleanup.py
find -name \*.py[co] | xargs rm

# Create the ogre.cfg file
cat > ogre.cfg <<EOF
Render System=OpenGL Rendering Subsystem

[OpenGL Rendering Subsystem]
Display Frequency=50 MHz
FSAA=0
Full Screen=No
RTT Preferred Mode=FBO
VSync=No
Video Mode= 640 x  480
sRGB Gamma Conversion=No
EOF

# Put an ogre.cfg in each subdirectory too
for i in *; do
  if [ -d $i ]; then
    cd $i
    ln -sf ../ogre.cfg ogre.cfg
    cd ..
  fi
done

# Setup the symlink for plugins.cfg
ln -s /etc/OGRE/plugins.cfg plugins.cfg

# Setup the UnitTest scripts
cp ../python-ogre-pristine/scripts/UnitTest.py UnitTest.py
cp ../python-ogre-pristine/scripts/restore_repeat.py restore_repeat.py
echo "#! /bin/sh" > UnitTest.sh
echo "mkdir /tmp/python-ogre-demos" > UnitTest.sh
echo "rm -rf /tmp/python-ogre-demos"
echo "python UnitTest.py -p "." -s /tmp/python-ogre-demos -l /tmp/python-ogre-demos.log \$@" >> UnitTest.sh
chmod a+x UnitTest.sh

cd ..

mkdir -p ../python-ogre-demos
tar -czvf ../python-ogre-demos/python-ogre-demos.tar.gz python-ogre-demos
cp python-ogre-pristine/debs/python-ogre-demos/rules ../python-ogre-demos/debian.rules
cp python-ogre-pristine/debs/python-ogre-demos/control ../python-ogre-demos/debian.control
cp python-ogre-pristine/debs/python-ogre-demos/dsc ../python-ogre-demos/python-ogre-demos.dsc

# Update the changelog with the subversion information.
cat > ../python-ogre-demos/debian.changelog <<EOF
python-ogre-demos (1.6.1+svn$CHANGELOG_VERSION-0) unstable; urgency=low

  * Updating for Subversion revision $CHANGELOG_VERSION.

 -- Tim 'Mithro' Ansell <mithro@mithis.com>  $CHANGELOG_DATE

EOF
cat python-ogre-pristine/debs/python-ogre-demos/changelog >> ../python-ogre-demos/debian.changelog
