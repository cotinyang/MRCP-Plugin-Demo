cmake_minimum_required (VERSION 2.8)
project (aneexrecog)

set(MY_BOOST_INCLUDE_DIR

    "/use/local/share/boost_1_66_0"
)
set(MY_BOOST_LIBRARY_x64_DIR

    "/usr/local/share/boost_1_66_0/stage/lib/7"
)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

set(AX_SRC_ROOT /usr/src/audioneex)
set(DATASTORE_T "TCDataStore")
add_definitions(-DDATASTORE_T_ID=1)
add_definitions(-DDATASTORE_T=${DATASTORE_T})

include_directories("${AX_SRC_ROOT}/src"
                    "${AX_SRC_ROOT}/audio" 
                    "${AX_SRC_ROOT}/DAO"
                    "${AX_SRC_ROOT}/tools")

# --- Platform-specific module compilation settings ---

set(APP_CXX_FLAGS "")
set(THREAD_LIB pthread)                    

# Set source files
set(INDEX_SRCS IndexFingerprints.cpp 
                  ${AX_SRC_ROOT}/DAO/${DATASTORE_T}.cpp 
                  ${AX_SRC_ROOT}/audio/AudioSource.cpp)

source_group ("src" FILES ${INDEX_SRCS})

# Plug-in declaration
add_library (${PROJECT_NAME} MODULE ${AINDEX_SRCS}
	$<TARGET_OBJECTS:aneexrecog>
)
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER "plugins")

find_library(DATASTORE_LIB
             NAMES tokyocabinet libtokyocabinet
             PATHS "/usr/local/lib")  
                            
# Input libraries
target_link_libraries(${PROJECT_NAME}
	${APU_LIBRARIES}
	${APR_LIBRARIES}
	${DATASTORE_LIB}
	${Boost_LIBRARIES} 
	${THREAD_LIB}
)

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
	-DDATASTORE_T_ID=1
	-DDATASTORE_T=${DATASTORE_T}
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

# Installation directives
install (TARGETS ${PROJECT_NAME} LIBRARY DESTINATION plugin)
if (MSVC)
	install (FILES ${PROJECT_BINARY_DIR}/Debug/${PROJECT_NAME}.pdb DESTINATION plugin CONFIGURATIONS Debug)
	install (FILES ${PROJECT_BINARY_DIR}/RelWithDebInfo/${PROJECT_NAME}.pdb DESTINATION plugin CONFIGURATIONS RelWithDebInfo)
endif (MSVC)