set(SRC
	"Core/Application.cpp"
	"Core/Application.h"
	"RlBase.h"
)

set(INC
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

set(LIB
	"RlDebug"
)

add_library("Reyal" SHARED "${SRC}")

# Setting the include directory public will add the include directory to all Projects depending on it
target_include_directories("Reyal" PRIVATE "${INC}")
target_link_libraries("Reyal" PRIVATE "${LIB}")

target_compile_definitions("Reyal" PUBLIC "RL_BUILD")
add_dependencies("Reyal" "RlRender")
AddRecursiveFilters("${SRC}")
