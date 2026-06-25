#ifndef _VIDEOSOURCEPLUGIN_H
#define _VIDEOSOURCEPLUGIN_H

#if defined(__WXMSW__)
#ifdef IFLOOR_EXPORTS
#define IFLOOR_API __declspec(dllexport)
#else
#define IFLOOR_API __declspec(dllimport)
#endif
#else
#define IFLOOR_API
#endif

#endif // _VIDEOSOURCEPLUGIN_H
