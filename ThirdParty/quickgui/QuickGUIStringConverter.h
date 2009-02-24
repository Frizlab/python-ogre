#ifndef QUICKGUISTRINGCONVERTER_H
#define QUICKGUISTRINGCONVERTER_H

#include "QuickGUIAnchors.h"
#include "QuickGUIBrushEnums.h"
#include "QuickGUIConsoleEnums.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIProgressBarEnums.h"
#include "QuickGUIEventTypes.h"
#include "QuickGUIScrollBarButtonLayouts.h"
#include "QuickGUITextAlignment.h"
#include "QuickGUIToolBarEnums.h"

#include "OgrePrerequisites.h"
#include "OgreStringConverter.h"

namespace QuickGUI
{
	class _QuickGUIExport StringConverter
	{
	public:
		static Ogre::String toString(BrushFilterMode m);
		static Ogre::String toString(ConsoleLayout l);
		static Ogre::String toString(HorizontalAnchor a);
		static Ogre::String toString(HorizontalTextAlignment a);
		static Ogre::String toString(HScrollBarButtonLayout l);
		static Ogre::String toString(ProgressBarFillDirection d);
		static Ogre::String toString(ProgressBarLayout l);
		static Ogre::String toString(ProgressBarClippingEdge e);
		static Ogre::String toString(ToolBarItemLayout l);
		static Ogre::String toString(VerticalAnchor a);
		static Ogre::String toString(VerticalTextAlignment a);
		static Ogre::String toString(VScrollBarButtonLayout l);
		static Ogre::String toString(WidgetEvent e);
		static Ogre::String toString(WindowEvent e);

		static BrushFilterMode parseBrushFilterMode(const Ogre::String& s);
		static ConsoleLayout parseConsoleLayout(const Ogre::String& s);
		static HorizontalAnchor parseHorizontalAnchor(const Ogre::String& s);
		static HorizontalTextAlignment parseHorizontalTextAlignment(const Ogre::String& s);
		static HScrollBarButtonLayout parseHScrollBarButtonLayout(const Ogre::String& s);
		static ProgressBarFillDirection parseProgressBarFillDirection(const Ogre::String& s);
		static ProgressBarLayout parseProgressBarLayout(const Ogre::String& s);
		static ProgressBarClippingEdge parseProgressBarClippingEdge(const Ogre::String& s);
		static ToolBarItemLayout parseToolBarItemLayout(const Ogre::String& s);
		static VerticalAnchor parseVerticalAnchor(const Ogre::String& s);
		static VerticalTextAlignment parseVerticalTextAlignment(const Ogre::String& s);
		static VScrollBarButtonLayout parseVScrollBarButtonLayout(const Ogre::String& s);
		static WidgetEvent parseWidgetEvent(const Ogre::String& s);
		static WindowEvent parseWindowEvent(const Ogre::String& s);

	//protected:
	private:
	};
}

#endif
