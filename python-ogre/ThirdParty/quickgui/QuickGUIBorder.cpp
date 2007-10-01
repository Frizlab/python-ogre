#include "QuickGUIBorder.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Border::Border(const Ogre::String& name, Type type, BorderType bType, const Rect& pixelDimensions, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,pixelDimensions,texture,container,ParentWidget,gm),
		mBorderType(bType)
	{
		mDraggingEnabled = true;
		mWidgetToDrag = NULL;

		switch(bType)
		{
		case BORDER_TYPE_TOP_LEFT:
			mHorizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
			mVerticalAnchor = ANCHOR_VERTICAL_TOP;
			break;
		case BORDER_TYPE_TOP_RIGHT:
			mHorizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
			mVerticalAnchor = ANCHOR_VERTICAL_TOP;
			break;
		case BORDER_TYPE_BOTTOM_LEFT:
			mHorizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
			mVerticalAnchor = ANCHOR_VERTICAL_BOTTOM;
			break;
		case BORDER_TYPE_BOTTOM_RIGHT:
			mHorizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
			mVerticalAnchor = ANCHOR_VERTICAL_BOTTOM;
			break;
		case BORDER_TYPE_LEFT:
			mHorizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
			mVerticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
			break;
		case BORDER_TYPE_TOP:
			mHorizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
			mVerticalAnchor = ANCHOR_VERTICAL_TOP;
			break;
		case BORDER_TYPE_RIGHT:
			mHorizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
			mVerticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
			break;
		case BORDER_TYPE_BOTTOM:
			mHorizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
			mVerticalAnchor = ANCHOR_VERTICAL_BOTTOM;
			break;
		}

		addEventHandler(EVENT_DRAGGED,&Border::onDragged,this);
	}

	Border::~Border()
	{
	}

	void Border::onDragged(const EventArgs& args)
	{
		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);

		switch(mBorderType)
		{
		case BORDER_TYPE_TOP_LEFT:
			mParentWidget->setScreenPosition(mea.position.x,mea.position.y);
			mParentWidget->setSize(mParentWidget->getWidth() - mea.moveDelta.x,mParentWidget->getHeight() - mea.moveDelta.y);
			break;
		case BORDER_TYPE_TOP_RIGHT:
			mParentWidget->setScreenYPosition(mea.position.y);
			mParentWidget->setSize(mParentWidget->getWidth() + mea.moveDelta.x,mParentWidget->getHeight() - mea.moveDelta.y);
			break;
		case BORDER_TYPE_BOTTOM_LEFT:
			mParentWidget->setScreenXPosition(mea.position.x);
			mParentWidget->setSize(mParentWidget->getWidth() - mea.moveDelta.x,mParentWidget->getHeight() + mea.moveDelta.y);
			break;
		case BORDER_TYPE_BOTTOM_RIGHT:
			mParentWidget->setSize(mParentWidget->getWidth() + mea.moveDelta.x,mParentWidget->getHeight() + mea.moveDelta.y);
			break;
		case BORDER_TYPE_LEFT:
			mParentWidget->setScreenXPosition(mea.position.x);
			mParentWidget->setWidth(mParentWidget->getWidth() - mea.moveDelta.x);
			break;
		case BORDER_TYPE_TOP:
			mParentWidget->setScreenYPosition(mea.position.y);
			mParentWidget->setHeight(mParentWidget->getHeight() - mea.moveDelta.y);
			break;
		case BORDER_TYPE_RIGHT:
			mParentWidget->setWidth(mParentWidget->getWidth() + mea.moveDelta.x);
			break;
		case BORDER_TYPE_BOTTOM:
			mParentWidget->setHeight(mParentWidget->getHeight() + mea.moveDelta.y);
			break;
		}
	}
}
