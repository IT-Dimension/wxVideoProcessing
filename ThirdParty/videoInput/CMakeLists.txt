#
# Stuff the average programmer needs to change
#
set(SRCS 
	videoInput.cpp)
set(HFILES
	videoInput.h)
set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES})
set(LIBRARY_NAME videoInput)
if(WIN32)
	set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${QEDIT_PATH})
  	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_LIB;/D_WIN32_WINNT=0x0501)
endif(WIN32)
set(SRCS ${SRCS} ${HFILES})

add_library(${LIBRARY_NAME} STATIC ${SRCS})

target_compile_definitions(${LIBRARY_NAME} PRIVATE ${PREPROCESSOR_DEFINITIONS})

target_include_directories(${LIBRARY_NAME} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}
	${BASE_INCLUDE_DIRECTORIES}
	"${CMAKE_CURRENT_SOURCE_DIR}/../strmbas"
)

target_link_libraries(${LIBRARY_NAME} PUBLIC strmbas)