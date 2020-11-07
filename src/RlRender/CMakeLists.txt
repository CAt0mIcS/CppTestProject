set(SRC
    "src/Platform/D3D11/D3D11RendererAPI.cpp"
    "src/Renderer3D.cpp"
    "src/RendererAPI.cpp"
    "src/rlrpch.cpp"
    "src/rlrpch.h"

    "include/RlRender/Platform/D3D11/D3D11RendererAPI.h"
    "include/RlRender/RendererAPI.h"
    "include/RlRender/Renderer3D.h"
    "include/RlRender/RlRBase.h"
    "include/RlRender/TypeDefines.h"
    "include/RlRender/RlRWin.h"
)

set(PUBLIC_INC
	"include"
)

set(PRIVATE_INC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(PUBLIC_LIB
    "RlDebug"
)

set(PRIVATE_LIB
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
target_include_directories("RlRender" PRIVATE "src/rlrpch.h")

AddRecursiveFilters("${SRC}")
