file(GLOB SRCS
    "*.h"
    "*.cpp"
)

add_executable(CDLoggerTester ${SRCS})

target_link_libraries(CDLoggerTester CDLogger)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(CDLoggerTester pthread)
endif()
include_directories(../include)
set_property(TARGET CDLoggerTester PROPERTY CXX_STANDARD 11)
