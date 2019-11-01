set(CMAKE_CXX_COMPILER "g++")
cmake_minimum_required (VERSION 2.8)
project (aneexrecog)

# Set source files
set(AX_SRC_ROOT /usr/src/audioneex/build)
set(DATASTORE_T "TCDataStore")
set(DATASTORE_LIB_NAME tokyocabinet)

set (ANEEX_RECOG_SOURCES
	src/aneex_recog_engine.c
	${AX_SRC_ROOT}/DAO/${DATASTORE_T}.cpp 
	${AX_SRC_ROOT}/audio/AudioSource.cpp
)

source_group ("src" FILES ${ANEEX_RECOG_SOURCES})

# Plug-in declaration
add_library (${PROJECT_NAME} MODULE ${ANEEX_RECOG_SOURCES}
	$<TARGET_OBJECTS:mrcpengine>
	$<TARGET_OBJECTS:mrcp>
	$<TARGET_OBJECTS:mpf>
	$<TARGET_OBJECTS:aprtoolkit>
)
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER "plugins")

                             
# Input libraries
target_link_libraries(${PROJECT_NAME}
	${APU_LIBRARIES}
	${APR_LIBRARIES}
)

find_library(DATASTORE_LIB
             NAMES tokyocabinet libtokyocabinet
             PATHS "/usr/local/lib")			 

# Input system libraries
if (WIN32)
	target_link_libraries(${PROJECT_NAME} ws2_32 winmm)
elseif (UNIX)
	target_link_libraries(${PROJECT_NAME} m)
endif ()

# Preprocessor definitions
add_definitions (
	${MRCP_DEFINES}
	${MPF_DEFINES}
	${APR_TOOLKIT_DEFINES}
	${APR_DEFINES}
	${APU_DEFINES}	
	${DATASTORE_T}
)

# Include directories
include_directories (
	${PROJECT_SOURCE_DIR}/include
	${MRCP_ENGINE_INCLUDE_DIRS}
	${MRCP_INCLUDE_DIRS}
	${MPF_INCLUDE_DIRS}
	${APR_TOOLKIT_INCLUDE_DIRS}
	${APR_INCLUDE_DIRS}
	${APU_INCLUDE_DIRS}
	
	${AX_SRC_ROOT}/src
    ${AX_SRC_ROOT}/audio 
    ${AX_SRC_ROOT}/DAO
    ${AX_SRC_ROOT}/tools	
)

target_link_libraries(aneex_recog_engine audioneex
                      ${Boost_LIBRARIES} ${DATASTORE_LIB})                  
target_compile_options(aneex_recog_engine PRIVATE -std=c++11)

# Installation directives
install (TARGETS ${PROJECT_NAME} LIBRARY DESTINATION plugin)
if (MSVC)
	install (FILES ${PROJECT_BINARY_DIR}/Debug/${PROJECT_NAME}.pdb DESTINATION plugin CONFIGURATIONS Debug)
	install (FILES ${PROJECT_BINARY_DIR}/RelWithDebInfo/${PROJECT_NAME}.pdb DESTINATION plugin CONFIGURATIONS RelWithDebInfo)
endif (MSVC)