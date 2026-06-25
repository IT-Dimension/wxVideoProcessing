#
# Stuff the average programmer needs to change
#
set(SRCS 
	appindic.cpp
)
set(HFILES 
	appindic.h
)
set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES} 
	/usr/include/libappindicator3-0.1
	/usr/include/gtk-3.0
	/usr/include/glib-2.0 /usr/lib/x86_64-linux-gnu/glib-2.0/include
	/usr/include/pango-1.0
	/usr/include/cairo
	/usr/include/gdk-pixbuf-2.0
	/usr/include/atk-1.0)
set(LIBRARY_NAME AppIndicatorHelper)
if(WIN32)
  	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};${wxWidgets_DEFINITIONS};/D_LIB)
endif(WIN32)
set(SRCS ${SRCS} ${HFILES})

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

add_library(${LIBRARY_NAME} STATIC ${SRCS})

target_link_libraries(${LIBRARY_NAME} ${wxWidgets_LIBRARIES})
