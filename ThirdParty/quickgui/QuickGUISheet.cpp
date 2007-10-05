#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "OgreFontManager.h"

namespace QuickGUI
{
	Sheet::Sheet(const Ogre::String& name, const Ogre::String& texture, GUIManager* gm) :
		Panel(name,Rect(0,0,gm->getViewportWidth(),gm->getViewportHeight()),texture,gm),
		mDefaultTextColor(Ogre::ColourValue::White),
		mDefaultSkin("qgui"),
		mAutoNameWindowCounter(0)
	{
		mParentSheet = this;
		mWidgetType = TYPE_SHEET;

		Ogre::FontManager* fm = Ogre::FontManager::getSingletonPtr();
		Ogre::ResourceManager::ResourceMapIterator rmi = fm->getResourceIterator();
		if(rmi.hasMoreElements()) 
			mDefaultFont = rmi.getNext()->getName();
		else
			Ogre::Exception(1,"No fonts have been defined!","Sheet::Sheet");

		mQuad->setSize(mSize);
	}

	Sheet::~Sheet()
	{
	}

	Window* Sheet::createWindow()
	{
		Window* newWindow = new Window("Window"+Ogre::StringConverter::toString(mAutoNameWindowCounter++),Rect(0,0,100,100),"qgui.window.png",mGUIManager);
		addChild(newWindow);
		
		return newWindow;
	}

	Ogre::String Sheet::getDefaultFont()
	{
		return mDefaultFont;
	}

	Ogre::ColourValue Sheet::getDefaultTextColor()
	{
		return mDefaultTextColor;
	}

	Ogre::String Sheet::getDefaultSkin()
	{
		return mDefaultSkin;
	}

	Window* Sheet::getWindow(const Ogre::String& name)
	{
		if( name == "" ) return NULL;

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( ((*it)->getInstanceName() == name) && ((*it)->getWidgetType() == TYPE_WINDOW) ) 
				return dynamic_cast<Window*>(*it);
		}

		return NULL;
	}

	void Sheet::setDefaultFont(const Ogre::String& font)
	{
		mDefaultFont = font;
	}

	void Sheet::setDefaultTextColor(const Ogre::ColourValue& color)
	{
		mDefaultTextColor = color;
	}

	void Sheet::setDefaultSkin(const Ogre::String& skin)
	{
		mDefaultSkin = skin;
	}

	void Sheet::setQuadContainer(QuadContainer* container)
	{
		// do nothing, sheets don't belong inside another QuadContainer.
	}
}
