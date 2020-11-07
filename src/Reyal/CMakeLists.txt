set(SRC
	"src/Application.cpp"
	"src/Core/ThreadPool.cpp"
	"src/Core/Vec2.cpp"
	"src/Core/ZRect.cpp"
	"src/Events/ApplicationEvent.h"
	"src/Events/KeyboardEvent.h"
	"src/Events/MouseEvent.h"
	"src/Input/Keyboard.cpp"
	"src/Input/Mouse.cpp"
	"src/Layers/Layer.cpp"
	"src/Layers/LayerStack.cpp"
	"src/Widgets/ZWidget.cpp"
	"src/Window/Window.cpp"
	"src/rlpch.cpp"
	"src/rlpch.h"
	"src/EntryPoint.h"
	
	"include/Reyal/Core/ThreadPool.h"
	"include/Reyal/Core/Queue.h"
	"include/Reyal/Window/BaseWindow.h"
	"include/Reyal/Core/Vec2.h"
	"include/Reyal/Core/ZRect.h"
	"include/Reyal/Events/Event.h"
	"include/Reyal/Input/Keyboard.h"
	"include/Reyal/Input/Mouse.h"
	"include/Reyal/Layers/Layer.h"
	"include/Reyal/Layers/LayerStack.h"
	"include/Reyal/Widgets/ZWidget.h"
	"include/Reyal/Window/Window.h"
	
	"include/Reyal/Application.h"
	"include/Reyal/Reyal.h"
	"include/Reyal/RlBase.h"
	"include/Reyal/RlWin.h"
)

set(PUBLIC_INC
	"include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../RlRender/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../RlDebug/include"
)

set(PRIVATE_INC
	"include/Reyal"
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${CMAKE_CURRENT_SOURCE_DIR}/../RlUtilities/include"
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
AddRecursiveFilters("${SRC}")