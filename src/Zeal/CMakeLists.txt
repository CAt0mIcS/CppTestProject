
# List of all source files
set(SRC
    "src/pch.cpp"
    "src/pch.h"
    "src/Sandbox.cpp"
    "src/Sandbox.h"
)

set(PUBLIC_INC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(PRIVATE_INC
	"include/RlUtil"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Reyal/include"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
	"Reyal"
)

add_executable("Zeal" "${SRC}")
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
AddRecursiveFilters("${SRC}")




