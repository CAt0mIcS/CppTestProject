
# List of all source files
set(SRC
    "src/pch.cpp"
    "src/pch.h"
    "src/Sandbox.cpp"
    "src/Sandbox.h"
)

set(PUBLIC_INC
	""
)

set(PRIVATE_INC
    ""
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
    "Reyal"
    "RlDebug"
)

add_executable("Zeal" WIN32 "${SRC}")
target_include_directories("Zeal"
    PRIVATE
        "${PRIVATE_INC}"
    PUBLIC
        "${PUBLIC_INC}"
)
target_link_libraries("Zeal"
    PRIVATE
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

add_dependencies("Zeal" "Reyal" "ZealGUILayer")
target_precompile_headers("Zeal" PRIVATE "src/pch.h")
AddRecursiveFilters("${SRC}")




