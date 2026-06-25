cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME wxModularHost)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DWIN32_LEAN_AND_MEAN)

if(WIN32 OR APPLE)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
    set(LIB_SUFFIX "") 
endif()

if(UNIX AND NOT APPLE)
    set(LINUX ON)
    set(LIB_SUFFIX /${CMAKE_BUILD_TYPE}) 
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

if(APPLE)
    set(OS_BASE_NAME Mac)
    set(CMAKE_OSX_SYSROOT "macosx10.8")    
endif()
if(LINUX)
    set(OS_BASE_NAME Linux)
endif()
if(WIN32)
    set(OS_BASE_NAME Win)
endif()


set(OUTPUT_BIN_DIR "${PROJECT_SOURCE_DIR}/../bin")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_BIN_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_BIN_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BIN_DIR}")

set(CMAKE_INSTALL_RPATH "$ORIGIN")

foreach(OUTPUTCONFIG IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER ${OUTPUTCONFIG} CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${OUTPUT_BIN_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} "${OUTPUT_BIN_DIR}/lib")
endforeach()

link_directories("${CMAKE_BINARY_DIR}/lib")

set(wxWidgets_USE_LIBS base core adv aui net gl xml propgrid html)
set(BUILD_SHARED_LIBS 1)

find_package(wxWidgets REQUIRED)
add_definitions(-DWXUSINGDLL)
find_package(OpenCV REQUIRED)

if(WIN32)
    list(APPEND wxWidgets_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE" "_CRT_NONSTDC_NO_DEPRECATE" "_UNICODE" "UNICODE")
    add_compile_definitions(UNICODE _UNICODE _CRT_SECURE_NO_DEPRECATE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /wd4996")
endif()

if(WIN32 OR LINUX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D__WXDEBUG__=1")
endif()

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)
set(THIRD_PARTY_DIR ${PROJECT_ROOT_DIR}/ThirdParty)

include_directories(
    ${wxWidgets_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_ROOT_DIR}/include
    ${PROJECT_ROOT_DIR}/Utils
    ${PROJECT_ROOT_DIR}/MotionDetectorCore
    ${THIRD_PARTY_DIR}                        
    ${THIRD_PARTY_DIR}/include                
    ${THIRD_PARTY_DIR}/wxJSON/include
    ${THIRD_PARTY_DIR}/wxxmlserializer/include 
    ${THIRD_PARTY_DIR}/wxxmlserializer
)

add_subdirectory(../ThirdParty/build ../../ThirdParty/build/${OS_BASE_NAME}${LIB_SUFFIX})

if(TARGET wxXS)
    target_compile_definitions(wxXS PRIVATE WXMAKINGDLL_WXXS)
endif()

add_subdirectory(../Utils ../../Utils/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../wxModularCore ../../wxModularCore/${OS_BASE_NAME}${LIB_SUFFIX})    
add_subdirectory(../CommonPluginBase ../../CommonPluginBase/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../VideoSourcePluginBase ../../VideoSourcePluginBase/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../wxNonGuiPluginBase ../../wxNonGuiPluginBase/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../wxGuiPluginBase ../../wxGuiPluginBase/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../MotionDetectorPluginBase ../../MotionDetectorPluginBase/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../MotionDetectorCore ../../MotionDetectorCore/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../wxModularHost ../../wxModularHost/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../VideoSourceDirectShow ../../VideoSourceDirectShow/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../MotionDetectorMultiCam ../../MotionDetectorMultiCam/${OS_BASE_NAME}${LIB_SUFFIX})

add_dependencies(VideoSourceDirectShow CommonPluginBase Utils)
add_dependencies(CommonPluginBase wxXS)
add_dependencies(wxModularHost CommonPluginBase Utils wxModularCore)
add_dependencies(MotionDetectorMultiCam CommonPluginBase MotionDetectorCore MotionDetectorPluginBase Utils)