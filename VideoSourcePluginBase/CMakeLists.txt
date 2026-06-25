#
# Stuff the average programmer needs to change
#
set(SRCS 
	VideoSourcePluginBase.cpp
	VideoGrabberBase.cpp
	VideoSourceConfigWindowBase.cpp
)
set(HFILES 
	VideoSourcePluginBase.h
	VideoGrabberBase.h
	VideoSourceConfigWindowBase.h
	VideoSourcePlugin.h
)

set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES}
	${THIRD_PARTY_DIR}/wxXS/include
	${PROJECT_ROOT_DIR}/CommonPluginBase
)

set(LIBRARY_NAME VideoSourcePluginBase)

if(WIN32)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_USRDLL;/DIFLOOR_EXPORTS)
endif(WIN32)

set(SRCS ${SRCS} ${HFILES} ${PROJECT_ROOT_DIR}/include/stdwx.h ${PROJECT_ROOT_DIR}/include/stdwx.cpp)

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

add_library(${LIBRARY_NAME} SHARED ${SRCS})

target_link_libraries(${LIBRARY_NAME}
    ${wxWidgets_LIBRARIES}
    CommonPluginBase
    Utils
    wxXS
)

add_dependencies(${LIBRARY_NAME}
	CommonPluginBase
)

target_precompile_headers(${LIBRARY_NAME}
    PRIVATE
    "${PROJECT_ROOT_DIR}/include/stdwx.h"
)