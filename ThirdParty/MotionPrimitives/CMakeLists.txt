#
# Stuff the average programmer needs to change
#
set(SRCS 
	iFloorMotionTypes.cpp 
	iFloorBlob.cpp)
set(HFILES
	iFloorMotionTypes.h 
	iFloorBlob.h)
set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES})
set(LIBRARY_NAME MotionPrimitives)
if(WIN32)
  	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_LIB)
endif(WIN32)
set(SRCS ${SRCS} ${HFILES})

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

add_library(${LIBRARY_NAME} STATIC ${SRCS})

