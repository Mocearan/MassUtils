cmake_minimum_required(VERSION 3.12)
PROJECT(common)

aux_source_directory(./ SRC)

set(CRYPT_SRC crypto.cpp)


INCLUDE_DIRECTORIES(
    ./ 
    ${3RD_PATH}/include 
    ${CONFIG_PATH}
)

# ADD_LIBRARY(mlog SHARED ${MLOG_SRC})
# target_link_libraries(mlog log4cplus)

add_library(mcrypt SHARED ${CRYPT_SRC})

#ADD_LIBRARY(common STATIC ${SRC})

#target_link_libraries(common pthread mlog crypto)

#FILE(GLOB BASE_INCLUDE "*.h")
#install(FILES ${BASE_INCLUDE} DESTINATION include)
#install(TARGETS base ARCHIVE DESTINATION lib)
# install(TARGETS mlog ARCHIVE DESTINATION lib)
# install(FILES ${3RD_PATH}/lib/liblog4cplus.so DESTINATION lib)
# install(FILES ${3RD_PATH}/lib/liblog4cplus-3.0.so.0 DESTINATION lib)
# install(FILES ${3RD_PATH}/lib/liblog4cplus-3.0.so.0.0.0 DESTINATION lib)