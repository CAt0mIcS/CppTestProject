cmake_minimum_required(VERSION 3.16)

project("VulkanTesting")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include("CMake/Macros.cmake")

# ------------------------------------------------------------------------------------------------
# Set common options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)
set(ENABLE_SPVREMAPPER OFF CACHE BOOL "" FORCE)
set(ENABLE_HLSL OFF CACHE BOOL "" FORCE)
set(ENABLE_AMD_EXTENSIONS OFF CACHE BOOL "" FORCE)
set(ENABLE_NV_EXTENSIONS CACHE BOOL "" FORCE)
set(SKIP_GLSLANG_INSTALL ON CACHE BOOL "" FORCE)
set(ENABLE_OPT ON CACHE BOOL "" FORCE)
set(ENABLE_CTEST OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)


add_subdirectory("Extern")

set(SRC
    "Sources/Utils/Logger/Formatters/RBracketFormatter.h"
    "Sources/Utils/Logger/Formatters/RDateTimeFormatter.h"
    "Sources/Utils/Logger/Formatters/RFormatter.h"
    "Sources/Utils/Logger/Formatters/RLogLevelFormatter.h"
    "Sources/Utils/Logger/Formatters/RNullTerminatorFormatter.h"
    "Sources/Utils/Logger/Loggers/RBaseLogger.h"
    "Sources/Utils/Logger/Loggers/RFileLogger.h"
    "Sources/Utils/Logger/Loggers/RConsoleLogger.h"
    "Sources/Utils/RAssert.h"
    "Sources/Utils/RException.h"
    "Sources/Utils/RException.cpp"
    "Sources/Utils/RString.h"
    "Sources/Utils/RString.cpp"
    "Sources/Utils/RLogger.h"
    "Sources/Utils/RLogger.cpp"

    # Graphics
    "Sources/Graphics/Camera.h"
    "Sources/Graphics/Camera.cpp"
    "Sources/Graphics/Graphics.h"
    "Sources/Graphics/Graphics.cpp"
    
    "Sources/Graphics/Core/Codex.h"
    "Sources/Graphics/Core/UniformHandler.h"
    "Sources/Graphics/Core/UniformHandler.cpp"
    "Sources/Graphics/Core/DescriptorsHandler.h"
    "Sources/Graphics/Core/DescriptorsHandler.cpp"
    "Sources/Graphics/Core/Entity.h"
    "Sources/Graphics/Core/Scene.h"
    "Sources/Graphics/Core/Scene.cpp"

    "Sources/Graphics/Vulkan/VulkanInstance.h"
    "Sources/Graphics/Vulkan/VulkanInstance.cpp"
    "Sources/Graphics/Vulkan/PhysicalDevice.h"
    "Sources/Graphics/Vulkan/PhysicalDevice.cpp"
    "Sources/Graphics/Vulkan/LogicalDevice.h"
    "Sources/Graphics/Vulkan/LogicalDevice.cpp"
    "Sources/Graphics/Vulkan/Surface.h"
    "Sources/Graphics/Vulkan/Surface.cpp"
    "Sources/Graphics/Vulkan/Framebuffer.h"
    "Sources/Graphics/Vulkan/Framebuffer.cpp"
    "Sources/Graphics/Vulkan/Swapchain.h"
    "Sources/Graphics/Vulkan/Swapchain.cpp"
    "Sources/Graphics/Vulkan/Descriptor.h"
    "Sources/Graphics/Vulkan/Descriptor.cpp"
    "Sources/Graphics/Vulkan/Shader.h"
    "Sources/Graphics/Vulkan/Shader.cpp"
    "Sources/Graphics/Vulkan/Commands/CommandBuffer.h"
    "Sources/Graphics/Vulkan/Commands/CommandBuffer.cpp"
    "Sources/Graphics/Vulkan/Commands/CommandPool.h"
    "Sources/Graphics/Vulkan/Commands/CommandPool.cpp"
    "Sources/Graphics/Vulkan/Images/ImageView.h"
    "Sources/Graphics/Vulkan/Images/ImageView.cpp"
    "Sources/Graphics/Vulkan/Images/Image.h"
    "Sources/Graphics/Vulkan/Images/Image.cpp"
    "Sources/Graphics/Vulkan/Bindable.h"
    "Sources/Graphics/Vulkan/Pipeline.h"
    "Sources/Graphics/Vulkan/GraphicsPipeline.h"
    "Sources/Graphics/Vulkan/GraphicsPipeline.cpp"
    "Sources/Graphics/Vulkan/Buffer.h"
    "Sources/Graphics/Vulkan/Buffer.cpp"
    "Sources/Graphics/Vulkan/VertexBuffer.h"
    "Sources/Graphics/Vulkan/VertexBuffer.cpp"
    "Sources/Graphics/Vulkan/IndexBuffer.h"
    "Sources/Graphics/Vulkan/IndexBuffer.cpp"
    "Sources/Graphics/Vulkan/UniformBuffer.h"
    "Sources/Graphics/Vulkan/UniformBuffer.cpp"
    "Sources/Graphics/Vulkan/Renderpass/Renderpass.h"
    "Sources/Graphics/Vulkan/Renderpass/Renderpass.cpp"
    "Sources/Graphics/Vulkan/Renderpass/Subpass.h"
    "Sources/Graphics/Vulkan/Renderpass/Subpass.cpp"
    "Sources/Graphics/Vulkan/Renderpass/Attachment.h"
    "Sources/Graphics/Vulkan/Renderpass/Attachment.cpp"
    "Sources/Graphics/Vulkan/Textures/Sampler.h"
    "Sources/Graphics/Vulkan/Textures/Sampler.cpp"

    "Sources/Graphics/Primitives/Drawable.h"
    "Sources/Graphics/Primitives/Drawable.cpp"
    "Sources/Graphics/Primitives/Triangle.h"
    "Sources/Graphics/Primitives/Triangle.cpp"
    "Sources/Graphics/Primitives/Square.h"
    "Sources/Graphics/Primitives/Square.cpp"
    "Sources/Graphics/Primitives/Cube.h"
    "Sources/Graphics/Primitives/Cube.cpp"
    "Sources/Graphics/Primitives/Model.h"
    "Sources/Graphics/Primitives/Model.cpp"
    "Sources/Graphics/Primitives/Mesh.h"
    "Sources/Graphics/Primitives/Mesh.cpp"
    "Sources/Graphics/Primitives/PointLight.h"
    "Sources/Graphics/Primitives/PointLight.cpp"
    "Sources/Graphics/Primitives/TexturedPlane.h"
    "Sources/Graphics/Primitives/TexturedPlane.cpp"


    "Sources/main.cpp"
    "Sources/Window.h"
    "Sources/Window.cpp"
    "Sources/pch.h"
    "Sources/Components.h"
    "Sources/Vertex.h"
)

set(SHADERS
    "${CMAKE_SOURCE_DIR}/Resources/Shaders/VertexShader.vert"
    "${CMAKE_SOURCE_DIR}/Resources/Shaders/FragmentShader.frag"
    "${CMAKE_SOURCE_DIR}/Resources/Shaders/PhongModelShader.vert"
    "${CMAKE_SOURCE_DIR}/Resources/Shaders/PhongModelShader.frag"
    "${CMAKE_SOURCE_DIR}/Resources/Shaders/FlatTextureShader.vert"
    "${CMAKE_SOURCE_DIR}/Resources/Shaders/FlatTextureShader.frag"
)

find_package("Vulkan" REQUIRED FATAL_ERROR)

set(INC
    "${Vulkan_INCLUDE_DIRS}"
    "${CMAKE_SOURCE_DIR}/Extern/glfw/include"
    "${CMAKE_SOURCE_DIR}/Sources"
    "${CMAKE_SOURCE_DIR}/Extern/entt/src"
    "${CMAKE_SOURCE_DIR}/Extern/"
)

set(LIB
    "${Vulkan_LIBRARIES}"
    "glfw"
    "glm"
    "glslang"
    "SPIRV"
    "assimp"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable("VulkanTesting" "${SRC}" "${SHADERS}")
target_include_directories("VulkanTesting" PRIVATE "${INC}")
target_link_libraries("VulkanTesting" PRIVATE "${LIB}")
target_precompile_headers("VulkanTesting" PRIVATE "Sources/pch.h")

# ------------------------------------------------------------------------------------------------
# Set the startup project for VS
if(MSVC)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "VulkanTesting")
endif()

# Preserves directory structure by creating Visual Studio filters for folders
foreach(source IN ITEMS ${SRC})
    get_filename_component(sourcePath "${source}" PATH)
    if(IS_ABSOLUTE source)
        file(RELATIVE_PATH relSourcePath "${_src_root_path}" "${sourcePath}")
    else()
        set(relSourcePath "${sourcePath}")
    endif()

    string(REPLACE "/" "\\" groupPath "${relSourcePath}")
    source_group("${groupPath}" FILES "${source}")
endforeach()


SetOutputDirectory("VulkanTesting" "")
SetWorkingDirectory("VulkanTesting" "${CMAKE_SOURCE_DIR}/")

# -------------------------------------------------------------------------------
# Recompile shaders automatically
CompileGLSLShaders("VulkanTesting" "${SHADERS}" "CustomShaderCompiler")

if(UNIX)
add_custom_command(TARGET "VulkanTesting" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/Resources>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/Resources>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/Resources>"
)
endif()
