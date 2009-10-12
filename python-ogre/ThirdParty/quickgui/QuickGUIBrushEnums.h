#ifndef QUICKGUIBRUSHENUMS_H
#define QUICKGUIBRUSHENUMS_H

namespace QuickGUI
{
	enum BrushBlendMode
	{
		BRUSHBLEND_NONE = 0,
		BRUSHBLEND_MODULATE,
		BRUSHBLEND_DISCARDALPHA,
		BRUSHBLEND_INVERT,
		BRUSHBLEND_ALPHA
	};

	enum BrushFilterMode
	{
		BRUSHFILTER_NONE	= 0,
		BRUSHFILTER_NEAREST		,
		BRUSHFILTER_LINEAR
	};
}

#endif
