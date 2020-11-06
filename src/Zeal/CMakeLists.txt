
# List of all source files
set(SRC
    "main.cpp"
    "Sandbox.cpp"
    "Sandbox.h"
)

# List of all include directories
set(INC
    "${CMAKE_SOURCE_DIR}/src/Zeal"
    "${CMAKE_SOURCE_DIR}/src/"
)

# List of all libraries to link to
set(LIB
    "Reyal"
)

add_executable("Zeal" "${SRC}")
target_include_directories("Zeal" PRIVATE "${INC}")
target_link_libraries("Zeal" PRIVATE "${LIB}")

add_dependencies("Zeal" "Reyal" "ZealGUILayer")
AddRecursiveFilters("${SRC}")




