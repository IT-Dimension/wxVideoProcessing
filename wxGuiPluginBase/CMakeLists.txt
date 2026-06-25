set(SRCS
	wxGuiPluginBase.cpp
	wxGuiPluginWindowBase.cpp
)
set(HEADERS
	Declarations.h
	wxGuiPluginBase.h
	wxGuiPluginWindowBase.h
)
	
set(LIBRARY_NAME wxGuiPluginBase)

if(WIN32)
	set(SRCS ${SRCS} ${LIBRARY_NAME}.rc)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};
		/D_USRDLL;/DDEMO_PLUGIN_EXPORTS;/D__STDC_CONSTANT_MACROS)
endif(WIN32)

set(SRCS ${SRCS} ${HEADERS} 
	${PROJECT_ROOT_DIR}/include/stdwx.h 
	${PROJECT_ROOT_DIR}/include/stdwx.cpp)

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES})

add_library(${LIBRARY_NAME} SHARED ${SRCS})

target_link_libraries(${LIBRARY_NAME} ${wxWidgets_LIBRARIES})

target_precompile_headers(${LIBRARY_NAME} PRIVATE ${PROJECT_ROOT_DIR}/include/stdwx.h)