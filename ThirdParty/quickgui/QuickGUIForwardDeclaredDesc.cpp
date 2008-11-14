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
		supportScrollBars = false;
		tabFrontWidth = -1;
		verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;
	}

	void TabPageDesc::serialize(SerialBase* b)
	{
		PanelDesc::serialize(b);

		b->IO("TabFrontWidth",&tabFrontWidth);
		b->IO("VerticalTextAlignment",&verticalTextAlignment);
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
