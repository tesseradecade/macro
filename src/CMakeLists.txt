set(SOURCES macro.c)
set(HEADERS macro.h)

add_library(macro SHARED ${SOURCES} ${HEADERS})

set_target_properties(macro PROPERTIES
    OUTPUT_NAME macro
    PUBLIC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/macro.h"
)
