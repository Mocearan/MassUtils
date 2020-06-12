# 此为顶层CMakeLists.txt，用以配置版本，安装，发布包，license，版权等信息。

cmake_minimum_required(VERSION 3.12)

project(MassUtils)

# 配置版本信息
set(MassUtils_VERSION_MAJOR ${VERSION_MAJOR})
set(MassUtils_VERSION_MINOR ${VERSION_MINOR})
set(MassUtils_VERSION_PATCH ${VERSION_PATCH})

set(CONFIG_PATH ${PROJECT_SOURCE_DIR}/conf)
message(STATUS ${CONFIG_PATH})     
#configure_file (         # 生成程序版本信息
#  ${CONFIG_PATH}/config.h.in
#  ${CONFIG_PATH}/config.h
#)

# 配置运行时链接目录
SET (CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN/../lib/'" )

add_subdirectory(src)

# 配置安装目录
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/run)

# 安装运行时脚本
#install(FILES )

# 配置安装包构建
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_DIRECTORY ${PROJECT_SOURCE_DIR}/pack)
set(CPACK_PACKAGE_FILE_LICENSE
  ${CMAKE_CURRENT_SOURCE_DIR}/License.txt
)

set(CPACK_PACKAGE_VERSION_MAJOR ${MassUtils_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${MassUtils_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${MassUtils_VERSION_PATCH})

set(PROJECT_VERSION_FULL 	
    ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(CPACK_SOURCE_PACKAGE_FILE_NAME
    mass-utils-v${PROJECT_VERSION_FULL})
set(CPACK_PACKAGE_FILE_NAME
    mass-utils-v${PROJECT_VERSION_FULL}-bin)

SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_GENERATOR "TGZ")

SET(CPACK_SOURCE_IGNORE_FILES
  ${PROJECT_SOURCE_DIR}/build
  ${PROJECT_SOURCE_DIR}/pack
  ${PROJECT_SOURCE_DIR}/run
#   ${PROJECT_SOURCE_DIR}/3rdparty/apache-log4cxx*
#   ${PROJECT_SOURCE_DIR}/3rdparty/hiredis-master
#   ${PROJECT_SOURCE_DIR}/3rdparty/include
#   ${PROJECT_SOURCE_DIR}/3rdparty/lib
#   ${PROJECT_SOURCE_DIR}/3rdparty/pb
#   ${PROJECT_SOURCE_DIR}/3rdparty/protobuf*
)

include(CPack)
