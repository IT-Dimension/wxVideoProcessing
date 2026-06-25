#
# Stuff the average programmer needs to change
#
set(SRCS 
	GeometryProvider.cpp
	MotionDetectorPluginBase.cpp
	MotionDetectorBase.cpp
	MotionDetectorConfigWindowBase.cpp
	IFloorCameraSettings.cpp
)
set(HFILES 
	GeometryProvider.h
	MotionDetectorPluginBase.h
	MotionDetectorBase.h
	MotionDetectorConfigWindowBase.h
	MotionDetectorPlugin.h
	IFloorCameraSettings.h
)
set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES}
	${THIRD_PARTY_DIR}/wxXS/include
	${THIRD_PARTY_DIR}/wxJSON/include
	${THIRD_PARTY_DIR}/MotionPrimitives
	${PROJECT_ROOT_DIR}/CommonPluginBase
	${PROJECT_ROOT_DIR}/Utils
)
set(LIBRARY_NAME MotionDetectorPluginBase)
if(WIN32)
	set(LINK_DIRECTORIES
		${PROJECT_ROOT_DIR}/CommonPluginBase/${OS_BASE_NAME}${LIB_SUFFIX}/$(ConfigurationName)
		${PROJECT_ROOT_DIR}/Utils/${OS_BASE_NAME}${LIB_SUFFIX}/$(ConfigurationName)
		${THIRD_PARTY_DIR}/wxXS/build/${OS_BASE_NAME}${LIB_SUFFIX}/$(ConfigurationName)
		${THIRD_PARTY_DIR}/wxJSON/build/${OS_BASE_NAME}${LIB_SUFFIX}/$(ConfigurationName)
	)
  	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_USRDLL;/DIFLOOR_EXPORTS)
	set(IFLOOR_LIBS CommonPluginBase.lib Utils.lib)
	set(THIRDPARTY_LIBS wxXS.lib wxJSON.lib)
endif(WIN32)
if(LINUX)
	set(LINK_DIRECTORIES
		${PROJECT_ROOT_DIR}/CommonPluginBase/${OS_BASE_NAME}${LIB_SUFFIX}
		${PROJECT_ROOT_DIR}/Utils/${OS_BASE_NAME}${LIB_SUFFIX}
		${THIRD_PARTY_DIR}/wxXS/build/${OS_BASE_NAME}${LIB_SUFFIX}
		${THIRD_PARTY_DIR}/wxJSON/build/${OS_BASE_NAME}${LIB_SUFFIX}
	)
  	#set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_USRDLL;/DIFLOOR_EXPORTS)
	set(IFLOOR_LIBS CommonPluginBase Utils)
	set(THIRDPARTY_LIBS wxXS wxJSON)
endif(LINUX)
set(SRCS ${SRCS} ${HFILES} ${PROJECT_ROOT_DIR}/include/stdwx.h ${PROJECT_ROOT_DIR}/include/stdwx.cpp)

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})
link_directories(${LINK_DIRECTORIES})
add_library(${LIBRARY_NAME} SHARED ${SRCS})

set(DLL_DIR bin)
set(TARGET_LOCATION ${PROJECT_SOURCE_DIR}/${DLL_DIR}${LIB_SUFFIX})
set_target_properties(${LIBRARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TARGET_LOCATION})

target_link_libraries(${LIBRARY_NAME} 
    CommonPluginBase
    Utils
    wxXS       
    wxJSON       
    ${wxWidgets_LIBRARIES}
)
add_dependencies(${LIBRARY_NAME}
	wxXS 
	CommonPluginBase
)

# Precompiled header stuff must be after the target is added
#set_precompiled_header(${LIBRARY_NAME} ${PROJECT_ROOT_DIR}/include/stdwx.h ${PROJECT_ROOT_DIR}/include/stdwx.cpp)
target_precompile_headers(${LIBRARY_NAME}
    PRIVATE
    "${PROJECT_ROOT_DIR}/include/stdwx.h"
)


if(LINUX)
	add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${OS_BASE_NAME}${LIB_SUFFIX}/lib${LIBRARY_NAME}.so" "${PROJECT_SOURCE_DIR}/${DLL_DIR}${LIB_SUFFIX}/lib${LIBRARY_NAME}.so"
	)
endif(LINUX)

add_dependencies(MotionDetectorPluginBase wxXS)
