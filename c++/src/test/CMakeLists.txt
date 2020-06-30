cmake_minimum_required(VERSION 3.12)
PROJECT(test)

set(BASE_PATH ../common)
set(CONCUR_PATH ../concurrent)

aux_source_directory(${BASE_PATH} COMM_SRC)

INCLUDE_DIRECTORIES(
    ./ 
    ${3RD_PATH}/include
    ${BASE_PATH}
    ${CONCUR_PATH}
)

LINK_DIRECTORIES(${3RD_PATH}/lib)

# test singleton
add_executable(tsingle t_singleton.cpp)
target_link_libraries(tsingle 
    pthread
)

# test string
add_executable(tstring t_stringutils.cpp ${BASE_PATH}/stringutils.cpp)

# test crypto
add_executable(tcrypt t_openssl.cpp ${BASE_PATH}/crypto.cpp ${BASE_PATH}/stringutils.cpp)
target_link_libraries(tcrypt
    ssl crypto
)

# test log
add_executable(tlog t_simplelog.cpp ${BASE_PATH}/simplelogutil.cpp ${BASE_PATH}/stringutils.cpp ${BASE_PATH}/datetimeutil.cpp)

# test concurrent
add_executable(tconcur t_concurrent.cpp)
target_link_libraries(tconcur
    pthread
)

install(TARGETS tsingle RUNTIME DESTINATION test/comm)
install(TARGETS tstring RUNTIME DESTINATION test/comm)
install(TARGETS tcrypt RUNTIME DESTINATION test/comm)
install(TARGETS tlog RUNTIME DESTINATION test/comm)
#install(FILES ${CONFIG_PATH}/log4cplus.properties DESTINATION test)

install(TARGETS tconcur RUNTIME DESTINATION test/concurrent)