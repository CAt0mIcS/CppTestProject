set(SRC
    "src/Exception.cpp"
    "src/Random.cpp"
    "src/TypeConvert.cpp"
    "src/utpch.cpp"
    "src/utpch.h"

    "include/RlUtil/CoreConfig.h"
    "include/RlUtil/Exception.h"
    "include/RlUtil/Random.h"
    "include/RlUtil/RlUBase.h"
    "include/RlUtil/Serialize.h"
    "include/RlUtil/TypeConvert.h"
)

set(PUBLIC_INC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(PRIVATE_INC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
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

target_precompile_headers("RlUtilities" PRIVATE "src/utpch.h")
set_target_properties("RlUtilities" PROPERTIES FOLDER "RlUtilities")
AddRecursiveFilters("${SRC}")
