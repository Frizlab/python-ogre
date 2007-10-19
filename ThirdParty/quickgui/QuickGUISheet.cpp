#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "OgreFontManager.h"

namespace QuickGUI
{
	Sheet::Sheet(const Ogre::String& name, const Ogre::String& texture, GUIManager* gm) :
		Panel(name,Size(gm->getViewportWidth(),gm->getViewportHeight()),texture,gm),
		mDefaultTextColor(Ogre::ColourValue::White),
		mSkin("qgui")
	{
		mParentSheet = this;
		mQuadContainer = this;
		mWidgetType = TYPE_SHEET;

		Ogre::FontManager* fm = Ogre::FontManager::getSingletonPtr();
		Ogre::ResourceManager::ResourceMapIterator rmi = fm->getResourceIterator();
		if(rmi.hasMoreElements()) 
			mFontName = rmi.getNext()->getName();
		else
			Ogre::Exception(1,"No fonts have been defined!","Sheet::Sheet");

		mQuad->setSize(mSize);
	}

	Sheet::~Sheet()
	{
		mParentSheet = NULL;
		mQuadContainer = NULL;
	}

	Window* Sheet::createWindow()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_WINDOW);
		mGUIManager->notifyNameUsed(name);

		Window* newWindow = new Window(name,Size(100,100),"qgui.window.png",mGUIManager);
		addChild(newWindow);
		newWindow->setPosition(0,0);
		newWindow->setFont(mFontName,true);
		
		return newWindow;
	}

	Widget* Sheet::getTargetWidget(const Point& pixelPosition)
	{
		Widget* w = NULL;

		// Iterate through Menu Layer Child Widgets.
		int widgetOffset = 0;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getQuadLayer() == Quad::LAYER_CHILD )
				continue;

			Widget* temp = (*it)->getTargetWidget(pixelPosition);
			if( (temp != NULL) && (temp->getOffset() > widgetOffset) )
			{
				widgetOffset = temp->getOffset();
				w = temp;
			}
		}
		if(w != NULL)
			return w;

		// Iterate through Windows, from highest offset to lowest.
		std::list<QuadContainer*>* windowList = QuadContainer::getWindowList();
		std::list<QuadContainer*>::reverse_iterator rit;
		for( rit = windowList->rbegin(); rit != windowList->rend(); ++rit )
		{
			if( (w = (*rit)->getOwner()->getTargetWidget(pixelPosition)) != NULL )
				return w;
		}

		// Iterate through Panels, from highest offset to lowest.
		std::list<QuadContainer*>* panelList = QuadContainer::getPanelList();
		for( rit = panelList->rbegin(); rit != panelList->rend(); ++rit )
		{
			if( (w = (*rit)->getOwner()->getTargetWidget(pixelPosition)) != NULL )
				return w;
		}

		// Iterate through Child Layer Child Widgets.
		widgetOffset = 0;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getQuadLayer() == Quad::LAYER_MENU )
				continue;

			Widget* temp = (*it)->getTargetWidget(pixelPosition);
			if( (temp != NULL) && (temp->getOffset() > widgetOffset) )
			{
				widgetOffset = temp->getOffset();
				w = temp;
			}
		}
		if(w != NULL)
			return w;

		return this;
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

	void Sheet::setQuadContainer(QuadContainer* container)
	{
		// do nothing, sheets don't belong inside another QuadContainer.
	}
}
