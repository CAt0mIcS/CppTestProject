set(SRC
	"include/Reyal/Application.cpp"
	"include/Reyal/Core/ThreadPool.cpp"
	"include/Reyal/Core/Vec2.cpp"
	"include/Reyal/Core/ZRect.cpp"
	"include/Reyal/Input/Keyboard.cpp"
	"include/Reyal/Input/Mouse.cpp"
	"include/Reyal/Layers/Layer.cpp"
	"include/Reyal/Layers/LayerStack.cpp"
	"include/Reyal/Widgets/ZWidget.cpp"
	"include/Reyal/Window/Window.cpp"
	"include/Reyal/rlpch.cpp"
	"include/Reyal/rlpch.h"
	
	"include/Reyal/Core/ThreadPool.h"
	"include/Reyal/Layers/LayerStack.h"
	"include/Reyal/Core/Queue.h"
	"include/Reyal/EntryPoint.h"
	"include/Reyal/Window/BaseWindow.h"
	"include/Reyal/Core/Vec2.h"
	"include/Reyal/Core/ZRect.h"
	"include/Reyal/Events/Event.h"
	"include/Reyal/Events/ApplicationEvent.h"
	"include/Reyal/Events/KeyboardEvent.h"
	"include/Reyal/Events/MouseEvent.h"
	"include/Reyal/Input/Keyboard.h"
	"include/Reyal/Input/Mouse.h"
	"include/Reyal/Layers/Layer.h"
	"include/Reyal/Widgets/ZWidget.h"
	"include/Reyal/Window/Window.h"
	"include/Reyal/Application.h"
	"include/Reyal/Reyal.h"
	"include/Reyal/RlBase.h"
	"include/Reyal/RlWin.h"
)

set(PUBLIC_INC
	"include"
)

set(PRIVATE_INC
	"include/Reyal"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
	"RlRender"
)

add_library("Reyal" SHARED "${SRC}")

# Setting the include directory public will add the include directory to all Projects depending on it
target_include_directories("Reyal" 
	PUBLIC 
		"${PUBLIC_INC}" 
	PRIVATE 
		"${PRIVATE_INC}"
)

target_link_libraries("Reyal" 
	PRIVATE 
		"${PRIVATE_LIB}"
	PUBLIC
		"${PUBLIC_LIB}"
)

target_compile_definitions("Reyal" PRIVATE "RL_BUILD")
add_dependencies("Reyal" "RlRender")
target_precompile_headers("Reyal" PRIVATE "include/Reyal/rlpch.h")

AddRecursiveFilters("${SRC}")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND RL_BUILD_TESTS)
	add_subdirectory("tests")
endif()