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
    "${CMAKE_CURRENT_SOURCE_DIR}/../Reyal/include"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
    "Reyal"
    "RlDebug"
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
target_precompile_headers("ZealGUILayer" PRIVATE "src/glpch.h")
add_dependencies("ZealGUILayer" "Reyal")
AddRecursiveFilters("${SRC}")
