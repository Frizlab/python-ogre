# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http://www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
#
#	MyGUI is free software: you can redistribute it and/or modify
#	it under the terms of the GNU Lesser General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#	
#	MyGUI is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU Lesser General Public License for more details.
#
#	You should have received a copy of the GNU Lesser General Public License
#	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
# -----------------------------------------------------------------------------
import sys
sys.path.insert(0,'..')
import PythonOgreConfig
import ogre.renderer.OGRE as ogre
import ogre.gui.mygui as mygui
import SampleFramework as sf
import ogre.io.OIS as OIS
import os, sys

def editEventSelectAccept(_sender):
    print "subscribeEventSelectAccept arg1: %s" % str(_sender)
    
def editEventTextChange(_sender):
    print "subscribeEventTextChange arg1: %s" % str(_sender)
    
def widgetEventMouseLostFocus(widget1, widget2):
    print "subscribeEventMouseLostFocus arg1: %s\targ2: %s" % (widget1, widget2)
    
def widgetEventMouseSetFocus(widget1, widget2):
    print "subscribeEventMouseSetFocus arg1: %s\targ2: %s" % (widget1, widget2)
    
def widgetEventMouseDrag(widget, v1, v2):
    print "subscribeEventMouseDrag arg1: %s\targ2: %s\targ3: %s" % (str(widget), str(v1), str(v2))
    
def widgetEventMouseMove(widget, v1, v2):
    print "subscribeEventMouseMove arg1: %s\targ2: %s\targ3: %s" % (str(widget), str(v1), str(v2))
    
def widgetEventMouseWheel(widget, v1):
    print "subscribeEventMouseWheel arg1: %s\targ2: %s" % (str(widget), str(v1))
    
def widgetEventMouseButtonPressed(widget, v1, v2, v3):
    print "subscribeEventMouseButtonPressed arg1: %s\targ2: %s\targ3: %s\targ4: %s" % (str(widget), str(v1), str(v2), str(v3))
    
def widgetEventMouseButtonReleased(widget, v1, v2, v3):
    print "subscribeEventMouseButtonReleased arg1: %s\targ2: %s\targ3: %s\targ4: %s" % (str(widget), str(v1), str(v2), str(v3))
    
def widgetEventMouseButtonClick(widget):
    print "subscribeEventMouseButtonClick arg1: %s" % (str(widget))
    mygui.InputManager.getInstance().resetKeyFocusWidget(edit)

def widgetEventMouseButtonDoubleClick(widget):
    print "subscribeEventMouseButtonDoubleClick arg1: %s" % (str(widget))  
    
def widgetEventKeyLostFocus(widget, v1):
    print "subscribeEventKeyLostFocus arg1: %s\targ2: %s" % (str(widget), str(v1))
    
def widgetEventKeySetFocus(widget, v1):
    print "subscribeEventKeySetFocus arg1: %s\targ2: %s" % (str(widget), str(v1))
    
def widgetEventRootMouseChangeFocus(widget, v1):
    print "subscribeEventRootMouseChangeFocus arg1: %s\targ2: %s" % (str(widget), str(v1))
    
def widgetEventRootKeyChangeFocus(widget, v1):
    print "subscribeEventRootKeyChangeFocus arg1: %s\targ2: %s" % (str(widget), str(v1))
    
def widgetEventToolTip(widget, v1):
    print "subscribeEventToolTip arg1: %s\targ2: %s" % (str(widget), str(v1))
    
def widgetEventActionInfo(widget, v1, v2):
    print "subscribeEventToolTip arg1: %s\targ2: %s\targ3: %s" % (str(widget), str(v1), str(v2))

def listEventListSelectAccept(widget, v1):
    print "subscribeEventListSelectAccept arg1: %s\targ2: %s" % (str(widget), str(v1))
    
def listEventListChangePosition(widget, v1):
    print "subscribeEventListChangePosition arg1: %s\targ2: %s" % (str(widget), str(v1))
    
def listEventListMouseItemActivate(widget, v1):
    print "listEventListMouseItemActivate arg1: %s\targ2: %s" % (str(widget), str(v1))
    
def listEventListMouseItemFocus(widget, v1):
    print "listEventListMouseItemFocus arg1: %s\targ2: %s" % (str(widget), str(v1))
    
def listEventListChangeScroll(widget, v1):
    print "listEventListChangeScroll arg1: %s\targ2: %s" % (str(widget), str(v1))

def comboboxEventComboAccept(widget, v1):
    print "comboboxEventComboAccept arg1: %s\targ2: %s" % (str(widget), str(v1))

def comboboxEventComboChangePosition(widget, v1):
    print "comboboxEventComboChangePosition arg1: %s\targ2: %s" % (str(widget), str(v1))

 
 
def _buttonListAddClick(widget):
    global list
    list.addItem(ogre.UTFString("item " + str(list.getItemCount())), mygui.Any() )
    
def _buttonListRemoveClick(widget):
    global list
    sel = list.getIndexSelected()
    if sel != mygui.ITEM_NONE:
        list.removeItemAt(sel)
        
def _buttonListClearClick(widget):
    global list
    list.removeAllItems()

class MyGUISceneApplication(sf.Application):
    def _setUpResources ( self ):
        # first load the default resources
        sf.Application._setUpResources ( self )
        
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        
		# creating mygui
        self.gui = mygui.Gui()
        self.gui.initialise(self.renderWindow)
		
        window = self.gui.createWidgetT("Window", "Window", mygui.IntCoord(70, 70, 550, 450), mygui.Align(), "Overlapped", '')
        button = window.createWidgetT("Button", "Button", mygui.IntCoord(20, 30, 140, 30), mygui.Align())
        button.setCaption("#5555FFTest #55FF55python #FF5555button")
        print button.subscribeEventMouseLostFocus(widgetEventMouseLostFocus, '')
        print button.subscribeEventMouseSetFocus(widgetEventMouseSetFocus, '')
        print button.subscribeEventMouseDrag(widgetEventMouseDrag, '')
        print button.subscribeEventMouseMove(widgetEventMouseMove, '')
        print button.subscribeEventMouseWheel(widgetEventMouseWheel, '')
        print button.subscribeEventMouseButtonPressed(widgetEventMouseButtonPressed, '')
        print button.subscribeEventMouseButtonReleased(widgetEventMouseButtonReleased, '')
        print button.subscribeEventMouseButtonClick(widgetEventMouseButtonClick, '')
        print button.subscribeEventMouseButtonDoubleClick(widgetEventMouseButtonDoubleClick, '')
        print button.subscribeEventKeyLostFocus(widgetEventKeyLostFocus, '')
        print button.subscribeEventKeySetFocus(widgetEventKeySetFocus, '')
        print button.subscribeEventRootMouseChangeFocus(widgetEventRootMouseChangeFocus, '')
        print button.subscribeEventRootKeyChangeFocus(widgetEventRootKeyChangeFocus, '')
        print button.subscribeEventToolTip(widgetEventToolTip, '')
        print button.subscribeEventActionInfo(widgetEventActionInfo, '')
		
        global edit
        edit = window.createWidgetT("Edit", "Edit", mygui.IntCoord(20, 60, 140, 30), mygui.Align())
        edit.setCaption("#ffffffP#ffffeey#ffffddt#ffffcch#ffffbbo#ffffaan #ffff99e#ffff88d#ffff77i#ffff66t")
        print edit.subscribeEventSelectAccept(editEventSelectAccept, '')
        print edit.subscribeEventTextChange(editEventTextChange, '')
		
        mygui.InputManager.getInstance().setKeyFocusWidget(edit)
		
        canvas = window.createWidgetT("Canvas", "Canvas", mygui.IntCoord(20, 110, 140, 140), mygui.Align())
		
        global list
        list = window.createWidgetT("List", "List", mygui.IntCoord(20, 270, 140, 100), mygui.Align())
        print list.subscribeEventSelectAccept(listEventListSelectAccept, '')
        print list.subscribeEventChangePosition(listEventListChangePosition, '')
        print list.subscribeEventMouseItemActivate(listEventListMouseItemActivate, '')
        print list.subscribeEventMouseItemFocus(listEventListMouseItemFocus, '')
        print list.subscribeEventChangeScroll(listEventListChangeScroll, '')
		
        buttonAdd = window.createWidgetT("Button", "Button", mygui.IntCoord(170, 270, 60, 25), mygui.Align())
        buttonAdd.setCaption("Add")
        buttonAdd.subscribeEventMouseButtonClick(_buttonListAddClick, '')
		
        buttonRemove = window.createWidgetT("Button", "Button", mygui.IntCoord(170, 300, 60, 25), mygui.Align())
        buttonRemove.setCaption("Remove")
        buttonRemove.subscribeEventMouseButtonClick(_buttonListRemoveClick, '')
		
        buttonClear = window.createWidgetT("Button", "Button", mygui.IntCoord(170, 330, 60, 25), mygui.Align())
        buttonClear.setCaption("Clear")
        buttonClear.subscribeEventMouseButtonClick(_buttonListClearClick, '')

        combobox = window.createWidgetT("ComboBox", "ComboBox", mygui.IntCoord(180, 30, 150, 30), mygui.Align())
        combobox.setComboModeDrop(True)
        for item in xrange(100):
			combobox.addItem(ogre.UTFString("Item %d" % item), mygui.Any() )
        print combobox.subscribeEventAccept(comboboxEventComboAccept, '')
        print combobox.subscribeEventChangePosition(comboboxEventComboChangePosition, '')
		
        progress = window.createWidgetT("Progress", "Progress", mygui.IntCoord(180, 65, 150, 15), mygui.Align())
        progress.setProgressAutoTrack(True)
		
        menubar = window.createWidgetT("MenuCtrl", "MenuBar", mygui.IntCoord(0, 0, window.getWidth(), 25), mygui.Align())
		#item = menubar.addItem(ogre.UTFString("test"), mygui.MenuItemType.Normal, "id_test")
		
		
        mygui.InputManager.getInstance().setKeyFocusWidget(edit)
        
                
    def __del__(self):
		self.gui.shutdown()
		sf.Application.__del__(self)  

    def _createFrameListener(self):
		self.frameListener = MyGUIDemoFrameListener(self.renderWindow, self.camera, self.gui)
		self.root.addFrameListener(self.frameListener)
		self.frameListener.showDebugOverlay(True)
                

class MyGUIDemoFrameListener(sf.FrameListener , OIS.KeyListener, OIS.MouseListener):

	def __init__(self, renderWindow, camera, gui):
		sf.FrameListener.__init__(self, renderWindow, camera, True, True, True)
		OIS.KeyListener.__init__(self)
		OIS.MouseListener.__init__(self)
		
		self.Mouse.setEventCallback(self)
		self.Keyboard.setEventCallback(self)
		
		self.gui = gui
		self.needStop = False
		
	def __del__(self):
		sf.FrameListener.__del__(self)	
		
	def keyPressed(self, evt):
		self.gui.injectKeyPress(evt)
		
	def keyReleased(self, evt):
		self.gui.injectKeyRelease(evt)
		
		if (evt.key == OIS.KC_ESCAPE):
			self.needStop = True
		
	def mouseMoved(self, evt):
		self.gui.injectMouseMove(evt)
		
	def mousePressed(self, evt, _id):
		self.gui.injectMousePress(evt, _id)
		
	def mouseReleased(self, evt, _id):
		self.gui.injectMouseRelease(evt, _id)
		
	def frameStarted(self, evt):
		self.gui.injectFrameEntered(evt.timeSinceLastFrame)
		return not self.needStop

if __name__ == '__main__':
    try:
        application = MyGUISceneApplication()
        application.go()
    except ogre.OgreException, e:
        print e