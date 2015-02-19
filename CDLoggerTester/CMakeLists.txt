file(GLOB SRCS
    "*.h"
    "*.cpp"
)

add_executable(CDLoggerTester ${SRCS})

target_link_libraries(CDLoggerTester CDLogger)
include_directories(../include)
set_property(TARGET CDLoggerTester PROPERTY CXX_STANDARD 11)
