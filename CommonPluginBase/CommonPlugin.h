#ifndef _COMMONPLUGIN_H
#define _COMMONPLUGIN_H

#if defined(__WXMSW__)
#ifdef IFLOOR_EXPORTS_COMMONPLUGINBASE
#define IFLOOR_API_COMMONPLUGINBASE __declspec(dllexport)
#else
#define IFLOOR_API_COMMONPLUGINBASE __declspec(dllimport)
#endif
#else
#define IFLOOR_API_COMMONPLUGINBASE
#endif

#endif // _COMMONPLUGIN_H
