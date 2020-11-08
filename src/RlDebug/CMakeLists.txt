set(SRC
    "include/RlDebug/Instrumentor.cpp"
    "include/RlDebug/ReyalLogger.cpp"
    "include/RlDebug/rldpch.cpp"
    "include/RlDebug/rldpch.h"

    "include/RlDebug/Instrumentor.h"
    "include/RlDebug/ReyalLogger.h"
    "include/RlDebug/RlAssert.h"
    "include/RlDebug/RlDBase.h"
)

set(PUBLIC_INC
    "include"
)

set(PRIVATE_INC
	"include/RlDebug"
)

set(PUBLIC_LIB
    "RlUtilities"
    "RlLogger"
)

set(PRIVATE_LIB
    ""
)


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
target_precompile_headers("RlDebug" PRIVATE "include/RlDebug/rldpch.h")
add_dependencies("RlDebug" "RlUtilities")
AddRecursiveFilters("${SRC}")
