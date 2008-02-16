##/ Betajaen's GUI 015 Uncompressed
##/ Written by Robin "Betajaen" Southern 07-Nov-2006, http:##www.ogre3d.org/wiki/index.php/BetaGUI
##/ This code is under the Whatevar! licence. Do what you want but keep the original copyright header.

## Converted to Python-Ogre By Andy Miller

#include "Ogre.h"
import ogre.renderer.OGRE as ogre

from Enums import *

wt = Enum ('NONE','MOVE','RESIZE','RESIZE_AND_MOVE')
WindowFeatureType = Enum ('WFT_NONE','WFT_MOVE','WFT_RESIZE','WFT_RESIZE_AND_MOVE')
WindowPlacementType = Enum ( 'WPT_NONE','WPT_VERTICAL', 'WPT_HORIZONTAL' )
WindowActivity = Enum ( 'WA_NONE', 'WA_MOVE', 'WA_RESIZE', 'WA_PRESS' )

class GUI :
    def __init__ (self, baseOverlay,font, fontSize,win):
      self.WindowToDestroy=None
      self.PointerOverlay=None
      self.Font = font
      self.FontSize = fontSize
      self.WindowCount=0
      self.ButtonCount=0
      self.TextCount=0
      self.Overlay = ogre.OverlayManager.getSingleton().create("BetaGUI")
      self.Overlay.show()
      self.Windows=[]
      self.mRenderWindow=win
      self.mName = baseOverlay
      self.mDragWindowAction = WindowActivity.WA_NONE
      self.mTextCaptionCount = 0
      self.mTextCaptionCountArea = 0
      self.mButtonCountTextArea = 0
      self.mNextMenuWindowPos=[0,0]
      self.mMenuWindow=0
      self.mActiveTextInput=0
      self.mActiveButton=0
      self.mUpTime=0
      self.mFarZ=100
      self.mNearZ=101
      self.mActiveEffects=[]
      
    def __del__ ( self ):
        for w in self.Windows:
            del w
        for e in self.mActiveEffects:
            del e
    
    def injectMouse(self, x,  y, LMB):
#       if (self.PointerOverlay):
        self.PointerOverlay.setPosition(x,y)
        if LMB:
            if self.mActiveTextInput:
                self.mActiveTextInput.mMainOverlay.setMaterialName(self.mActiveTextInput.mMaterialName)
                self.mActiveTextInput = None
            
            if self.mActiveButton and self.mActiveDraggedWindow:
                 if self.mActiveDraggedWindow.mDragType == WA_MOVE:
                       self.mDragWindowAction = WA_MOVE; 
                       self.mActiveDraggedWindow.setPosition (
                           self.mDragWindowX + (x - self.mDragX),
                           self.mDragWindowY + (y - self.mDragY)
                           )
                       return True;
                 elif (self.mActiveDraggedWindow.mDragType == WA_RESIZE): 
                     self.mDragWindowAction = WA_RESIZE; 
                     self.mActiveDraggedWindow.setSize (
                         mDragWindowX + (x - self.mDragX),
                         mDragWindowY + (y - self.mDragY)
                         )
                     return True
        elif (self.mDragWindowAction != WA_NONE):
            self.mDragWindowAction = WA_NONE
            self.mActiveDraggedWindow = None 
            self.sortZOrder ()

        assert (mDragWindowAction == WA_NONE) ;  
        for(std::vector<Window*>::iterator i=mWindows.begin();i!=mWindows.end();++i) 
        {
		    if((*i)->isVisible() && 
                (*i)->check(x, y, LMB)) 
            {
                mActiveDraggedWindow = (*i);
                sortZOrder ();

                //if (mMousePointerOverlay)
                //   mMousePointerOverlay->show ();
			    return true;
		    }
	    }      

		return false;
    }	


    def injectKey(self, key, x, y):
      for w in self.Windows:
         if w.checkKey(key,x,y):
            return True
      return False
    
    def injectBackspace(self, x, y):
      self.in_jectKey("!b", x, y)

    def createOverlay(self, name, position, dimensions, material="", caption="",  autoAdd=True):
        type="Panel"
        
        if caption!="":
            type="TextArea"
        
        e=ogre.OverlayManager.getSingleton().createOverlayElement(type, name, False)
        
        e.setMetricsMode(ogre.GMM_PIXELS)
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

    def createWindow(self, Dimensions, Material, type_, caption=""):
        window = Window(Dimensions, Material, type_, caption, self)
        self.Windows.append(window)
        return window
      
    def destroyWindow ( self, win ):
        self.mNextWindowToDestroy = win
    
    def getUpTime(self):
        return self.mUpTime
        
    def setZRangeOrder(self,  nearZ, FarZ): 
        assert (self.mNearZ > self.mFarZ)
        self.mNearZ = nearZ
        self.mFarZ = FarZ
        self.sortZOrder()
    ##------------------------------------------------------------------------------------------------
    def sortZOrder():
        assert (self.mNearZ > self.mFarZ)
        farZ = self.mFarZ
        for w in self.mWindows:
            w.setZOrder(farZ)
            if farZ + 1 < self.mNearZ:
                farZ+=1
        if (self.mActiveDraggedWindow):
            self.mActiveDraggedWindow.setZOrder(self.mNearZ)
    ##------------------------------------------------------------------------------------------------
    def update (self, timeElapsed):
        self.mUpTime += timeElapsed;
        for itWindow in self.mWindows:
            itWindow.setUnderEffect(False)
        for x in range (len (self.mActiveEffects )):
            itEffect = self.mActiveEffects[x]       
            if itEffect.update(timeElapsed):
                self.mActiveEffects.remove(x)

        if self.mNextWindowToDestroy: 
            for x in range ( len ( self.mWindows )):
                w = self.mWindows[x]
                if self.mNextWindowToDestroy==w: 
                    self.mWindows.remove(x)
                    self.mNextWindowToDestroy=None
    ##------------------------------------------------------------------------------------------------
    def addEffect (self, e):
        self.mActiveEffects.append(e)
        
    
class Window:
      
    def hide(self ):
      self.Overlay.hide()
    
    def show( self ):
      self.Overlay.show()
    
    def isVisible(self):
      return self.Overlay.isVisible()
    
    
    def __init__(self, Dimensions, Material,  t, caption, gui):
      self.x =Dimensions.x
      self.y =Dimensions.y
      self.w =Dimensions.z
      self.h =Dimensions.w
      self.GUI = gui
      self.Titlebar =  None
      self.Reziser=None
      self.ActiveTextInput=None
      self.ActiveButton=None 
      self.Buttons=[]
      self.TextInputs=[]
      
      self.Overlay = gui.createOverlay("BetaGUI.w" + str(gui.WindowCount), 
                     ogre.Vector2(Dimensions.x,Dimensions.y), ogre.Vector2(Dimensions.z,Dimensions.w), Material) 
      gui.WindowCount += 1
      if t >= 2:
         c = Callback()
         c.t=4
         self.Reziser=self.createButton(ogre.Vector4(Dimensions.z-16,Dimensions.w-16,16,16),Material+".resize", "",c)

      if t == 1 or t == 3 :
         c=Callback()
         c.t=3
         self.Titlebar = self.createButton(ogre.Vector4(0,0,Dimensions.z,22),Material + ".titlebar", caption, c)


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

      if not(px>=self.x and py>=self.y) or not(px<=self.x+self.w and py<=self.y+self.h):
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
    def __init__(self):
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

      ma=ogre.MaterialManager.getSingleton().getByName(self.MatNameNormal + ".active")
      if not ma:
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
      ma=ogre.MaterialManager.getSingleton().getByName(self.MatNameNormal + ".active")
      if not ma:
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
