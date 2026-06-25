cmake_minimum_required(VERSION 3.5.0)
project(wxJSON)

file(GLOB_RECURSE SOURCES 
    "../src/*.cpp"
    "../include/wx/*.h"
)

add_library(wxJSON STATIC ${SOURCES})

target_include_directories(wxJSON PUBLIC "../include")

target_link_libraries(wxJSON PUBLIC ${wxWidgets_LIBRARIES})

target_compile_definitions(wxJSON PRIVATE ${PREPROCESSOR_DEFINITIONS})
