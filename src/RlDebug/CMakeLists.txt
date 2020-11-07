set(SRC
    "src/Instrumentor.cpp"
    "src/ReyalLogger.cpp"
    "src/rldpch.cpp"
    "src/rldpch.h"

    "include/RlDebug/Instrumentor.h"
    "include/RlDebug/ReyalLogger.h"
    "include/RlDebug/RlAssert.h"
    "include/RlDebug/RlDBase.h"

    "RlDebug.def"
)

set(PUBLIC_INC
    "include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../RlLogger/include"
)

set(PRIVATE_INC
	"include/RlDebug"
	"${CMAKE_CURRENT_SOURCE_DIR}/../RlLogger/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
    "RlUtilities"
)

message(STATUS "${PUBLIC_INC}")
message(STATUS "${PRIVATE_INC}")

add_library("RlDebug" SHARED "${SRC}")
target_include_directories("RlDebug" 
    PUBLIC 
        "${PUBLIC_INC}"
    PRIVATE
        "${PRIVATE_INC}"
)
target_link_libraries("RlDebug" 
    PRIVATE 
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

set_target_properties("RlDebug" PROPERTIES FOLDER "RlUtilities")
target_compile_definitions("RlDebug" PRIVATE "RLD_BUILD")
add_dependencies("RlDebug" "RlUtilities")
AddRecursiveFilters("${SRC}")
