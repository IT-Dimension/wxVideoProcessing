#
# Stuff the average programmer needs to change
#
set(SRCS 
	wxTranslationHelper.cpp
)
set(HFILES 
	wxTranslationHelper.h
)
set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES})
set(LIBRARY_NAME wxTranslationHelper)
if(WIN32)
  	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};${wxWidgets_DEFINITIONS};/D_LIB)
endif(WIN32)
set(SRCS ${SRCS} ${HFILES})

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

add_library(${LIBRARY_NAME} STATIC ${SRCS})

target_link_libraries(${LIBRARY_NAME} ${wxWidgets_LIBRARIES})
