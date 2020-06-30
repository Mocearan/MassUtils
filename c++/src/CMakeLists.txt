cmake_minimum_required(VERSION 3.12)

project(src)

message(STATUS "cmake type ++++" ${CMAKE_BUILD_TYPE})
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

#add_compoile_options(-std=c++17) 

ADD_DEFINITIONS( 
    -W 
    -Wall 
    -D_REENTRANT 
    -D_FILE_OFFSET_BITS=64 
    -DAC_HAS_INFO
    -DAC_HAS_WARNING 
    -DAC_HAS_ERROR 
    -DAC_HAS_CRITICAL 
    -DTIXML_USE_STL
    -DAC_HAS_DEBUG 
    -DLINUX_DAEMON 
    -std=c++17 
    -DENCRYPT
)


set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -DNDEBUG")

set(PROJECT_BUILD_ROOT ${PROJECT_SOURCE_DIR}/../build)
set(3RD_PATH ${PROJECT_SOURCE_DIR}/../3rdparty)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)
set(COMM_PATH ${PROJECT_SOURCE_DIR}/common)
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

message(STATUS "Create Building Directory : " ${PROJECT_BUILD_ROOT})
message(STATUS "Specified 3rdparty path : " ${3RD_PATH})
message(STATUS "Create libraries generation directory : " ${LIBRARY_OUTPUT_PATH})


include_directories(${PROJECT_SOURCE_DIR})
link_directories(
  ${LIBRARY_OUTPUT_PATH}
  ${3RD_PATH}
)

LINK_DIRECTORIES(${3RD_PATH}/lib)

message(STATUS "include globle source file path : " ${PROJECT_SOURCE_DIR})
message(STATUS "dynamic link-libraries path : " ${LIBRARY_OUTPUT_PATH})

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})


add_subdirectory(common)
add_subdirectory(concurrent)
#add_subdirectory(db_proxy_server)
#add_subdirectory(file_server)
#add_subdirectory(http_msg_server)
#add_subdirectory(msfs)
#add_subdirectory(msg_server)
#add_subdirectory(push_server)
#add_subdirectory(route_server)
#add_subdirectory(tools)
add_subdirectory(test)

