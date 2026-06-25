set(SRCS 
    VideoSourceDirectShowPlugin.cpp
    VideoSourceDirectShowApp.cpp
    VideoSourceDirectShowExports.cpp
    VideoGrabberDirectShow.cpp
    VideoSourceDirectShow.def
)
set(HFILES 
    VideoSourceDirectShowPlugin.h
    VideoGrabberDirectShow.h
    VideoSourceDirectShowApp.h
)
set(DSNATIVE_SRCS
    DSNative/Crossbar.cpp
    DSNative/Device.cpp
    DSNative/DeviceManager.cpp
    DSNative/Format.cpp
    DSNative/FrameGrabber.cpp
    DSNative/DirectShow/Sources/DSCategoryHelper.cpp
    DSNative/DirectShow/Sources/DSFileWriter.cpp
    DSNative/DirectShow/Sources/DSFilter.cpp
    DSNative/DirectShow/Sources/DSFilterGraphBase.cpp
    DSNative/DirectShow/Sources/DSObjectBase.cpp
    DSNative/DirectShow/Sources/DSSampleGrabber.cpp
)
set(DSNATIVE_HFILES
    DSNative/Crossbar.h
    DSNative/Device.h
    DSNative/DeviceManager.h
    DSNative/Format.h
    DSNative/FrameGrabber.h
    DSNative/DirectShow/Include/DSCategoryHelper.h
    DSNative/DirectShow/Include/DSCommon.h
    DSNative/DirectShow/Include/DSFilter.h
    DSNative/DirectShow/Include/DSFilterGraphBase.h
    DSNative/DirectShow/Include/DSFiltersDefinition.h
    DSNative/DirectShow/Include/DSObjectBase.h
)

source_group("DSNative\\DirectShow\\Sources" REGULAR_EXPRESSION DSNative/DirectShow/Sources/.*)
source_group("DSNative\\DirectShow\\Include" REGULAR_EXPRESSION DSNative/DirectShow/Include/.*)
source_group("DSNative\\Sources" REGULAR_EXPRESSION DSNative/.*\\.cpp)
source_group("DSNative\\Include" REGULAR_EXPRESSION DSNative/.*\\.h)

set(LOCAL_INCLUDE_DIR "${PROJECT_ROOT_DIR}/include")

set(INCLUDE_DIRECTORIES 
    ${LOCAL_INCLUDE_DIR}
    DSNative
    ${CMAKE_CURRENT_SOURCE_DIR}/../VideoSourcePluginBase
    ${CMAKE_CURRENT_SOURCE_DIR}/../CommonPluginBase
    ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/wxXS/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/strmbas
    ${OpenCV_INCLUDE_DIRS}
)

if(WIN32)
    set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${QEDIT_PATH})
    set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_USRDLL;/DEXPORTS_API=)
endif()

set(ALL_PROJECT_FILES ${SRCS} ${HFILES} ${DSNATIVE_SRCS} ${DSNATIVE_HFILES} "${LOCAL_INCLUDE_DIR}/stdwx.h" "${LOCAL_INCLUDE_DIR}/stdwx.cpp")

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

set(LIBRARY_NAME VideoSourceDirectShow)

add_library(${LIBRARY_NAME} SHARED ${ALL_PROJECT_FILES})

target_link_libraries(${LIBRARY_NAME}
    ${wxWidgets_LIBRARIES}
    VideoSourcePluginBase
    CommonPluginBase
    MotionDetectorPluginBase
    Utils
    strmbas
    strmiids
    quartz
    ole32
    oleaut32
    winmm
)

add_dependencies(${LIBRARY_NAME} 
    VideoSourcePluginBase 
    CommonPluginBase
    MotionDetectorPluginBase
)


if(WIN32)
    set_target_properties(${LIBRARY_NAME} PROPERTIES 
        LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib"
    )
endif()

target_precompile_headers(${LIBRARY_NAME}
    PRIVATE
    "${LOCAL_INCLUDE_DIR}/stdwx.h"
)

# Post Build Events
if(WIN32)
    add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "\"$(TargetPath)\"" "\"$(TargetDir)../$(TargetFileName)\""
    )
endif()

set(PLUGIN_TARGET_DIR "${OUTPUT_BIN_DIR}/plugins/video_source")

add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PLUGIN_TARGET_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "$<TARGET_FILE:${LIBRARY_NAME}>" 
        "${PLUGIN_TARGET_DIR}/$<TARGET_FILE_NAME:${LIBRARY_NAME}>"
)