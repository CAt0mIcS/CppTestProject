set(SRC
    "GUILayer.h"
    "GUILayer.cpp"
)

set(INC
    "../"
)

set(LIB
    "Reyal"
)

add_library("ZealGUILayer" "${SRC}")
target_include_directories("ZealGUILayer" PRIVATE "${INC}")
target_link_libraries("ZealGUILayer" PRIVATE "${LIB}")

set_target_properties("ZealGUILayer" PROPERTIES FOLDER "LayerExtensions")
add_dependencies("ZealGUILayer" "Reyal")