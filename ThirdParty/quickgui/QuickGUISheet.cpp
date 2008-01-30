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

		// Iterate through Menu Layer Child Widgets.
		int widgetOffset = 0;
		WidgetArray::iterator it;
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

	bool Sheet::overTransparentPixel(const Point& mousePixelPosition)
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
