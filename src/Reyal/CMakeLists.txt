set(SRC
	"Core/Application.cpp"
	"Core/Application.h"
	"RlBase.h"
)

set(INC
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

set(LIB
	""
)

AddLib("Reyal" "${SRC}" "${INC}" "${LIB}")

target_compile_definitions("Reyal" PUBLIC "RL_BUILD")
