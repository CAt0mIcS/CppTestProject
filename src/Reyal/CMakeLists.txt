set(SRC
	"src/Core/Application.cpp"
	"src/Core/Application.h"

	"include/Reyal/RlBase.h"
	"include/Reyal/Reyal.h"
)

set(INC
	"include"
)

set(LIB
	"RlDebug"
)

add_library("Reyal" SHARED "${SRC}")

# Setting the include directory public will add the include directory to all Projects depending on it
target_include_directories("Reyal" 
	PUBLIC 
		"${INC}" 
	PRIVATE 
		"include/Reyal"
		"src"
)

target_link_libraries("Reyal" PRIVATE "${LIB}")

target_compile_definitions("Reyal" PRIVATE "RL_BUILD")
add_dependencies("Reyal" "RlRender")
AddRecursiveFilters("${SRC}")