set(SRC
	"pch.h"
	"main.cpp"
)

set(LIB
	""
)

set(INC
	"${CMAKE_SOURCE_DIR}/Lib1/"
	"${CMAKE_SOURCE_DIR}/Lib2/"
)

add_executable("TestProject" "${SRC}")
target_include_directories("TestProject" PRIVATE "${INC}")
target_link_libraries("TestProject" PRIVATE "${LIB}")
target_precompile_headers("TestProject" PRIVATE "pch.h")

SetOutputDirectory("TestProject" "")
SetWorkingDirectoryPlane("TestProject" "${CMAKE_SOURCE_DIR}")
AddRecursiveFilters("${SRC}")