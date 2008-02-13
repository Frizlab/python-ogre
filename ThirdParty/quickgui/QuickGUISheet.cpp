#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIVector4.h"

#include "OgreFontManager.h"

namespace QuickGUI
{
	Sheet::Sheet(const std::string& name, const std::string& skinName, GUIManager* gm) :
		Panel(name,gm),
		mMaterialName("")
	{
		mGUIManager = gm;
		mQuadContainer = this;
		mQuad->_notifyQuadContainer(this);
		mWidgetType = TYPE_SHEET;
		mSkinName = skinName;
		mSkinComponent = ".sheet";
		setSize(gm->getViewportWidth(),gm->getViewportHeight());

		Ogre::FontManager* fm = Ogre::FontManager::getSingletonPtr();
		Ogre::ResourceManager::ResourceMapIterator rmi = fm->getResourceIterator();
		if(rmi.hasMoreElements())
			mFontName = rmi.getNext()->getName();
		else
			Ogre::Exception(1,"No fonts have been defined!","Sheet::Sheet");
	}

	Sheet::~Sheet()
	{
		mQuadContainer = NULL;
	}

	Window* Sheet::createWindow()
	{
		return createWindow(mGUIManager->generateName(TYPE_WINDOW));
	}

	Window* Sheet::createWindow(const std::string& name)
	{
		if(mGUIManager->isNameUnique(name))
		{
			mGUIManager->notifyNameUsed(name);
			return dynamic_cast<Window*>(_createChild(name,TYPE_WINDOW));
		}
		else
		{
			std::string name = mGUIManager->generateName(TYPE_WINDOW);
			mGUIManager->notifyNameUsed(name);
			return dynamic_cast<Window*>(_createChild(name,TYPE_WINDOW));
		}
	}

	Widget* Sheet::getTargetWidget(const Point& pixelPosition)
	{
		Widget* w = NULL;

		// Iterate through Child Widgets and record the highest offset menu and child layer widget.
		Widget* highestMenuLayerWidget = NULL;
		int highestMenuOffset = -1;

		Widget* highestChildLayerWidget = NULL;
		int highestChildOffset = -1;

		WidgetArray::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getQuadLayer() == Quad::LAYER_CHILD )
			{
				Widget* temp = (*it)->getTargetWidget(pixelPosition);
				if( (temp != NULL) && (temp->getOffset() > highestChildOffset) )
				{
					highestChildOffset = temp->getOffset();
					highestChildLayerWidget = temp;
				}
			}
			else // LAYER_MENU
			{
				Widget* temp = (*it)->getTargetWidget(pixelPosition);
				if( (temp != NULL) && (temp->getOffset() > highestMenuOffset) )
				{
					highestMenuOffset = temp->getOffset();
					highestMenuLayerWidget = temp;
				}
			}
		}

		if(highestMenuLayerWidget != NULL)
			return highestMenuLayerWidget;

		// Iterate through Windows, from highest offset to lowest.
		QuadContainerList* windowList = QuadContainer::getWindowList();
		QuadContainerList::reverse_iterator rit;
		for( rit = windowList->rbegin(); rit != windowList->rend(); ++rit )
		{
			w = (*rit)->getOwner();
			if (w != NULL)
			{
				w = w->getTargetWidget(pixelPosition);
				if (w != NULL)
					return w;
			}
		}

		// Iterate through Panels, from highest offset to lowest.
		QuadContainerList* panelList = QuadContainer::getPanelList();
		for( rit = panelList->rbegin(); rit != panelList->rend(); ++rit )
		{
			w = (*rit)->getOwner();
			if (w != NULL)
			{
				w = w->getTargetWidget(pixelPosition);
				if (w != NULL)
					return w;
			}
		}

		if(highestChildLayerWidget != NULL)
			return highestChildLayerWidget;

		// If we made it here, we are inside this Widget's bounds, but not over any non-transparent child widget areas.

		if( !overTransparentPixel(pixelPosition) )
			return this;
		else // We're over a transparent pixel
			return NULL;
	}

	const Widget* Sheet::getTargetWidget(const Point& pixelPosition) const
	{
		Widget* w = NULL;

		// Iterate through Child Widgets and record the highest offset menu and child layer widget.
		Widget* highestMenuLayerWidget = NULL;
		int highestMenuOffset = -1;

		Widget* highestChildLayerWidget = NULL;
		int highestChildOffset = -1;

		WidgetArray::const_iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getQuadLayer() == Quad::LAYER_CHILD )
			{
				Widget* temp = (*it)->getTargetWidget(pixelPosition);
				if( (temp != NULL) && (temp->getOffset() > highestChildOffset) )
				{
					highestChildOffset = temp->getOffset();
					highestChildLayerWidget = temp;
				}
			}
			else // LAYER_MENU
			{
				Widget* temp = (*it)->getTargetWidget(pixelPosition);
				if( (temp != NULL) && (temp->getOffset() > highestMenuOffset) )
				{
					highestMenuOffset = temp->getOffset();
					highestMenuLayerWidget = temp;
				}
			}
		}

		if(highestMenuLayerWidget != NULL)
			return highestMenuLayerWidget;

		// Iterate through Windows, from highest offset to lowest.
		const QuadContainerList* windowList = QuadContainer::getWindowList();
		QuadContainerList::const_reverse_iterator rit;
		for( rit = windowList->rbegin(); rit != windowList->rend(); ++rit )
		{
			w = (*rit)->getOwner();
			if (w != NULL)
			{
				w = w->getTargetWidget(pixelPosition);
				if (w != NULL)
					return w;
			}
		}

		// Iterate through Panels, from highest offset to lowest.
		const QuadContainerList* panelList = QuadContainer::getPanelList();
		for( rit = panelList->rbegin(); rit != panelList->rend(); ++rit )
		{
			w = (*rit)->getOwner();
			if (w != NULL)
			{
				w = w->getTargetWidget(pixelPosition);
				if (w != NULL)
					return w;
			}
		}

		if(highestChildLayerWidget != NULL)
			return highestChildLayerWidget;

		// If we made it here, we are inside this Widget's bounds, but not over any non-transparent child widget areas.

		if( !overTransparentPixel(pixelPosition) )
			return this;
		else // We're over a transparent pixel
			return NULL;
	}

	Window* Sheet::getWindow(const std::string& name)
	{
		if( name.empty() ) return NULL;

		WidgetArray::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( ((*it)->getInstanceName() == name) && ((*it)->getWidgetType() == TYPE_WINDOW) )
				return dynamic_cast<Window*>(*it);
		}

		return NULL;
	}

	bool Sheet::overTransparentPixel(const Point& mousePixelPosition) const
	{
		if(mMaterialName == "")
			return Widget::overTransparentPixel(mousePixelPosition);
		return false;
	}

	void Sheet::setHeight(float pixelHeight)
	{
		Widget::setHeight(pixelHeight);
	}

	void Sheet::setQuadContainer(QuadContainer* container)
	{
		// do nothing, sheets don't belong inside another QuadContainer.
	}

	void Sheet::setSize(const float& pixelWidth, const float& pixelHeight)
	{
		Widget::setSize(pixelWidth,pixelHeight);
	}

	void Sheet::setSize(const Size& pixelSize)
	{
		Widget::setSize(pixelSize);
	}

	void Sheet::setMaterial(const std::string& materialName)
	{
		if(mTextureLocked)
			return;

		mMaterialName = materialName;
		mQuad->setMaterial(materialName);
		mQuad->setTextureCoordinates(Vector4(0,0,1,1));
	}

	void Sheet::setSkin(const std::string& skinName, bool recursive)
	{
		mMaterialName = "";
		Widget::setSkin(skinName,recursive);
	}

	void Sheet::setWidth(float pixelWidth)
	{
		Widget::setWidth(pixelWidth);
	}
}
