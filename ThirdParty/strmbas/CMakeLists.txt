#
# Stuff the average programmer needs to change
#
set(SRCS 
	amextra.cpp
	amfilter.cpp
	amvideo.cpp
	combase.cpp
	cprop.cpp
	ctlutil.cpp
	ddmm.cpp
	dllentry.cpp
	dllsetup.cpp
	mtype.cpp
	outputq.cpp
	pstream.cpp
	pullpin.cpp
	refclock.cpp
	renbase.cpp
	schedule.cpp
	seekpt.cpp
	source.cpp
	strmctl.cpp
	sysclock.cpp
	transfrm.cpp
	transip.cpp
	videoctl.cpp
	vtrans.cpp
	winctrl.cpp
	winutil.cpp
	wxdebug.cpp
	wxlist.cpp
	wxutil.cpp)
set(HFILES
	dllsetup.h
	dsschedule.h
	fourcc.h
	measure.h
	msgthrd.h
	mtype.h
	outputq.h
	pstream.h
	pullpin.h
	refclock.h
	reftime.h
	renbase.h
	seekpt.h
	source.h
	streams.h
	strmctl.h
	sysclock.h
	transfrm.h
	transip.h
	videoctl.h
	vtrans.h
	winctrl.h
	winutil.h
	wxdebug.h
	wxlist.h
	wxutil.h)
set(LIBRARY_NAME strmbas)
if(WIN32)
  	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_LIB;/D_WIN32_DCOM;/D_MBCS;/DDEBUG)
endif(WIN32)
set(SRCS ${SRCS} ${HFILES})

add_library(${LIBRARY_NAME} STATIC ${SRCS})

target_compile_definitions(${LIBRARY_NAME} PRIVATE ${PREPROCESSOR_DEFINITIONS})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_precompile_headers(${LIBRARY_NAME} PUBLIC streams.h)