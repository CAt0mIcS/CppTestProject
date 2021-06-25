set(SRC
	"Lib2.h"
	"Lib2.cpp"
)

set(LIB
	""
)

set(INC
	""
)

add_library("Lib2" SHARED "${SRC}")
target_include_directories("Lib2" PRIVATE "${INC}")
target_link_libraries("Lib2" PRIVATE "${LIB}")
target_compile_definitions("Lib1" PRIVATE "LIB1_BUILD")

SetOutputDirectory("Lib2" "")
SetWorkingDirectoryPlane("Lib2" "${CMAKE_SOURCE_DIR}")
AddRecursiveFilters("${SRC}")