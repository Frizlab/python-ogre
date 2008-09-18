#ifndef QUICKGUITEXTCURSOR_H
#define QUICKGUITEXTCURSOR_H

#include "QuickGUIBaseDesc.h"
#include "QuickGUIBrush.h"
#include "QuickGUIExportDLL.h"

#include "OgreFrameListener.h"

namespace QuickGUI
{
	class _QuickGUIExport TextCursor
	{
	public:
		TextCursor();
		~TextCursor();

		/**
		* Returns the position of the TextCursor.
		*/
		Point getPosition();
		/**
		* Returns the size of the TextCursor.
		*/
		Size getSize();
		/**
		* Returns true if the cursor is visible, false otherwise.
		*/
		bool getVisible();

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		void onDraw();

		/**
		* Sets the position of the TextCursor.
		* NOTE: The position is in texture coordinates.
		* NOTE: The position of the TextCursor is offset horizontally to 
		* center the cursor around the x coordinate.
		*/
		void setPosition(Point& p);
		/**
		* Sets the cursor visible or not.
		*/
		void setVisible(bool visible);

	protected:
		Point mPosition;
		Size mSize;
		bool mVisible;

	private:
	};
}

#endif
