#include "stdwx.h"
#if defined(__LINUX__)
#include <sys/time.h>

 unsigned int timeGetTime()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec) * 1000 + (now.tv_usec) / 1000;
}
#endif
