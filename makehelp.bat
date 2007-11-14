rem generate help files -- note it's very crude and based upon my system .....
rem we have to specify the modules individually otherwise mshlp compiler crashes..
del /q c:\temp\doc\html\*
python \python25\scripts\epydoc.py --docformat=javadoc -o c:\temp\doc\html ogre.renderer.OGRE ogre.io.OIS ogre.gui.CEGUI ogre.physics ogre.sound
python \temp\chm1\gen_epydoc.py --compile c:\temp\doc\html