cmake_minimum_required (VERSION 3.1)
project (CDLogger)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4512 /wd4100")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../include")
include(compilerflags)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../install")
file(GLOB CDLogger_INCLUDES "include/*.h")
install(FILES ${CDLogger_INCLUDES} DESTINATION include/CDLogger/)

add_subdirectory(src)
add_subdirectory(CDLoggerTester)
