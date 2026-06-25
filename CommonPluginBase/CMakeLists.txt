#
# Stuff the average programmer needs to change
#
set(SRCS 
	CommonConfigWindowBase.cpp
	SerializableBase.cpp
)
set(HFILES
	CommonConfigWindowBase.h
	SerializableBase.h
	CommonPlugin.h
)

set(INCLUDE_DIRECTORIES 
	${BASE_INCLUDE_DIRECTORIES} 
	${THIRD_PARTY_DIR}/wxXS/include
)

set(LIBRARY_NAME CommonPluginBase)

if(WIN32)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_USRDLL;/DIFLOOR_EXPORTS_COMMONPLUGINBASE)
endif(WIN32)

set(SRCS ${SRCS} ${HFILES} ${PROJECT_ROOT_DIR}/include/stdwx.h ${PROJECT_ROOT_DIR}/include/stdwx.cpp)

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

add_library(${LIBRARY_NAME} SHARED ${SRCS})

target_link_libraries(${LIBRARY_NAME} 
	${wxWidgets_LIBRARIES} 
	wxXS
)

target_precompile_headers(${LIBRARY_NAME}
    PRIVATE
    "${PROJECT_ROOT_DIR}/include/stdwx.h"
)