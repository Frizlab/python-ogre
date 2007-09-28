#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "OgreFontManager.h"

namespace QuickGUI
{
	Sheet::Sheet(const Ogre::String& name, Type type, const Ogre::String& texture, GUIManager* gm) :
		Panel(name,type,Rect(0,0,gm->getViewportWidth(),gm->getViewportHeight()),texture,NULL,NULL,gm),
		mDefaultTextColor(Ogre::ColourValue::White),
		mDefaultSkin("qgui"),
		mAutoNameWindowCounter(0)
	{
		Ogre::FontManager* fm = Ogre::FontManager::getSingletonPtr();
		Ogre::ResourceManager::ResourceMapIterator rmi = fm->getResourceIterator();
		if(rmi.hasMoreElements()) 
			mDefaultFont = rmi.getNext()->getName();
		else
			Ogre::Exception(1,"No fonts have been defined!","Sheet::Sheet");

		if(type == TYPE_SHEET)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
			mQuad->setSize(mSize);

			mScrollPane = new ScrollPane(mInstanceName+".ScrollPane",TYPE_SCROLL_PANE,this,this,mGUIManager);
		}
	}

	Sheet::~Sheet()
	{
	}

	void Sheet::_applyDimensions()
	{
		// cannot set position/size of sheet widget..
	}

	Window* Sheet::_createWindow(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Window* newWindow = new Window(name,TYPE_WINDOW,pixelDimensions,texture,this,this,mGUIManager);

		return newWindow;
	}

	Window* Sheet::createWindow(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// Many widgets can have no material, (label or empty window for example) but a regular window
		// must have a material!
		if( texture == "" ) return NULL;

		Ogre::String name = "DefaultWindow" + Ogre::StringConverter::toString(mAutoNameWindowCounter);
		++mAutoNameWindowCounter;

		return _createWindow(name,pixelDimensions,texture);
	}

	Window* Sheet::createWindow(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		// Many widgets can have no material, (label or empty window for example) but a regular window
		// must have a material!
		if( texture == "" ) return NULL;

		return _createWindow(name,pixelDimensions,texture);
	}

	Window* Sheet::createWindow(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = mDefaultSkin + ".window.png";

		return _createWindow(name,pixelDimensions,texture);
	}

	Window* Sheet::createWindow(const Rect& pixelDimensions)
	{
		Ogre::String name = "DefaultWindow" + Ogre::StringConverter::toString(mAutoNameWindowCounter);
		++mAutoNameWindowCounter;

		Ogre::String texture = mDefaultSkin + ".window.png";

		return _createWindow(name,pixelDimensions,texture);
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
}
