#include "QuickGUIForwardDeclaredDesc.h"

namespace QuickGUI
{
	PanelDesc::PanelDesc(const Ogre::String& id) :
		ContainerWidgetDesc(id)
	{
		resetToDefault();
	}

	void PanelDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();
	}

	TabPageDesc::TabPageDesc(const Ogre::String& id) :
		PanelDesc(id)
	{
		resetToDefault();
	}

	void TabPageDesc::resetToDefault()
	{
		PanelDesc::resetToDefault();

		containerwidget_supportScrollBars = false;
		tabpage_tabFrontWidth = -1;
		tabpage_verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;
	}

	void TabPageDesc::serialize(SerialBase* b)
	{
		PanelDesc::serialize(b);

		b->IO("TabFrontWidth",&tabpage_tabFrontWidth);
		b->IO("VerticalTextAlignment",&tabpage_verticalTextAlignment);
	}

	TabControlDesc::TabControlDesc(const Ogre::String& id) :
		ContainerWidgetDesc(id)
	{
		resetToDefault();
	}

	void TabControlDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		containerwidget_supportScrollBars = false;
		tabcontrol_selectedTab = 0;
		tabcontrol_tabHeight = 20;
		tabcontrol_tabOverlap = 10;
		widget_transparencyPicking = false;
		tabcontrol_tabReordering = false;
	}

	void TabControlDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("tabcontrol_tabHeight",&tabcontrol_tabHeight);
		b->IO("tabcontrol_tabOverlap",&tabcontrol_tabOverlap);
		b->IO("tabcontrol_tabReordering",&tabcontrol_tabReordering);
	}
}
