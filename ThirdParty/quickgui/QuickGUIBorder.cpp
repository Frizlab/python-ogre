#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIBorder.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Border::Border(const std::string& name, GUIManager* gm) :
		Widget(name,gm),
		mThickness(5),
		mOverlap(1)
	{
		mWidgetType = TYPE_BORDER;
		mScrollPaneAccessible = false;
		mDraggingEnabled = true;
		mWidgetToDrag = NULL;
		mSize = Size(mThickness,mThickness);

		addEventHandler(EVENT_DRAGGED,&Border::onDragged,this);
		addEventHandler(EVENT_MOUSE_ENTER,&Border::onMouseEnter,this);
		addEventHandler(EVENT_MOUSE_LEAVE,&Border::onMouseLeave,this);
	}

	Border::~Border()
	{
	}

	void Border::_notifyParentSkinComponent(const std::string& skinComponent)
	{
		switch(mBorderType)
		{
		case BORDER_TYPE_TOP_LEFT:
			mSkinComponent = skinComponent + ".border.topleft";
			break;
		case BORDER_TYPE_TOP_RIGHT:
			mSkinComponent = skinComponent + ".border.topright";
			break;
		case BORDER_TYPE_BOTTOM_LEFT:
			mSkinComponent = skinComponent + ".border.bottomleft";
			break;
		case BORDER_TYPE_BOTTOM_RIGHT:
			mSkinComponent = skinComponent + ".border.bottomright";
			break;
		case BORDER_TYPE_LEFT:
			mSkinComponent = skinComponent + ".border.left";
			break;
		case BORDER_TYPE_TOP:
			mSkinComponent = skinComponent + ".border.top";
			break;
		case BORDER_TYPE_RIGHT:
			mSkinComponent = skinComponent + ".border.right";
			break;
		case BORDER_TYPE_BOTTOM:
			mSkinComponent = skinComponent + ".border.bottom";
			break;
		}

		if(mSkinName != "")
			setSkin(mSkinName);
	}

	Border::BorderType Border::getBorderType()
	{
		return mBorderType;
	}

	int Border::getOverlap()
	{
		return mOverlap;
	}

	int Border::getThickness()
	{
		return mThickness;
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

		std::string skin = mParentWidget->getSkin();

		switch(mBorderType)
		{
		case BORDER_TYPE_TOP_LEFT:
			mc->setCursorState(MouseCursor::CURSOR_STATE_RESIZE_DIAGONAL_1);
			break;
		case BORDER_TYPE_TOP_RIGHT:
			mc->setCursorState(MouseCursor::CURSOR_STATE_RESIZE_DIAGONAL_2);
			break;
		case BORDER_TYPE_BOTTOM_LEFT:
			mc->setCursorState(MouseCursor::CURSOR_STATE_RESIZE_DIAGONAL_2);
			break;
		case BORDER_TYPE_BOTTOM_RIGHT:
			mc->setCursorState(MouseCursor::CURSOR_STATE_RESIZE_DIAGONAL_1);
			break;
		case BORDER_TYPE_LEFT:
			mc->setCursorState(MouseCursor::CURSOR_STATE_RESIZE_HORIZONTAL);
			break;
		case BORDER_TYPE_TOP:
			mc->setCursorState(MouseCursor::CURSOR_STATE_RESIZE_VERTICAL);
			break;
		case BORDER_TYPE_RIGHT:
			mc->setCursorState(MouseCursor::CURSOR_STATE_RESIZE_HORIZONTAL);
			break;
		case BORDER_TYPE_BOTTOM:
			mc->setCursorState(MouseCursor::CURSOR_STATE_RESIZE_VERTICAL);
			break;
		}
	}

	void Border::onMouseLeave(const EventArgs& args)
	{
		if((mParentWidget == NULL) || (!mParentWidget->resizingAllowed()))
			return;

		mGUIManager->getMouseCursor()->setCursorState(MouseCursor::CURSOR_STATE_NORMAL);
	}

	void Border::setBorderType(BorderType t)
	{
		if(mParentWidget == NULL)
			return;

		// Set size to default values.
		mSize = Size(mThickness,mThickness);

		mBorderType = t;
		Size parentSize = mParentWidget->getSize();
		std::string parentSkinComponent = mParentWidget->getSkinComponent();

		switch(mBorderType)
		{
		case BORDER_TYPE_TOP_LEFT:
			mSkinComponent = parentSkinComponent + ".border.topleft";
			setPosition(-mThickness + mOverlap,-mThickness + mOverlap);
			break;
		case BORDER_TYPE_TOP_RIGHT:
			mSkinComponent = parentSkinComponent + ".border.topright";
			setPosition(parentSize.width - mOverlap,-mThickness + mOverlap);
			mHorizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
			break;
		case BORDER_TYPE_BOTTOM_LEFT:
			mSkinComponent = parentSkinComponent + ".border.bottomleft";
			setPosition(-mThickness + mOverlap,parentSize.height - mOverlap);
			mVerticalAnchor = ANCHOR_VERTICAL_BOTTOM;
			break;
		case BORDER_TYPE_BOTTOM_RIGHT:
			mSkinComponent = parentSkinComponent + ".border.bottomright";
			setPosition(parentSize.width - mOverlap,parentSize.height - mOverlap);
			mHorizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
			mVerticalAnchor = ANCHOR_VERTICAL_BOTTOM;
			break;
		case BORDER_TYPE_LEFT:
			mSkinComponent = parentSkinComponent + ".border.left";
			setPosition(-mThickness + mOverlap,0);
			setHeight(parentSize.height);
			mHorizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
			mVerticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
			break;
		case BORDER_TYPE_TOP:
			mSkinComponent = parentSkinComponent + ".border.top";
			setPosition(0,-mThickness + mOverlap);
			setWidth(parentSize.width);
			mHorizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
			break;
		case BORDER_TYPE_RIGHT:
			mSkinComponent = parentSkinComponent + ".border.right";
			setPosition(parentSize.width - mOverlap,0);
			setHeight(parentSize.height);
			mHorizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
			mVerticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
			break;
		case BORDER_TYPE_BOTTOM:
			mSkinComponent = parentSkinComponent + ".border.bottom";
			setPosition(0,parentSize.height - mOverlap);
			setWidth(parentSize.width);
			mHorizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
			mVerticalAnchor = ANCHOR_VERTICAL_BOTTOM;
			break;
		}

		_deriveAnchorValues();
		if(mSkinName != "")
			setSkin(mSkinName);
	}

	void Border::setOverlap(int lengthInPixels)
	{
		mOverlap = lengthInPixels;
		Size parentSize = mParentWidget->getSize();

		switch(mBorderType)
		{
		case BORDER_TYPE_TOP_LEFT:
			setPosition(-mThickness + mOverlap,-mThickness + mOverlap);
			break;
		case BORDER_TYPE_TOP_RIGHT:
			setPosition(parentSize.width - mOverlap,-mThickness + mOverlap);
			break;
		case BORDER_TYPE_BOTTOM_LEFT:
			setPosition(-mThickness + mOverlap,parentSize.height - mOverlap);
			break;
		case BORDER_TYPE_BOTTOM_RIGHT:
			setPosition(parentSize.width - mOverlap,parentSize.height - mOverlap);
			break;
		case BORDER_TYPE_LEFT:
			setPosition(-mThickness + mOverlap,0);
			break;
		case BORDER_TYPE_TOP:
			setPosition(0,-mThickness + mOverlap);
			break;
		case BORDER_TYPE_RIGHT:
			setPosition(parentSize.width - mOverlap,0);
			break;
		case BORDER_TYPE_BOTTOM:
			setPosition(0,parentSize.height - mOverlap);
			break;
		}

		_deriveAnchorValues();
	}

	void Border::setThickness(int lengthInPixels)
	{
		mThickness = lengthInPixels;
		Size parentSize = mParentWidget->getSize();

		switch(mBorderType)
		{
		case BORDER_TYPE_TOP_LEFT:
			setPosition(-mThickness + mOverlap,-mThickness + mOverlap);
			break;
		case BORDER_TYPE_TOP_RIGHT:
			setPosition(parentSize.width - mOverlap,-mThickness + mOverlap);
			break;
		case BORDER_TYPE_BOTTOM_LEFT:
			setPosition(-mThickness + mOverlap,parentSize.height - mOverlap);
			break;
		case BORDER_TYPE_BOTTOM_RIGHT:
			setPosition(parentSize.width - mOverlap,parentSize.height - mOverlap);
			break;
		case BORDER_TYPE_LEFT:
			setPosition(-mThickness + mOverlap,0);
			break;
		case BORDER_TYPE_TOP:
			setPosition(0,-mThickness + mOverlap);
			break;
		case BORDER_TYPE_RIGHT:
			setPosition(parentSize.width - mOverlap,0);
			break;
		case BORDER_TYPE_BOTTOM:
			setPosition(0,parentSize.height - mOverlap);
			break;
		}

		_deriveAnchorValues();
	}

	void Border::setParent(Widget* parent)
	{
		mParentWidget = parent;

		if(mParentWidget != NULL)
		{
			setQuadContainer(parent->getQuadContainer());
			setGUIManager(parent->getGUIManager());

			// set the correct offset
			setOffset(mParentWidget->getOffset() + 1);
			// calculated properties
			_deriveAnchorValues();
			// inheritted properties
			if(!mParentWidget->isVisible())
				hide();
			if(mInheritQuadLayer)
				setQuadLayer(mParentWidget->getQuadLayer());
			mGainFocusOnClick = mParentWidget->getGainFocusOnClick();
		}

		WidgetEventArgs args(this);
		fireEvent(EVENT_PARENT_CHANGED,args);
	}
}
