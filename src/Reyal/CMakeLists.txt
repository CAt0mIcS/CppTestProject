set(SRC
	"src/Application.cpp"
	"src/Core/ThreadPool.cpp"
	"src/Core/Vec2.cpp"
	"src/Core/ZRect.cpp"
	"src/Input/Keyboard.cpp"
	"src/Input/Mouse.cpp"
	"src/Layers/Layer.cpp"
	"src/Layers/LayerStack.cpp"
	"src/Widgets/ZWidget.cpp"
	"src/Window/Window.cpp"
	"src/rlpch.cpp"
	"src/rlpch.h"
	
	# TODO: Move to be private
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
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
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
target_precompile_headers("Reyal" PRIVATE "src/rlpch.h")
AddRecursiveFilters("${SRC}")