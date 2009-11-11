/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

/** All supported Events used in QuickGUI.
	@remarks
	Every widget supports functions to fire the events listed below.  
	These Events will go to the widget's corresponding event handlers, 
	and call user defined event handlers if they have been created.
	Events iterate through their child or parent widgets, until it
	is handled, or there are no more children/parents.
	@note
	Events are fired via the GUIManager, which interacts with injected
	inputs.
*/

#ifndef QUICKGUIEVENTARGS_H
#define QUICKGUIEVENTARGS_H

#include "OgrePrerequisites.h"
#include "OgreUTFString.h"

#include "QuickGUIKeyCode.h"
#include "QuickGUIMouseButtonID.h"
#include "QuickGUIPoint.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIEventTypes.h"

namespace QuickGUI
{
	class Widget;
	class Text;

	/** Basic EventArgs
	*/
	class _QuickGUIExport EventArgs
	{
	public:
		enum Type
		{
			TYPE_DEFAULT		=  0,
			TYPE_WIDGET				,
			TYPE_MOUSE				,
			TYPE_KEY				,
			TYPE_SCROLL				,
			TYPE_TEXT				
		};
	public:
		EventArgs() : handled(false)
		{
			type = TYPE_DEFAULT;
		}

		virtual ~EventArgs() {}

		// A virtual function is required to make the WidgetDesc class polymorphic
		virtual void virtualFunction() {}

		// handlers should set this to true if they handled the event.
		bool	handled;

		Type	type;
	};

	/** Widget EventArgs
	*/
	class _QuickGUIExport WidgetEventArgs : public EventArgs
	{
	public:
		WidgetEventArgs(Widget* w) : EventArgs(), widget(w)
		{
			type = TYPE_WIDGET;
		}
		WidgetEventArgs() : EventArgs(), widget(NULL)
		{
			type = TYPE_WIDGET;
		}

		virtual ~WidgetEventArgs() {}

		//pointer to a Widget object of relevance to the event.
		Widget*	widget;
	};

	/** Mouse EventArgs
	*/
	class _QuickGUIExport MouseEventArgs : public WidgetEventArgs
	{
	public:
		MouseEventArgs(Widget* w) : WidgetEventArgs(w) 
		{
			type = TYPE_MOUSE;
			button = MB_None;
			autoRepeat = false;
		}
		MouseEventArgs() : WidgetEventArgs() 
		{
			type = TYPE_MOUSE;
			button = MB_None;
			autoRepeat = false;
			eventType = MOUSE_EVENT_MOVE;
		}

		virtual ~MouseEventArgs() {}

		// holds current mouse position. (pixels)
		Point			position;
		// holds variation of mouse position from last mouse input
		Point			moveDelta;
		// stores the button being pressed/released
		MouseButtonID	button;
		// holds the bit mask storing buttons that are currently pressed.
		unsigned int	buttonMask;
		// holds the amount the scroll wheel has changed.
		float			wheelChange;	
		//! Bit field that holds status of Alt, Ctrl, Shift
		unsigned int	keyModifiers;
		// stores wether the event was caused via auto Repeat functionality, ie holding the mouse button down.
		bool			autoRepeat;
		// stores the type of mouse event, ie button down/up/click/move/scroll, etc.
		MouseEvent		eventType;
	};

	/** Key EventArgs
	*/
	class _QuickGUIExport KeyEventArgs : public WidgetEventArgs
	{
	public:
		KeyEventArgs(Widget* w) : WidgetEventArgs(w) 
		{
			type = TYPE_KEY;
			scancode = KC_UNASSIGNED;
			codepoint = 0;
			autoRepeat = false;
		}

		KeyEventArgs() : WidgetEventArgs()
		{
			type = TYPE_KEY;
			scancode = KC_UNASSIGNED;
			codepoint = 0;
			autoRepeat = false;
		}

		virtual ~KeyEventArgs() {}

		// codepoint for the key (only used for Character inputs).
		Ogre::UTFString::unicode_char	codepoint;		
		// Scan code of key that caused event (only used for key up & down inputs.
		KeyCode							scancode;
		// holds the bit mask storing buttons that are currently pressed.
		unsigned int					keyMask;
		//! Bit field that holds status of Alt, Ctrl, Shift
		unsigned int					keyModifiers;
		// stores wether the event was caused via auto Repeat functionality, ie holding the key down.
		bool							autoRepeat;
	};

	/** Text EventArgs
	*/
	class _QuickGUIExport TextEventArgs : public EventArgs
	{
	public:
		TextEventArgs(Text* t) : EventArgs(), text(t)
		{
			type = TYPE_TEXT;
			colorChanged = false;
			captionChanged = false;
			fontChanged = false;
		}

		virtual ~TextEventArgs() {}

		bool colorChanged;
		bool captionChanged;
		bool fontChanged;

		Text* text;
	};

	/** Scroll EventArgs
	*/
	class _QuickGUIExport ScrollEventArgs : public WidgetEventArgs
	{
	public:
		ScrollEventArgs(Widget* w) : WidgetEventArgs(w), sliderIncreasedPosition(false)
		{
			type = TYPE_SCROLL;
		}
		ScrollEventArgs() : WidgetEventArgs(), sliderIncreasedPosition(false)
		{
			type = TYPE_SCROLL;
		}

		virtual ~ScrollEventArgs() {}

		bool sliderIncreasedPosition;
	};
}

#endif
