
file(GLOB LIBSRCS
    "../include/*.h"
    "*.h"
    "*.cpp"
)

add_library(CDLogger STATIC ${LIBSRCS})

include_directories(../include)

set_property(TARGET CDLogger PROPERTY CXX_STANDARD 11)
install(TARGETS CDLogger DESTINATION lib)

