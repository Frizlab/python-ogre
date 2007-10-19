#!/bin/bash
# Part of the Python-Ogre installation 

# master config setup
source ./config.sh
if [ $1 != "" ]
then 
    echo "=== generating $1 ===" 
    echo "=== generating $1 ===" >> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh $1
else

    echo ' -- creating OGRE code'
    echo ' -- creating OGRE code' >> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh ogre
    
    echo ' -- creating OGRErefapp code'
    echo ' -- creating OGRErefapp code'>> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh ogrerefapp 
     
    echo ' -- creating quickgui code'
    echo ' -- creating quickgui code'>> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh quickgui 

    echo ' -- creating OIS code'
    echo ' -- creating OIS code'>> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh ois 
     
    echo ' -- creating CEGUI code'
    echo ' -- creating CEGUI code'>> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh cegui 

    echo ' -- creating OgreAL code'
    echo ' -- creating OgreAL code'>> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh ogreal 
     
    echo ' -- creating OGREode code'
    echo ' -- creating OGREode code'>> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh ogreode 
     
    echo ' -- creating PLIB code'
    echo ' -- creating PLIB code'>> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh plib 
     
    #echo ' -- creating Theora code'
    #echo ' -- creating Theora code'>> $INSTALL_DIR/log.out
    #$INSTALL_DIR/gen_code.sh theora 

    echo ' -- creating OGREnewt code'
    echo ' -- creating OGREnewt code' >> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh ogrenewt 
     
    echo ' -- creating ode code'
    echo ' -- creating ode code' >> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh ode 

    echo ' -- creating et code'
    echo ' -- creating et code' >> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh et 
     
    echo ' -- creating caelum code'
    echo ' -- creating caelum code' >> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh caelum 
 
    echo ' -- creating forests code'
    echo ' -- creating forests code' >> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh forests 

    echo ' -- creating bullet code '
    echo ' -- creating bullet code' >> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh bullet 

    echo ' -- creating ogrebulletc code '
    echo ' -- creating ogrebulletc code' >> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh ogrebulletc 

    echo ' -- creating ogrebulletd code '
    echo ' -- creating ogrebulletd code' >> $INSTALL_DIR/log.out
    $INSTALL_DIR/gen_code.sh ogrebulletd 

#    echo ' -- creating nxogre code '
#    echo ' -- creating nxogre code' >> $INSTALL_DIR/log.out
#    $INSTALL_DIR/gen_code.sh nxogre     
        
fi
cd $INSTALL_DIR
echo
echo "=== done building code ==="
echo "=== done building code ===">> $INSTALL_DIR/log.out

