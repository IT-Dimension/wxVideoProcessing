#include "stdwx.h"
#include "VideoSourceConfigWindowBase.h"


VideoSourceConfigWindowBase::VideoSourceConfigWindowBase()
: m_Plugin(NULL)
{
}

VideoSourceConfigWindowBase::VideoSourceConfigWindowBase(VideoSourcePluginBase * plugin, wxWindow * parent)
{
	Create(plugin, parent);
}

bool VideoSourceConfigWindowBase::Create(VideoSourcePluginBase * plugin, wxWindow * parent)
{
	m_Plugin = plugin;
	return wxPanel::Create(parent);
}

VideoSourceConfigWindowBase::~VideoSourceConfigWindowBase(void)
{
}

bool VideoSourceConfigWindowBase::ReadConfig()
{
	return false;
}

bool VideoSourceConfigWindowBase::SaveConfig()
{
	return false;
}
