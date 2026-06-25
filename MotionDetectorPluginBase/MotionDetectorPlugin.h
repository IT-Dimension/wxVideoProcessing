#ifndef _MOTIONDETECTORPLUGIN_H
#define _MOTIONDETECTORPLUGIN_H

#if defined(__WXMSW__)
#ifdef IFLOOR_EXPORTS
#define IFLOOR_API __declspec(dllexport)
#else
#define IFLOOR_API __declspec(dllimport)
#endif
#else
#define IFLOOR_API
#endif

#endif // _MOTIONDETECTORPLUGIN_H
