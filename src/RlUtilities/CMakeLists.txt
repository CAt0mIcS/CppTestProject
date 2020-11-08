set(SRC
    "include/RlUtil/Exception.cpp"
    "include/RlUtil/Random.cpp"
    "include/RlUtil/TypeConvert.cpp"
    "include/RlUtil/utpch.cpp"
    "include/RlUtil/utpch.h"
    "include/RlUtil/PlatformDetection.h"

    "include/RlUtil/CoreConfig.h"
    "include/RlUtil/Exception.h"
    "include/RlUtil/Random.h"
    "include/RlUtil/RlUBase.h"
    "include/RlUtil/Serialize.h"
    "include/RlUtil/TypeConvert.h"
)

set(PUBLIC_INC
	"include"
)

set(PRIVATE_INC
    "include/RlUtil"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
	""
)

add_library("RlUtilities" STATIC "${SRC}")
target_include_directories("RlUtilities"
    PRIVATE
        "${PRIVATE_INC}"
    PUBLIC
        "${PUBLIC_INC}"
)
target_link_libraries("RlUtilities"
    PRIVATE
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

target_precompile_headers("RlUtilities" PRIVATE "include/RlUtil/utpch.h")
set_target_properties("RlUtilities" PROPERTIES FOLDER "RlUtilities")
AddRecursiveFilters("${SRC}")
