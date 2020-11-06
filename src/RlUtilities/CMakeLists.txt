set(SRC
    "Log.h"
    "Log.cpp"
)

set(INC
    ""
)

set(LIB
    ""
)

add_library("RlUtilities" "${SRC}")
target_include_directories("RlUtilities" PRIVATE "${INC}")

set_target_properties("RlUtilities" PROPERTIES FOLDER "RlUtilities")