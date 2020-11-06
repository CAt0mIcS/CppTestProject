set(SRC
    "RlLog.h"
    "RlLog.cpp"
)

set(INC
    "../"
)

set(LIB
    "RlUtilities"
)


add_library("RlLogger" "${SRC}")
target_include_directories("RlLogger" PRIVATE "${INC}")
target_link_libraries("RlLogger" PRIVATE "${LIB}")

set_target_properties("RlLogger" PROPERTIES FOLDER "RlUtilities")
add_dependencies("RlLogger" "RlUtilities")