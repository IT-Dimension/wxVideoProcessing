#
# Stuff the average programmer needs to change
#
set(SRCS 
	../src/base64.cpp
	../src/connection.cpp
	../src/frame.cpp
	../src/sha1.cpp
	../src/websocket.cpp
	)
set(HFILES
	../include/stdint.h
	../include/tools.h
	../include/base64.h
	../include/connection.h
	../include/frame.h
	../include/sha1.h
	../include/websocket.h)
set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES}
	${THIRD_PARTY_DIR}/websocket/include
	${THIRD_PARTY_DIR}/libevent/include)
set(LIBRARY_NAME websocket)
if(WIN32)
  	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_LIB;/D_CRT_SECURE_NO_WARNINGS)
	# Workaround for Release build of this library. 
	# Some functions get removed by compiler optimization without this flag
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS} "/Ob0")
	set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES}
		${THIRD_PARTY_DIR}/libevent/WIN32-Code)
endif(WIN32)
set(SRCS ${SRCS} ${HFILES})

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

add_library(${LIBRARY_NAME} STATIC ${SRCS})