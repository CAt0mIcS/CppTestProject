set(SRC
    "include/RlRender/Platform/D3D11/D3D11RendererAPI.cpp"
    "include/RlRender/Renderer3D.cpp"
    "include/RlRender/RendererAPI.cpp"
    "include/RlRender/rlrpch.cpp"
    "include/RlRender/rlrpch.h"

    "include/RlRender/Platform/D3D11/D3D11RendererAPI.h"
    "include/RlRender/RendererAPI.h"
    "include/RlRender/Renderer3D.h"
    "include/RlRender/RlRBase.h"
    "include/RlRender/TypeDefines.h"
    "include/RlRender/RlRWin.h"

    "Shaders/PixelShader-p.hlsl"
    "Shaders/VertexShader-v.hlsl"
)

set(PUBLIC_INC
	"include"
)

set(PRIVATE_INC
    "include/RlRender"
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
target_include_directories("RlRender" PRIVATE "include/RlRender/rlrpch.h")

# Set Pixel and Vertex Shaders to compile in VS
if(MSVC)
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_TYPE Pixel)
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_MODEL 4.0_level_9_3)
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_TYPE Vertex)
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_MODEL 4.0_level_9_3)
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SETTINGS "$<$<CONFIG:DEBUG>:SourceProperty1=SourceProperty1Value>")
endif()

AddRecursiveFilters("${SRC}")
