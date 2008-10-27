#include "QuickGUIForwardDeclaredDesc.h"

namespace QuickGUI
{
	PanelDesc::PanelDesc() :
		ContainerWidgetDesc()
	{
	}

	TabPageDesc::TabPageDesc() :
		PanelDesc()
	{
		for(int i = 0; i < PADDING_COUNT; ++i)
			padding[i] = 5.0;

		supportScrollBars = false;
		tabFrontWidth = -1;
	}

	void TabPageDesc::serialize(SerialBase* b)
	{
		PanelDesc::serialize(b);

		for(int i = 0; i < PADDING_COUNT; ++i)
			b->IO(StringConverter::toString(static_cast<Padding>(i)),&padding[i]);

		b->IO("TabFrontWidth",&tabFrontWidth);
	}

	TabControlDesc::TabControlDesc() :
		ContainerWidgetDesc()
	{
		supportScrollBars = false;
		tabHeight = 20;
		tabOverlap = 10;
		transparencyPicking = false;
		tabReordering = false;
	}

	void TabControlDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("TabHeight",&tabHeight);
		b->IO("TabOverlap",&tabOverlap);
		b->IO("TabReordering",&tabReordering);
	}
}
