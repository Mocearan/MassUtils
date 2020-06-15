cmake_minimum_required(VERSION 3.12)
PROJECT(test)

set(BASE_PATH ../common)

aux_source_directory(${BASE_PATH} COMM_SRC)

INCLUDE_DIRECTORIES(
    ./ 
    ${3RD_PATH}/include
    ${BASE_PATH}
)

LINK_DIRECTORIES(${3RD_PATH}/lib)
add_executable(tsingle t_singleton.cpp)
target_link_libraries(tsingle 
    pthread
)

add_executable(tstring t_stringutils.cpp ${BASE_PATH}/stringutils.cpp)

install(TARGETS tsingle RUNTIME DESTINATION test)
install(TARGETS tstring RUNTIME DESTINATION test)
#install(FILES ${CONFIG_PATH}/log4cplus.properties DESTINATION test)