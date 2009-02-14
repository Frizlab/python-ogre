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
		tabpage_tabFrontWidth = 0;
		tabpage_tabWidthBuffer = 5;
		tabpage_verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;

		textDesc.resetToDefault();
	}

	void TabPageDesc::serialize(SerialBase* b)
	{
		PanelDesc::serialize(b);

		b->IO("TabFrontWidth",&tabpage_tabFrontWidth);
		b->IO("TabWidthBuffer",&tabpage_tabWidthBuffer);
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
