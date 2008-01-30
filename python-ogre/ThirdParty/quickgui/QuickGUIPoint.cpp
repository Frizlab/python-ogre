#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIPoint.h"

namespace QuickGUI
{
	Point::Point() :
		x(0),
		y(0)
	{
	}

	Point::Point(float X, float Y) :
		x(X),
		y(Y)
	{
	}

	const Point Point::ZERO( 0, 0 );
}
