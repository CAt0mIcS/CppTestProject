set(SRC
    "src/glbase.h"
    "src/glpch.cpp"
    "src/glpch.h"
    "src/GUILayer.cpp"
    "src/GUILayer.h"
)

set(PUBLIC_INC
	""
)

set(PRIVATE_INC
	"include/ZealGUILayer"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Reyal/include"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
	"Reyal"
)

add_library("ZealGUILayer" SHARED "${SRC}")
target_include_directories("RlUtilities"
    PRIVATE
        "${PRIVATE_INC}"
    PUBLIC
        "${PUBLIC_INC}"
)
target_link_libraries("ZealGUILayer"
    PRIVATE
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

set_target_properties("ZealGUILayer" PROPERTIES FOLDER "LayerExtensions")
add_dependencies("ZealGUILayer" "Reyal")
AddRecursiveFilters("${SRC}")
