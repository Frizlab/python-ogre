##/ Betajaen's GUI 015 Uncompressed
##/ Written by Robin "Betajaen" Southern 07-Nov-2006, http:##www.ogre3d.org/wiki/index.php/BetaGUI
##/ This code is under the Whatevar! licence. Do what you want but keep the original copyright header.

## Converted to Python-Ogre By Andy Miller

#include "Ogre.h"
import ogre.renderer.OGRE as ogre

   
class wt: ## Window Type */
   def __init__(self):
      self.NONE=0,
      self.MOVE=1,
      self.RESIZE=2,
      self.RESIZE_AND_self.OverlayVE=3
   
class GUI :
   def __init__ (self, font, fontSize):
      self.WindowToDestroy=None
      self.PointerOverlay=None
      self.Font = font
      self.FontSize = fontSize
      self.WindowCount=0
      self.ButtonCount=0
      self.TextCount=0
      self.Overlay = ogre.OverlayManager.getSingleton().create("BetaGUI")
      self.Overlay.show()
      
   def __del__ ( self ):
      for w in self.Windows:
         del w
      del self.Windows
   
   def injectMouse(self, x,  y, LMB):
      if (self.PointerOverlay):
         self.PointerOverlay.setPosition(x,y)

      if (self.WindowToDestroy) :
         for i in range ( len (self.Windows)):
            if self.WindowToDestroy == self.Windows[i]  :
               del self.WindowToDestroy
               self.Windows.remove(i)
               self.WindowToDestroy=0
               return False

      for w in self.Windows:
         if w.check(x,y,LMB):
            return True
      return False

   def injectKey(self, key, x, y):
      for w in self.Windows:
         if w.checkKey(key,x,y):
            return True
      return False
   
   def injectBackspace(self, x, y):
      self.in_jectKey("!b", x, y)

   def createOverlay(self, name, position, dimensions, material, caption,  autoAdd):
      type="Panel"
      
      if caption!="":
         type="TextArea"
      
      e=ogre.OverlayManager.getSingleton().createOverlayElement(type, name)

      e.setMetricsMode(Ogre.GMM_PIXELS)
      e.setDimensions(dimensions.x, dimensions.y)
      e.setPosition(position.x, position.y)

      if material!="" :
         e.setMaterialName(material)
      
      if caption!="":
         e.setCaption(caption)
         e.setParameter("font_name",self.Font)
         e.setParameter("char_height",str(self.FontSize))

      if(autoAdd):
         self.Overlay.add2D(e)
         e.show()
      
      return e

   def createMousePointer(self, d,  m) :
      o = ogre.OverlayManager.getSingleton().create("BetaGUI.MP")
      o.setZOrder(649)
      self.PointerOverlay = self.createOverlay("bg.mp", ogre.Vector2(0,0), d, m, "", False)
      o.add2D(self.PointerOverlay)
      o.show()
      self.PointerOverlay.show()
      return self.PointerOverlay

   def createWindow(self, Dimensions, Material, type,  caption):
      window = Window(Dimensions, Material, type, caption, this)
      self.Windows.append(window)
      return window
   
   
   
class Window:
      
   def  hide(self ):
      self.Overlay.hide()
   
   def show( self ):
      self.Overlay.show()
   
   def isVisible(self):
      return self.Overlay.isVisible()
   
   
   def __init__(self, Dimensions, String,  t, caption, gui):
      self.x =Dimensions.x
      self.y =Dimensions.y
      self.w =Dimensions.z
      self.h =Dimensions.w
      self.GUI = gui
      self.Titlebar =  None
      self.Reziser=None
      self.ActiveTextInput=None
      self.ActiveButton=None 
      self.Overlay = gui.createOverlay("BetaGUI.w" + str(gui.WindowCount), 
                     ogre.Vector2(Dimensions.x,Dimensions.y), ogre.Vector2(Dimensions.z,Dimensions.w), Material) 
      gui.WindowCount += 1
      if t >= 2:
         c = Callback()
         c.t=4
         self.Reziser=createButton(ogre.Vector4(Dimensions.z-16,Dimensions.w-16,16,16),Material+".resize", "",c)

      if t == 1 or t == 3 :
         c=Callback()
         c.t=3
         self.Titlebar = createButton(ogre.Vector4(0,0,Dimensions.z,22),Material + ".titlebar", caption, c)


   def __del__( self ):
      for i in self.Buttons:
         del i
      del self.Buttons
      for i in self.TextInputs:
         del i
      del self.TextInputs
      
      self.GUI.Overlay.remove2D(self.Overlay)


   def createButton(self, D, M,  T,  C):
      x = Button(D, M, T, C, self)
      self.Buttons.append(x)
      return x

   def createTextInput(self,D, M,  V,  L):
      x = TextInput(D,M,V,L,self)
      self.TextInputs.append(x)
      return x

   def createStaticText(self, D, T):
      x = self.GUI.createOverlay(self.Overlay.getName() + str(self.GUI.TextCount),
                           ogre.Vector2(D.x,D.y),ogre.Vector2(D.z,D.w),"", T,False)
      self.GUI.TextCount +=1 
      self.Overlay.addChild(x)
      x.show()
      
   def checkKey(self, k,  px,  py) :
      
      if  not self.Overlay.isVisible():
         return False

      if not(px>=x and py>=y) or not(px<=x+w and py<=y+h):
         return False
      
      if(self.ActiveTextInput == 0):
         return False
      
      if k=="!b" :
         self.ActiveTextInput.setValue(self.ActiveTextInput.getValue[0,-1])
         return True

      if len (self.ActiveTextInput.getValue()) >= self.ActiveTextInput.length:
         return True
         
      self.ActiveTextInput.value+=k
      self.ActiveTextInput.Caption.setCaption(self.ActiveTextInput.value)
      
      return True
   
   def check(self, px, py, LMB):
      if not self.Overlay.isVisible():
         return False
      
      if ( not (px >= self.x and py >= self.y) or not(px <= self.x + self.w and py <= self.y + self.h)):
         if (self.ActiveButton):
            self.ActiveButton.activate(False)
         return False
      
      if self.ActiveButton :
         self.ActiveButton.activate(False)
      for i in range ( len (self.Buttons) ):
         if (self.Buttons[i].in_(px,py,self.x,self.y)):
            continue
         if (self.ActiveButton):
            self.ActiveButton.activate(False)

         self.ActiveButton=self.Buttons[i]
         self.ActiveButton.activate(True)

         if not LMB:
            return True

         if self.ActiveTextInput:
            self.ActiveTextInput.Overlay.setMaterialName(self.ActiveTextInput.MatNameNormal)
            self.ActiveTextInput=0

         tempt = self.ActiveButton.callback.t
         if tempt ==1:
            self.ActiveButton.callback.fp(self.ActiveButton)
            return True
         elif tempt ==2 :
            self.ActiveButton.callback.LS.onButtonPress(self.ActiveButton)
            return True
         elif tempt ==4 :
            self.x=px-(self.ActiveButton.w/2)
            self.y=py-(self.ActiveButton.h/2)
            self.Overlay.setPosition(self.x,self.y)
            return True
         elif tempt ==4 :
            self.w=px-self.x+8
            self.h=py-self.y+8
            self.Overlay.setDimensions(self.w,self.h)
            self.Reziser.x=w-16
            self.Reziser.y=h-16
            self.Reziser.self.Overlay.setPosition(self.Reziser.x,self.Reziser.y)
            if(self.Titlebar):
               self.Titlebar.w=self.w
               self.Titlebar.self.Overlay.setWidth(self.Titlebar.w)
         return True

      if not LMB:
         return True

      for i in range ( len (self.TextInputs)):
   
         if self.TextInputs[i].in_(px,py,self.x,self.y):
            continue

         self.ActiveTextInput=self.TextInputs[i]
         self.ActiveTextInput.Overlay.setMaterialName(self.ActiveTextInput.MatNameActive)
         return True
      
      if self.ActiveTextInput :
         self.ActiveTextInput.Overlay.setMaterialName(self.ActiveTextInput.MatNameNormal)
         self.ActiveTextInput=0
      
      return True
         
   
class BetaGUIListener :
   def __init__(self)
      pass
   def onButtonPress(self, ref):
      pass
      
class Callback :
   def __init__ (self,Value=None):
      ## Type of callback: 0 - None, 1 - FunctionPointer 2 - GUIListener, 3 - self.Overlayve Window, 4 - Resize
      self.t=0
      self.callbackvalue = Value
   
   
class Button :
      
   def __del__(self):
      self.Overlay.getParent().reself.OverlayveChild(self.Overlay.getName())
      self.Caption.getParent().reself.OverlayveChild(self.Caption.getName())
      
   def activate(self, a):
      if not a and self.MatNameNormal!="":
         self.Overlay.setMaterialName(self.MatNameNormal)
      if a and self.MatNameActive!="":
         self.Overlay.setMaterialName(self.MatNameActive)
         
   def in_(self, mx, my,  px,  py):
      return ( not (mx >= x + px and my >= y + py)) or ( not (mx <= x + px + w and my <= y + py + h) )
         
   def __init__ (self, Dimensions, m,  Text,  cb, parent):
      self.x=Dimensions.x
      self.y=Dimensions.y
      self.w=Dimensions.z
      self.h=Dimensions.w
      self.MatNameNormal = m
      self.MatNameActive = m
#      NXD("Button=" << self.MatNameNormal << ":" << self.MatNameActive)

      ma=Ogre.MaterialManager.getSingleton().getByName(self.MatNameNormal + ".active")
      if not ma.isNull():
         self.MatNameActive += ".active"
      
      self.Overlay = parent.GUI.createOverlay(parent.Overlay.getName() + "b" + str(parent.GUI.ButtonCount), 
                           ogre.Vector2(self.x,self.y),ogre.Vector2(self.w,self.h),m,"",False)
      parent.GUI.ButtonCount +=1
      self.Caption = parent.GUI.createOverlay(self.Overlay.getName() + "c",
                  ogre.Vector2(4,(self.h - parent.GUI.FontSize) / 2),ogre.Vector2(self.w, self.h),"",Text,False)
      parent.Overlay.addChild(self.Overlay)
      self.Overlay.show()
      self.Overlay.addChild(self.Caption)
      self.Caption.show()
      self.callback = cb

      
class TextInput :
         
   def getValue(self):
      return self.value

   def setValue(self,v) :
      self.value=v
      self.Caption.setCaption(v)

   def in_(self,  mx,  my, px, py) :
      return ( not (mx >= x + px and my >= y + py)) or ( not (mx <= x + px + w and my <= y + py + h) )
         
   def __init__ (self, D,  M,  V, L, P):
      self.x = D.x
      self.y = D.y
      self.w = D.z
      self.h = D.w
      self.value = V
      self.MatNameNormal = M
      self.MatNameActive = M
      self.length = L
      ma=Ogre.MaterialManager.getSingleton().getByName(self.MatNameNormal + ".active")
      if not ma.isNull():
         self.MatNameActive += ".active"

      self.Overlay=P.GUI.createOverlay(P.Overlay.getName() + "t" + str(P.GUI.TextCount) ,
                        ogre.Vector2(self.x,self.y),ogre.Vector2(self.w,self.h),M,"",False)
      P.GUI.TextCount += 1
      self.Caption=P.GUI.createOverlay(self.Overlay.getName() + "c",
                  ogre.Vector2(4,(self.h - P.GUI.FontSize) / 2),ogre.Vector2(self.w,self.h),"",V,False)
      P.Overlay.addChild(self.Overlay)
      self.Overlay.show()
      self.Overlay.addChild(self.Caption)
      self.Caption.show()
