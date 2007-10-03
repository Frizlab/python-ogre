# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http:##python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http:##www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ctypes



def WriteToTexture(in_string, destTexture, destRectangle, font, color, justify = 'l',  wordwrap = True):
    if destTexture.getHeight() < destRectangle.bottom:
        destRectangle.bottom = destTexture.getHeight()
    if destTexture.getWidth() < destRectangle.right:
        destRectangle.right = destTexture.getWidth()

    if not font.isLoaded():
        font.load()

    fontTexture = ogre.TextureManager.getSingleton().getByName\
                (font.getMaterial().getTechnique(0).getPass(0).getTextureUnitState(0).getTextureName())
    
    fontBuffer = fontTexture.getBuffer()
    destBuffer = destTexture.getBuffer()

    destPb = destBuffer.lock(destRectangle,ogre.HardwareBuffer.HBL_NORMAL)
    
    ## The font texture buffer was created write only...so we cannot read it back :o). 
    ## One solution is to copy the buffer  instead of locking it. 
    ## (Maybe there is a way to create a font texture which is not write_only ?)
    
    ## create a buffer
    nBuffSize = fontBuffer.getSizeInBytes()
    ## Here we show the difference in C++ and Python
    ## uint8* buff = (uint8*)calloc(nBuffSize, sizeof(uint8)) 
    storageclass = ctypes.c_uint8 * (nBuffSize)    # allocate a buffer class
    buff=storageclass()
    
    ## the PixelBox call needs a pointer to the buffer and here's how we do this in Python-Ogre
    VoidPointer = ogre.CastVoidPtr(ctypes.addressof(buff))
    
    ## create pixel box using the copy of the buffer
    fontPb = ogre.PixelBox(fontBuffer.getWidth(), fontBuffer.getHeight(),fontBuffer.getDepth(), 
                                            fontBuffer.getFormat(), VoidPointer)          
    fontBuffer.blitToMemory(fontPb)

#   fontData = static_cast<uint8*>( fontPb.data )
#   destData = static_cast<uint8*>( destPb.data )
    
    fontData = buff
    
    # we nede to know the size of the dest buffer so hopefully this is right
    destSize = destPb.getConsecutiveSize()
    destData = (ctypes.c_uint8 * (destSize)).from_address( ogre.CastInt(destPb.getData()) )

    fontPixelSize = ogre.PixelUtil.getNumElemBytes(fontPb.format)
    destPixelSize = ogre.PixelUtil.getNumElemBytes(destPb.format)

    fontRowPitchBytes = fontPb.rowPitch * fontPixelSize
    destRowPitchBytes = destPb.rowPitch * destPixelSize

    GlyphTexCoords =[]
    for i in range (len (in_string)):
        GlyphTexCoords.append(ogre.Box())   # need a fixed size array in Python

    charheight = 0
    charwidth = 0

    for i in range ( len(in_string) ):
        if ((in_string[i] != '\t') and (in_string[i] != '\n') and (in_string[i] != ' ')):
            glypheTexRect = font.getGlyphTexCoords(ord(in_string[i]) )
            GlyphTexCoords[i].left = int(glypheTexRect.left * fontTexture.getSrcWidth())
            GlyphTexCoords[i].top = int(glypheTexRect.top * fontTexture.getSrcHeight())
            GlyphTexCoords[i].right = int(glypheTexRect.right * fontTexture.getSrcWidth())
            GlyphTexCoords[i].bottom = int(glypheTexRect.bottom * fontTexture.getSrcHeight())

            if (GlyphTexCoords[i].getHeight() > charheight):
                charheight = GlyphTexCoords[i].getHeight()
            if (GlyphTexCoords[i].getWidth() > charwidth):
                charwidth = GlyphTexCoords[i].getWidth()

    cursorX = 0
    cursorY = 0
    lineend = destRectangle.getWidth()
    carriagreturn = True
    for strindex in range (len(in_string) ):
        tchar = in_string[strindex]
        if tchar == ' ': cursorX += charwidth
        elif tchar == '\t':cursorX += charwidth * 3
        elif tchar == '\n':
            cursorY += charheight
            carriagreturn = True
        else:
            ##wrapping
            if (cursorX + GlyphTexCoords[strindex].getWidth()> lineend) and not carriagreturn:
                cursorY += charheight
                carriagreturn = True
            
            ##justify
            if (carriagreturn):
                l = strindex
                textwidth = 0   
                wordwidth = 0
                while (l < len(in_string) ) and (in_string[l] != '\n)'):
                    wordwidth = 0
                    if in_string[l] == ' ': 
                        wordwidth = charwidth
                        l+=1
                    elif in_string[l] == '\t':
                        wordwidth = charwidth *3
                        l+=1
                    elif in_string[l] == '\n': l = len(in_string)
                    
                    if wordwrap:
                        while((l < len(in_string)) and (in_string[l] != ' ') and (in_string[l] != '\t') and (in_string[l] != '\n')):
                            wordwidth += GlyphTexCoords[l].getWidth()
                            l+=1
                    else:
                            wordwidth += GlyphTexCoords[l].getWidth()
                            l+=1

                    if ((textwidth + wordwidth) <= destRectangle.getWidth()):
                        textwidth += (wordwidth)
                    else:
                        break
            
                if ((textwidth == 0) and (wordwidth > destRectangle.getWidth())):
                    textwidth = destRectangle.getWidth()

                if justify == 'c':   
                    cursorX = (destRectangle.getWidth() - textwidth)/2
                    lineend = destRectangle.getWidth() - cursorX
                
                elif justify == 'r': 
                    cursorX = (destRectangle.getWidth() - textwidth)
                    lineend = destRectangle.getWidth()
            
                else:
                    cursorX = 0
                    lineend = textwidth
            
                carriagreturn = False
            
            ##abort - net enough space to draw
            if ((cursorY + charheight) > destRectangle.getHeight()):
                ## need to bail here
                print "ERROR"

            ##draw pixel by pixel
            for i in range ( GlyphTexCoords[strindex].getHeight() ):
                for j in range  (GlyphTexCoords[strindex].getWidth() ):
                
                        index =(i + GlyphTexCoords[strindex].top) * fontRowPitchBytes + (j + GlyphTexCoords[strindex].left) * fontPixelSize +1
                        fd = fontData[ index ]
                        alpha =  color.a * (fd / 255.0)
                        invalpha = 1.0 - alpha
                        offset = (i + cursorY) * destRowPitchBytes + (j + cursorX) * destPixelSize
                        pix = ogre.ColourValue()
                        address = ctypes.addressof(destData) + offset
                        ogre.PixelUtil.unpackColour(pix,destPb.format,address)
                        pix = (pix * invalpha) + (color * alpha)
                        ogre.PixelUtil.packColour(pix,destPb.format,address)

            cursorX += GlyphTexCoords[strindex].getWidth()

    destBuffer.unlock()


class TestApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        
        background = ogre.TextureManager.getSingleton().getByName("ogretext.png")

        font = ogre.FontManager.getSingleton().getByName("BlueHighway")
                
        ## Make sure the texture is not WRITE_ONLY, we need to read the buffer to do the blending with the font (get the alpha for example)
        texture = ogre.TextureManager.getSingleton().createManual("Write Texture","General",
                                            ogre.TEX_TYPE_2D, 512, 512, ogre.MIP_UNLIMITED , 
                                            ogre.PF_X8R8G8B8, ogre.TU_STATIC|ogre.TU_AUTOMIPMAP)
        
        ## Draw the background to the new texture
        texture.getBuffer().blit(background.getBuffer())
        
        WriteToTexture("Andy was here!",texture,ogre.Box(25,275,370,500),font,ogre.ColourValue(1.0,1.0,1.0,1.0),'c')
       
        c = sceneManager.createEntity("cubo", "cube.mesh")
        n = sceneManager.getRootSceneNode().createChildSceneNode()
        n.attachObject(c)
        n.setPosition(ogre.Vector3(0, 0, 350)) 
        
        c.setMaterialName('Examples/Rockwall')
        
        mat=ogre.MaterialManager.getSingleton().getByName('Examples/Rockwall')
        tex=mat.getTechnique(0).getPass(0).getTextureUnitState(0)
        tex.setTextureName("Write Texture")
            
                
if __name__ == '__main__':
    try:
        application = TestApplication()
        application.go()
    except ogre.OgreException, e:
        print e
