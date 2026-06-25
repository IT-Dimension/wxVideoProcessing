#
# Stuff the average programmer needs to change
#
set(SRCS src/DatabaseErrorReporter.cpp 
	src/DatabaseLayer.cpp 
	src/DatabaseQueryParser.cpp 
	src/DatabaseResultSet.cpp 
	src/DatabaseStringConverter.cpp 
	src/PreparedStatement.cpp 
	src/SqliteDatabaseLayer.cpp 
	src/SqlitePreparedStatement.cpp 
	src/SqliteResultSet.cpp 
	src/SqliteResultSetMetaData.cpp 
	sqlite3/sqlite3.c
)

#set(SRCS ${SRCS} sqlite3/sqlite3.c)


set(HFILES include/DatabaseErrorCodes.h 
	include/DatabaseErrorReporter.h 
	include/DatabaseLayer.h 
	include/DatabaseLayerDef.h 
	include/DatabaseLayerException.h 
	include/DatabaseQueryParser.h 
	include/DatabaseResultSet.h 
	include/DatabaseStringConverter.h 
	include/PreparedStatement.h 
	include/ResultSetMetaData.h 
	include/SqliteDatabaseLayer.h 
	include/SqlitePreparedStatement.h 
	include/SqliteResultSet.h 
	include/SqliteResultSetMetaData.h)
set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES} include sqlite3)
set(LIBRARY_NAME DatabaseLayer)
if(WIN32)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};${wxWidgets_DEFINITIONS};/D_LIB)
endif(WIN32)
set(SRCS ${SRCS} ${HFILES})

add_library(${LIBRARY_NAME} STATIC ${SRCS})


target_compile_definitions(${LIBRARY_NAME} PRIVATE ${PREPROCESSOR_DEFINITIONS})

target_include_directories(${LIBRARY_NAME} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite3
	${BASE_INCLUDE_DIRECTORIES}
	${wxWidgets_INCLUDE_DIRS}
)


target_link_libraries(${LIBRARY_NAME} PUBLIC ${wxWidgets_LIBRARIES})