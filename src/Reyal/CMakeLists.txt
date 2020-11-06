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
target_include_directories("Reyal" PRIVATE "${INC}")
target_link_libraries("Reyal" PRIVATE "${LIB}")
target_link_directories("Reyal" PRIVATE "${CMAKE_BINARY_DIR}/lib")

target_compile_definitions("Reyal" PUBLIC "RL_BUILD")
add_dependencies("Reyal" "RlRender")
