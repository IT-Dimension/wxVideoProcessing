cmake_minimum_required(VERSION 3.5.0)
project(wxModularThirdParty)

set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS} PARENT_SCOPE)

add_subdirectory(../DatabaseLayer ../DatabaseLayer/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../wxXS/build ../wxXS/build/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../wxJSON/build ../wxJSON/build/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory(../MotionPrimitives ../MotionPrimitives/${OS_BASE_NAME}${LIB_SUFFIX})

if(WIN32)
    add_subdirectory(../strmbas ../strmbas/${OS_BASE_NAME}${LIB_SUFFIX})
    add_subdirectory(../videoInput ../videoInput/${OS_BASE_NAME}${LIB_SUFFIX})
endif(WIN32)