cmake_minimum_required(VERSION 3.5.0)
project(wxXS)

file(GLOB_RECURSE SOURCES 
    "../src/*.cpp"
    "../include/wx/*.h"
)

add_library(wxXS STATIC ${SOURCES})

target_compile_definitions(wxXS PRIVATE
    WXUSINGDLL
    WXMAKINGDLL_WXXS
)


target_include_directories(wxXS PUBLIC "../include")

target_link_libraries(wxXS PUBLIC wxJSON ${wxWidgets_LIBRARIES})

target_compile_definitions(wxXS PRIVATE ${PREPROCESSOR_DEFINITIONS})