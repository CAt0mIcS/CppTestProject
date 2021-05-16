set(SRC
	"pch.h"
	"main.cpp"
)

set(monoPath "C:/Program Files/Mono/lib")

set(LIB
	"${monoPath}/mono-2.0-sgen.lib"
)


set(INC
	"C:/Program Files/Mono/include/mono-2.0"
)


add_executable("TestProject" "${SRC}")
target_include_directories("TestProject" PRIVATE "${INC}")
target_link_libraries("TestProject" PRIVATE "${LIB}")
target_precompile_headers("TestProject" PRIVATE "pch.h")

SetOutputDirectory("TestProject" "")
SetWorkingDirectoryPlane("TestProject" "${CMAKE_SOURCE_DIR}")
AddRecursiveFilters("${SRC}")

if(WIN32)
add_custom_command(TARGET "TestProject" PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	"${monoPath}/../bin/mono-2.0-sgen.dll"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo>"
)
endif()