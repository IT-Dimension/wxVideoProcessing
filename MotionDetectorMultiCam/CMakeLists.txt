cmake_minimum_required(VERSION 3.15)
project(MotionDetectorMultiCam)

set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_ROOT_DIR}/MotionDetectorCore
    ${PROJECT_ROOT_DIR}/MotionDetectorPluginBase
    ${PROJECT_ROOT_DIR}/wxGuiPluginBase
    ${PROJECT_ROOT_DIR}/CommonPluginBase
    ${PROJECT_ROOT_DIR}/Utils
    ${THIRD_PARTY_DIR}/wxXS/include
    ${OpenCV_INCLUDE_DIRS}
    ${THIRD_PARTY_DIR}/MotionPrimitives
)

if(CUDA_FOUND)
    set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/DHAVE_CUDA)
endif(CUDA_FOUND)

if(WIN32)
    set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${QEDIT_PATH})
    set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_USRDLL;/D_WIN32_WINNT=0x0501)
endif(WIN32)

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

set(STD_WX ${PROJECT_ROOT_DIR}/include/stdwx.h ${PROJECT_ROOT_DIR}/include/stdwx.cpp)

set(NONGUI_NAME MotionDetectorMultiCam)
set(NONGUI_SRCS
    MotionDetectorMultiCamPlugin.cpp
    MotionDetectorMultiCamExports.cpp
    MotionDetectorMultiCam.cpp
    MultiCamCameraManager.cpp
    ${STD_WX}
)

add_library(${NONGUI_NAME} SHARED ${NONGUI_SRCS})
target_compile_definitions(${NONGUI_NAME} PRIVATE -DMOTIONDETECTOR_MULTICAM_EXPORTS)

target_link_libraries(${NONGUI_NAME} ${wxWidgets_LIBRARIES} CommonPluginBase Utils MotionDetectorPluginBase MotionDetectorCore MotionPrimitives videoInput ${OpenCV_LIBS})
add_dependencies(${NONGUI_NAME} MotionDetectorPluginBase CommonPluginBase Utils MotionDetectorCore)
target_precompile_headers(${NONGUI_NAME} PRIVATE "${PROJECT_ROOT_DIR}/include/stdwx.h")

set(TARGET_NONGUI_DIR "${OUTPUT_BIN_DIR}")
add_custom_command(TARGET ${NONGUI_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TARGET_NONGUI_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${NONGUI_NAME}>" "${TARGET_NONGUI_DIR}/$<TARGET_FILE_NAME:${NONGUI_NAME}>"
)

set(GUI_NAME MotionDetectorMultiCamGui)
set(GUI_SRCS
    MotionDetectorMultiCamGuiPlugin.cpp
    MultiCamSettingsPanel.cpp
    ${STD_WX}
)

add_library(${GUI_NAME} SHARED ${GUI_SRCS})

target_link_libraries(${GUI_NAME} ${wxWidgets_LIBRARIES} ${NONGUI_NAME} CommonPluginBase Utils wxGuiPluginBase MotionDetectorCore)
add_dependencies(${GUI_NAME} ${NONGUI_NAME} wxGuiPluginBase CommonPluginBase Utils MotionDetectorCore)
target_precompile_headers(${GUI_NAME} PRIVATE "${PROJECT_ROOT_DIR}/include/stdwx.h")

set(TARGET_GUI_DIR "${OUTPUT_BIN_DIR}/plugins/gui")
add_custom_command(TARGET ${GUI_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TARGET_GUI_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${GUI_NAME}>" "${TARGET_GUI_DIR}/$<TARGET_FILE_NAME:${GUI_NAME}>"
)