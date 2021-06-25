set(SRC
	"Lib1.h"
	"Lib1.cpp"
)

set(LIB
	""
)

set(INC
	""
)

add_library("Lib1" SHARED "${SRC}")
target_include_directories("Lib1" PRIVATE "${INC}")
target_link_libraries("Lib1" PRIVATE "${LIB}")
target_compile_definitions("Lib1" PRIVATE "LIB1_BUILD")

SetOutputDirectory("Lib1" "")
SetWorkingDirectoryPlane("Lib1" "${CMAKE_SOURCE_DIR}")
AddRecursiveFilters("${SRC}")