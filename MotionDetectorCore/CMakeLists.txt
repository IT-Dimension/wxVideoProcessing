#
# Stuff the average programmer needs to change
#
set(SRCS
	FilterPreviewWindow.cpp
	AmplifyFilter.cpp
	BackgroundFilter.cpp
	BarrelDistortionCorrectionFilter.cpp
	BrightnessContrastFilter.cpp
	ContourFinder.cpp
	CropFilter.cpp
	DummyFilter.cpp
	DynamicBGFilter.cpp
	EqualizeHistFilter.cpp
	FilterFactory.cpp
	FilterTemplate.cpp
	FlipFilter.cpp
	HighpassFilter.cpp
	IFloorImagePreprocessor.cpp
	IFloorImagePreprocessorSettings.cpp
	InvertFilter.cpp
	MonoFilter.cpp
	PerspectiveFilter.cpp
	RectifyFilter.cpp
	ResizeFilter.cpp
	ScalerFilter.cpp
	ShapeFilter.cpp
	SimpleBGFilter.cpp
	SubOrBGFilter.cpp
	XorBGFilter.cpp
	SimpleHighpassFilter.cpp
	SmoothingFilter.cpp
	ThresholdFilter.cpp
	KinectMonoFilter.cpp
	KinectDepthBGFilter.cpp
	KinectBGFilter.cpp
	KinectThresholdFilter.cpp
	Tracking.cpp
	DisplayGeometryWindow.cpp
	LinuxUtils.cpp
)

set(HFILES
	MotionDetectorCorePlugin.h
	FilterPreviewWindow.h
	wxBitmapEvent.h
	AmplifyFilter.h
	BackgroundFilter.h
	BarrelDistortionCorrectionFilter.h
	BrightnessContrastFilter.h
	ContourFinder.h
	CropFilter.h
	DummyFilter.h
	DynamicBGFilter.h
	EqualizeHistFilter.h
	FilterFactory.h
	FilterTemplate.h
	FlipFilter.h
	HighpassFilter.h
	IFloorImagePreprocessor.h
	IFloorImagePreprocessorSettings.h
	Image.h
	InvertFilter.h
	MonoFilter.h
	PerspectiveFilter.h
	RectifyFilter.h
	ResizeFilter.h
	ScalerFilter.h
	ShapeFilter.h
	SimpleBGFilter.h
	SubOrBGFilter.h
	XorBGFilter.h
	SimpleHighpassFilter.h
	SmoothingFilter.h
	ThresholdFilter.h
	KinectMonoFilter.h
	KinectDepthBGFilter.h
	KinectBGFilter.h
	KinectThresholdFilter.h
	Tracking.h
	vector2d.h
	DisplayGeometryWindow.h
	LinuxUtils.h
)

source_group("Filters\\Headers" ".*Filter\\.h")
source_group("Filters\\Sources" ".*Filter\\.cpp")

set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES}
	${PROJECT_ROOT_DIR}/MotionDetectorPluginBase
	${PROJECT_ROOT_DIR}/CommonPluginBase
	${THIRD_PARTY_DIR}/wxXS/include
	${OpenCV_INCLUDE_DIRS}
	${THIRD_PARTY_DIR}/MotionPrimitives
)

set(LIBRARY_NAME MotionDetectorCore)

if(CUDA_FOUND)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/DHAVE_CUDA)
endif(CUDA_FOUND)

if(WIN32)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_USRDLL;/DMOTION_DETECTOR_CORE_EXPORTS)
endif(WIN32)


set(SRCS ${SRCS} ${HFILES} ${PROJECT_ROOT_DIR}/include/stdwx.h ${PROJECT_ROOT_DIR}/include/stdwx.cpp)

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})


add_library(${LIBRARY_NAME} SHARED ${SRCS})


target_link_libraries(${LIBRARY_NAME}
	${wxWidgets_LIBRARIES}
	CommonPluginBase
	Utils
	MotionDetectorPluginBase
	MotionPrimitives
	${OpenCV_LIBS}
)

add_dependencies(${LIBRARY_NAME}
	MotionDetectorPluginBase
	CommonPluginBase
)

target_precompile_headers(${LIBRARY_NAME}
    PRIVATE
    "${PROJECT_ROOT_DIR}/include/stdwx.h"
)

if(LINUX)
	add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${OS_BASE_NAME}${LIB_SUFFIX}/lib${LIBRARY_NAME}.so" "${PROJECT_SOURCE_DIR}/${DLL_DIR}${LIB_SUFFIX}/lib${LIBRARY_NAME}.so"
	)
endif(LINUX)

set(PLUGIN_TARGET_DIR "${OUTPUT_BIN_DIR}/plugins/motion_detector")

add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PLUGIN_TARGET_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "$<TARGET_FILE:${LIBRARY_NAME}>" 
        "${PLUGIN_TARGET_DIR}/$<TARGET_FILE_NAME:${LIBRARY_NAME}>"
)