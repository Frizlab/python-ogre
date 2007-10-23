#include "QuickGUIBorder.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Border::Border(const Ogre::String& instanceName, BorderType bType, const Size& pixelSize, Ogre::String texture, GUIManager* gm) :
		Image(instanceName,pixelSize,texture,gm),
		mBorderType(bType)
	{
		mWidgetType = TYPE_BORDER;
		mScrollPaneAccessible = false;
		mInheritClippingWidget = false;
		mDraggingEnabled = true;
		mWidgetToDrag = NULL;

		switch(mBorderType)
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
		addEventHandler(EVENT_MOUSE_ENTER,&Border::onMouseEnter,this);
		addEventHandler(EVENT_MOUSE_LEAVE,&Border::onMouseLeave,this);
	}

	Border::~Border()
	{
	}

	Border::BorderType Border::getBorderType()
	{
		return mBorderType;
	}

	void Border::onDragged(const EventArgs& args)
	{
		if((mParentWidget == NULL) || !mParentWidget->resizingAllowed())
			return;

		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);

		switch(mBorderType)
		{
		case BORDER_TYPE_TOP_LEFT:
			mParentWidget->setSize(mParentWidget->getWidth() - mea.moveDelta.x,mParentWidget->getHeight() - mea.moveDelta.y);
			mParentWidget->setScreenPosition(mea.position.x,mea.position.y);
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

	void Border::onMouseEnter(const EventArgs& args)
	{
		if((mParentWidget == NULL) || (!mParentWidget->resizingAllowed()))
			return;

		MouseCursor* mc = mGUIManager->getMouseCursor();
		mMouseCursorTexture = mc->getTexture();

		Ogre::String skin = mParentWidget->getSkin();

		switch(mBorderType)
		{
		case BORDER_TYPE_TOP_LEFT:
			mc->setTexture(skin+".cursor.resize.diagonal1.png");
			break;
		case BORDER_TYPE_TOP_RIGHT:
			mc->setTexture(skin+".cursor.resize.diagonal2.png");
			break;
		case BORDER_TYPE_BOTTOM_LEFT:
			mc->setTexture(skin+".cursor.resize.diagonal2.png");
			break;
		case BORDER_TYPE_BOTTOM_RIGHT:
			mc->setTexture(skin+".cursor.resize.diagonal1.png");
			break;
		case BORDER_TYPE_LEFT:
			mc->setTexture(skin+".cursor.resize.leftright.png");
			break;
		case BORDER_TYPE_TOP:
			mc->setTexture(skin+".cursor.resize.updown.png");
			break;
		case BORDER_TYPE_RIGHT:
			mc->setTexture(skin+".cursor.resize.leftright.png");
			break;
		case BORDER_TYPE_BOTTOM:
			mc->setTexture(skin+".cursor.resize.updown.png");
			break;
		}

		Point currentPosition = mc->getPosition();
		mc->centerOrigin();
		mc->setPosition(currentPosition.x,currentPosition.y);
	}

	void Border::onMouseLeave(const EventArgs& args)
	{
		if((mParentWidget == NULL) || (!mParentWidget->resizingAllowed()))
			return;

		MouseCursor* mc = mGUIManager->getMouseCursor();
		mc->setTexture(mMouseCursorTexture);
		Point currentPosition = mc->getPosition();
		mc->offsetOrigin(0,0);
		mc->setPosition(currentPosition.x,currentPosition.y);
	}
}
