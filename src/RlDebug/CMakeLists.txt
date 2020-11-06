set(SRC
    "RlAssert.h"
    "RlAssert.cpp"
)

set(INC
    "../"
)

set(LIB
    "RlUtilities"
    "RlLogger"
)


add_library("RlDebug" "${SRC}")
target_include_directories("RlDebug" PRIVATE "${INC}")
target_link_libraries("RlDebug" PRIVATE "${LIB}")

set_target_properties("RlDebug" PROPERTIES FOLDER "RlUtilities")
add_dependencies("RlDebug" "RlUtilities")
AddRecursiveFilters("${SRC}")
