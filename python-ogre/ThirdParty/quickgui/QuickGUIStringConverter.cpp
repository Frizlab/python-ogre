#include "QuickGUIStringConverter.h"

namespace QuickGUI
{
	Ogre::String StringConverter::toString(BrushFilterMode m)
	{
		switch(m)
		{
		case BRUSHFILTER_NONE:										return "BRUSHFILTER_NONE";
		case BRUSHFILTER_NEAREST:									return "BRUSHFILTER_NEAREST";
		case BRUSHFILTER_LINEAR:									return "BRUSHFILTER_LINEAR";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"BrushFilterMode type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(HorizontalAnchor a)
	{
		switch(a)
		{
		case ANCHOR_HORIZONTAL_LEFT:								return "ANCHOR_HORIZONTAL_LEFT";
		case ANCHOR_HORIZONTAL_RIGHT:								return "ANCHOR_HORIZONTAL_RIGHT";
		case ANCHOR_HORIZONTAL_LEFT_RIGHT:							return "ANCHOR_HORIZONTAL_LEFT_RIGHT";
		case ANCHOR_HORIZONTAL_NONE:								return "ANCHOR_HORIZONTAL_NONE";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"HorizontalAnchor type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(Padding p)
	{
		switch(p)
		{
		case PADDING_LEFT:											return "PADDING_LEFT";
		case PADDING_RIGHT:											return "PADDING_RIGHT";
		case PADDING_TOP:											return "PADDING_TOP";
		case PADDING_BOTTOM:										return "PADDING_BOTTOM";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"Padding type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(VScrollBarButtonLayout l)
	{
		switch(l)
		{
		case VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP:					return "VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP";
		case VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN:				return "VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN";
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH:				return "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH";
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP:					return "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP";
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN:				return "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN";
		case VSCROLL_BAR_BUTTON_LAYOUT_NONE:						return "VSCROLL_BAR_BUTTON_LAYOUT_NONE";
		case VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE:					return "VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"VScrollBarButtonLayout type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(HScrollBarButtonLayout l)
	{
		switch(l)
		{
		case HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT:				return "HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT";
		case HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT:				return "HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT";
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH:				return "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH";
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT:				return "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT";
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT:				return "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT";
		case HSCROLL_BAR_BUTTON_LAYOUT_NONE:						return "HSCROLL_BAR_BUTTON_LAYOUT_NONE";
		case HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE:					return "HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"HScrollBarButtonLayout type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ProgressBarFillDirection d)
	{
		switch(d)
		{
		case PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE:				return "PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE";
		case PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE:				return "PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarFillDirection type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ProgressBarLayout l)
	{
		switch(l)
		{
		case PROGRESSBAR_LAYOUT_HORIZONTAL:							return "PROGRESSBAR_LAYOUT_HORIZONTAL";
		case PROGRESSBAR_LAYOUT_VERTICAL:							return "PROGRESSBAR_LAYOUT_VERTICAL";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarLayout type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ProgressBarClippingEdge e)
	{
		switch(e)
		{
		case PROGRESSBAR_CLIP_LEFT_BOTTOM:						return "PROGRESSBAR_CLIP_LEFT_BOTTOM";
		case PROGRESSBAR_CLIP_RIGHT_TOP:						return "PROGRESSBAR_CLIP_RIGHT_TOP";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarClippingEdge type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(TextAlignment a)
	{
		switch(a)
		{
		case TEXT_ALIGNMENT_LEFT:									return "TEXT_ALIGNMENT_LEFT";
		case TEXT_ALIGNMENT_RIGHT:									return "TEXT_ALIGNMENT_RIGHT";
		case TEXT_ALIGNMENT_CENTER:									return "TEXT_ALIGNMENT_CENTER";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"TextAlignment type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ToolBarItemLayout l)
	{
		switch(l)
		{
		case TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE:				return "TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE";
		case TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE:				return "TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"ToolBarItemLayout type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(VerticalAnchor a)
	{
		switch(a)
		{
		case ANCHOR_VERTICAL_BOTTOM:								return "ANCHOR_VERTICAL_BOTTOM";
		case ANCHOR_VERTICAL_NONE:									return "ANCHOR_VERTICAL_NONE";
		case ANCHOR_VERTICAL_TOP:									return "ANCHOR_VERTICAL_TOP";
		case ANCHOR_VERTICAL_TOP_BOTTOM:							return "ANCHOR_VERTICAL_TOP_BOTTOM";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"VerticalAnchor type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(WidgetEvent e)
	{
		switch(e)
		{
		case WIDGET_EVENT_ENABLED_CHANGED:							return "WIDGET_EVENT_ENABLED_CHANGED";
		case WIDGET_EVENT_POSITION_CHANGED:							return "WIDGET_EVENT_POSITION_CHANGED";
		case WIDGET_EVENT_SCROLL_CHANGED:							return "WIDGET_EVENT_SCROLL_CHANGED";
		case WIDGET_EVENT_SIZE_CHANGED:								return "WIDGET_EVENT_SIZE_CHANGED";
		case WIDGET_EVENT_VISIBLE_CHANGED:							return "WIDGET_EVENT_VISIBLE_CHANGED";
		case WIDGET_EVENT_CHARACTER_KEY:							return "WIDGET_EVENT_CHARACTER_KEY";
		case WIDGET_EVENT_DRAGGED:									return "WIDGET_EVENT_DRAGGED";
		case WIDGET_EVENT_DROPPED:									return "WIDGET_EVENT_DROPPED";
		case WIDGET_EVENT_KEY_DOWN:									return "WIDGET_EVENT_KEY_DOWN";
		case WIDGET_EVENT_KEY_UP:									return "WIDGET_EVENT_KEY_UP";
		case WIDGET_EVENT_KEYBOARD_INPUT_GAIN:						return "WIDGET_EVENT_KEYBOARD_INPUT_GAIN";
		case WIDGET_EVENT_KEYBOARD_INPUT_LOSE:						return "WIDGET_EVENT_KEYBOARD_INPUT_LOSE";
		case WIDGET_EVENT_MOUSE_BUTTON_DOWN:						return "WIDGET_EVENT_MOUSE_BUTTON_DOWN";
		case WIDGET_EVENT_MOUSE_BUTTON_UP:							return "WIDGET_EVENT_MOUSE_BUTTON_UP";
		case WIDGET_EVENT_MOUSE_CLICK:								return "WIDGET_EVENT_MOUSE_CLICK";
		case WIDGET_EVENT_MOUSE_CLICK_DOUBLE:						return "WIDGET_EVENT_MOUSE_CLICK_DOUBLE";
		case WIDGET_EVENT_MOUSE_CLICK_TRIPLE:						return "WIDGET_EVENT_MOUSE_CLICK_TRIPLE";
		case WIDGET_EVENT_MOUSE_ENTER:								return "WIDGET_EVENT_MOUSE_ENTER";
		case WIDGET_EVENT_MOUSE_LEAVE:								return "WIDGET_EVENT_MOUSE_LEAVE";
		case WIDGET_EVENT_MOUSE_MOVE:								return "WIDGET_EVENT_MOUSE_MOVE";
		case WIDGET_EVENT_MOUSE_WHEEL:								return "WIDGET_EVENT_MOUSE_WHEEL";
		case WIDGET_EVENT_ON_HOVER:									return "WIDGET_EVENT_ON_HOVER";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"Widget Event does not have a string equivalent!  Time to update!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(WindowEvent e)
	{
		switch(e)
		{
		case WINDOW_EVENT_DRAWN:									return "WINDOW_EVENT_DRAWN";
		case WINDOW_EVENT_FOCUS_GAINED:								return "WINDOW_EVENT_FOCUS_GAINED";
		case WINDOW_EVENT_FOCUS_LOST:								return "WINDOW_EVENT_FOCUS_LOST";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"Window Event does not have a string equivalent!  Time to update!","StringConverter::toString");
		}
	}

	BrushFilterMode StringConverter::parseBrushFilterMode(const Ogre::String& s)
	{
		if(s == "BRUSHFILTER_NONE")									return BRUSHFILTER_NONE;
		else if(s == "BRUSHFILTER_NEAREST")							return BRUSHFILTER_NEAREST;
		else if(s == "BRUSHFILTER_LINEAR")							return BRUSHFILTER_LINEAR;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized BrushFilterMode type! (Possible need to update?)","StringConverter::parseBrushFilterMode");
	}

	HorizontalAnchor StringConverter::parseHorizontalAnchor(const Ogre::String& s)
	{
		if(s == "ANCHOR_HORIZONTAL_LEFT")							return ANCHOR_HORIZONTAL_LEFT;
		else if(s == "ANCHOR_HORIZONTAL_RIGHT")						return ANCHOR_HORIZONTAL_RIGHT;
		else if(s == "ANCHOR_HORIZONTAL_LEFT_RIGHT")				return ANCHOR_HORIZONTAL_LEFT_RIGHT;
		else if(s == "ANCHOR_HORIZONTAL_NONE")						return ANCHOR_HORIZONTAL_NONE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized HorizontalAnchor type! (Possible need to update?)","StringConverter::parseHorizontalAnchor");
	}

	Padding StringConverter::parsePadding(const Ogre::String& s)
	{
		if(s == "PADDING_LEFT")										return PADDING_LEFT;
		else if(s == "PADDING_RIGHT")								return PADDING_RIGHT;
		else if(s == "PADDING_TOP")									return PADDING_TOP;
		else if(s == "PADDING_BOTTOM")								return PADDING_BOTTOM;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized Padding type! (Possible need to update?)","StringConverter::parsePadding");
	}

	VScrollBarButtonLayout StringConverter::parseVScrollBarButtonLayout(const Ogre::String& s)
	{
		if(s == "VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP")			return VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN")		return VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH")		return VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP")		return VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN")		return VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_NONE")				return VSCROLL_BAR_BUTTON_LAYOUT_NONE;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE")			return VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized parseVScrollBarButtonLayout type! (Possible need to update?)","StringConverter::parseVScrollBarButtonLayout");
	}

	HScrollBarButtonLayout StringConverter::parseHScrollBarButtonLayout(const Ogre::String& s)
	{
		if(s == "HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT")			return HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT")	return HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH")		return HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT")		return HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT")	return HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_NONE")				return HSCROLL_BAR_BUTTON_LAYOUT_NONE;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE")			return HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized parseHScrollBarButtonLayout type! (Possible need to update?)","StringConverter::parseHScrollBarButtonLayout");
	}

	ProgressBarFillDirection StringConverter::parseProgressBarFillDirection(const Ogre::String& s)
	{
		if(s == "PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE")			return PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE;
		else if(s == "PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE")		return PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarFillDirection type! (Possible need to update?)","StringConverter::parseProgressBarFillDirection");
	}
	
	ProgressBarLayout StringConverter::parseProgressBarLayout(const Ogre::String& s)
	{
		if(s == "PROGRESSBAR_LAYOUT_HORIZONTAL")					return PROGRESSBAR_LAYOUT_HORIZONTAL;
		else if(s == "PROGRESSBAR_LAYOUT_VERTICAL")					return PROGRESSBAR_LAYOUT_VERTICAL;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarLayout type! (Possible need to update?)","StringConverter::parseProgressBarLayout");
	}

	ProgressBarClippingEdge StringConverter::parseProgressBarClippingEdge(const Ogre::String& s)
	{
		if(s == "PROGRESSBAR_CLIP_LEFT_BOTTOM")						return PROGRESSBAR_CLIP_LEFT_BOTTOM;
		else if(s == "PROGRESSBAR_CLIP_RIGHT_TOP")					return PROGRESSBAR_CLIP_RIGHT_TOP;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarClippingEdge type! (Possible need to update?)","StringConverter::parseProgressBarClippingEdge");
	}

	TextAlignment StringConverter::parseTextAlignment(const Ogre::String& s)
	{
		if(s == "TEXT_ALIGNMENT_LEFT")								return TEXT_ALIGNMENT_LEFT;
		else if(s == "TEXT_ALIGNMENT_RIGHT")						return TEXT_ALIGNMENT_RIGHT;
		else if(s == "TEXT_ALIGNMENT_CENTER")						return TEXT_ALIGNMENT_CENTER;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized TextAlignment type! (Possible need to update?)","StringConverter::parseTextAlignment");
	}

	ToolBarItemLayout StringConverter::parseToolBarItemLayout(const Ogre::String& s)
	{
		if(s == "TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE")			return TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE;
		else if(s == "TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE")	return TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ToolBarItemLayout type! (Possible need to update?)","StringConverter::parseToolBarItemLayout");
	}

	VerticalAnchor StringConverter::parseVerticalAnchor(const Ogre::String& s)
	{
		if(s == "ANCHOR_VERTICAL_BOTTOM")							return ANCHOR_VERTICAL_BOTTOM;
		else if(s == "ANCHOR_VERTICAL_NONE")						return ANCHOR_VERTICAL_NONE;
		else if(s == "ANCHOR_VERTICAL_TOP")							return ANCHOR_VERTICAL_TOP;
		else if(s == "ANCHOR_VERTICAL_TOP_BOTTOM")					return ANCHOR_VERTICAL_TOP_BOTTOM;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized VerticalAnchor type! (Possible need to update?)","StringConverter::parseVerticalAnchor");
	}

	WidgetEvent StringConverter::parseWidgetEvent(const Ogre::String& s)
	{
		if(s == "WIDGET_EVENT_ENABLED_CHANGED")						return WIDGET_EVENT_ENABLED_CHANGED;
		else if(s == "WIDGET_EVENT_POSITION_CHANGED")				return WIDGET_EVENT_POSITION_CHANGED;
		else if(s == "WIDGET_EVENT_SCROLL_CHANGED")					return WIDGET_EVENT_SCROLL_CHANGED;
		else if(s == "WIDGET_EVENT_SIZE_CHANGED")					return WIDGET_EVENT_SIZE_CHANGED;
		else if(s == "WIDGET_EVENT_VISIBLE_CHANGED")				return WIDGET_EVENT_VISIBLE_CHANGED;
		else if(s == "WIDGET_EVENT_CHARACTER_KEY")					return WIDGET_EVENT_CHARACTER_KEY;
		else if(s == "WIDGET_EVENT_DRAGGED")						return WIDGET_EVENT_DRAGGED;
		else if(s == "WIDGET_EVENT_DROPPED")						return WIDGET_EVENT_DROPPED;
		else if(s == "WIDGET_EVENT_KEY_DOWN")						return WIDGET_EVENT_KEY_DOWN;
		else if(s == "WIDGET_EVENT_KEY_UP")							return WIDGET_EVENT_KEY_UP;
		else if(s == "WIDGET_EVENT_KEYBOARD_INPUT_GAIN")			return WIDGET_EVENT_KEYBOARD_INPUT_GAIN;
		else if(s == "WIDGET_EVENT_KEYBOARD_INPUT_LOSE")			return WIDGET_EVENT_KEYBOARD_INPUT_LOSE;
		else if(s == "WIDGET_EVENT_MOUSE_BUTTON_DOWN")				return WIDGET_EVENT_MOUSE_BUTTON_DOWN;
		else if(s == "WIDGET_EVENT_MOUSE_BUTTON_UP")				return WIDGET_EVENT_MOUSE_BUTTON_UP;
		else if(s == "WIDGET_EVENT_MOUSE_CLICK")					return WIDGET_EVENT_MOUSE_CLICK;
		else if(s == "WIDGET_EVENT_MOUSE_CLICK_DOUBLE")				return WIDGET_EVENT_MOUSE_CLICK_DOUBLE;
		else if(s == "WIDGET_EVENT_MOUSE_CLICK_TRIPLE")				return WIDGET_EVENT_MOUSE_CLICK_TRIPLE;
		else if(s == "WIDGET_EVENT_MOUSE_ENTER")					return WIDGET_EVENT_MOUSE_ENTER;
		else if(s == "WIDGET_EVENT_MOUSE_LEAVE")					return WIDGET_EVENT_MOUSE_LEAVE;
		else if(s == "WIDGET_EVENT_MOUSE_MOVE")						return WIDGET_EVENT_MOUSE_MOVE;
		else if(s == "WIDGET_EVENT_MOUSE_WHEEL")					return WIDGET_EVENT_MOUSE_WHEEL;
		else if(s == "WIDGET_EVENT_ON_HOVER")						return WIDGET_EVENT_ON_HOVER;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized Widget Event! (Possible need to update?)","StringConverter::parseWidgetEvent");
	}

	WindowEvent StringConverter::parseWindowEvent(const Ogre::String& s)
	{
		if(s == "WINDOW_EVENT_DRAWN")								return WINDOW_EVENT_DRAWN;
		else if(s == "WINDOW_EVENT_FOCUS_GAINED")					return WINDOW_EVENT_FOCUS_GAINED;
		else if(s == "WINDOW_EVENT_FOCUS_LOST")						return WINDOW_EVENT_FOCUS_LOST;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized Winhdow Event! (Possible need to update?)","StringConverter::parseWindowEvent");
	}
}
