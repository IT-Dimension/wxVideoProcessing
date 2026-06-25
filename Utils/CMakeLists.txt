#
# Stuff the average programmer needs to change
#
set(SRCS 
	wxFPValidator.cpp
	MACAddressUtility.cpp
	Guid.cpp
	BlobToJSONFormatter.cpp
	JSONObjectFactory.cpp
)
set(HFILES 
	wxFPValidator.h
	wxMACAddressUtility.h
	MACAddressUtility.h
	wxTemplateClientData.h
	Guid.h
	BlobToJSONFormatter.h
	JSONObjectFactory.h
)


set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES}
	${THIRD_PARTY_DIR}/wxJSON/include
	${THIRD_PARTY_DIR}/MotionPrimitives)
set(LIBRARY_NAME Utils)
if(WIN32)
 	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_LIB)
endif(WIN32)
set(SRCS ${SRCS} ${HFILES} ${PROJECT_ROOT_DIR}/include/stdwx.cpp ${PROJECT_ROOT_DIR}/include/stdwx.h)

#
# Non Builds programmer should not need to change anything beyond this
#

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

add_library(${LIBRARY_NAME} STATIC ${SRCS})
# Precompiled header stuff must be after the target is added
#set_precompiled_header(${LIBRARY_NAME} ${PROJECT_ROOT_DIR}/include/stdwx.h ${PROJECT_ROOT_DIR}/include/stdwx.cpp)


target_precompile_headers(${LIBRARY_NAME}
    PRIVATE
    "${PROJECT_ROOT_DIR}/include/stdwx.h"
)
