cmake_minimum_required(VERSION 3.12)
PROJECT(concurrent)

aux_source_directory(./ SRC)

set(CRYPT_SRC crypto.cpp)


INCLUDE_DIRECTORIES(
    ./ 
    ${3RD_PATH}/include 
    ${CONFIG_PATH}
    ${COMM_PATH}
)



#ADD_LIBRARY(common STATIC ${SRC})

#target_link_libraries(common pthread mlog crypto)

#FILE(GLOB BASE_INCLUDE "*.h")
#install(FILES ${BASE_INCLUDE} DESTINATION include)
#install(TARGETS base ARCHIVE DESTINATION lib)
# install(FILES ${3RD_PATH}/lib/liblog4cplus.so DESTINATION lib)
