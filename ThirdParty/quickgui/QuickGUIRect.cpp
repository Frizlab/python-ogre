#include "QuickGUIRect.h"

namespace QuickGUI
{
	Rect::Rect() :
		x(0),
		y(0),
		width(0),
		height(0)
	{
	}

	Rect::Rect(Ogre::Real X, Ogre::Real Y, Ogre::Real Width, Ogre::Real Height) :
		x(X),
		y(Y),
		width(Width),
		height(Height)
	{
	}

	Rect::Rect(Point p, Size s) :
		x(p.x),
		y(p.y),
		width(s.width),
		height(s.height)
	{
	}

	bool Rect::inside(const Rect& r)
	{
		if( ((x - 0.00001) >= r.x) &&
			((y - 0.00001) >= r.y) &&
			(((x + width) - 0.00001) <= (r.x + r.width)) &&
			(((y + height) - 0.00001) <= (r.y + r.height)) )
			return true;

		return false;
	}

	const Rect Rect::ZERO( 0, 0, 0, 0 );
}
