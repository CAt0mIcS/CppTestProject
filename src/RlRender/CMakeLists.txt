set(SRC
    "src/Platform/D3D11/D3D11RendererAPI.cpp"
    "src/Platform/D3D11/D3D11RendererAPI.h"
    "src/Renderer3D.cpp"
    "src/RendererAPI.cpp"
    "src/RendererAPI.h"
    "src/rlrpch.cpp"
    "src/rlrpch.h"
)

set(PUBLIC_INC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(PRIVATE_INC
	"include/RlRender"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../RlDebug/include"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
    "RlDebug"
    "D3D11"
    "DXGI"
    "D3DCompiler"
)


add_library("RlRender" SHARED "${SRC}")

target_include_directories("RlRender"
    PRIVATE
        "${PRIVATE_INC}"
    PUBLIC
        "${PUBLIC_INC}"
)
target_link_libraries("RlRender"
    PRIVATE
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

add_dependencies("RlRender" "RlDebug")
target_compile_definitions("RlRender" PRIVATE "RLR_BUILD")

AddRecursiveFilters("${SRC}")
