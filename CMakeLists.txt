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
    "Sources/Graphics/Graphics.h"
    "Sources/Graphics/Graphics.cpp"
    "Sources/Graphics/VulkanInstance.h"
    "Sources/Graphics/VulkanInstance.cpp"
    "Sources/Graphics/PhysicalDevice.h"
    "Sources/Graphics/PhysicalDevice.cpp"
    "Sources/Graphics/LogicalDevice.h"
    "Sources/Graphics/LogicalDevice.cpp"
    "Sources/Graphics/Surface.h"
    "Sources/Graphics/Surface.cpp"
    "Sources/Graphics/Framebuffer.h"
    "Sources/Graphics/Framebuffer.cpp"
    "Sources/Graphics/Swapchain.h"
    "Sources/Graphics/Swapchain.cpp"
    "Sources/Graphics/Commands/CommandBuffer.h"
    "Sources/Graphics/Commands/CommandBuffer.cpp"
    "Sources/Graphics/Commands/CommandPool.h"
    "Sources/Graphics/Commands/CommandPool.cpp"
    "Sources/Graphics/Images/ImageView.h"
    "Sources/Graphics/Images/ImageView.cpp"

    "Sources/Graphics/Bindables/Pipeline.h"
    "Sources/Graphics/Bindables/Pipeline.cpp"
    "Sources/Graphics/Bindables/Buffer.h"
    "Sources/Graphics/Bindables/Buffer.cpp"
    "Sources/Graphics/Bindables/VertexBuffer.h"
    "Sources/Graphics/Bindables/VertexBuffer.cpp"
    "Sources/Graphics/Bindables/IndexBuffer.h"
    "Sources/Graphics/Bindables/IndexBuffer.cpp"

    "Sources/Graphics/Drawables/Drawable.h"
    "Sources/Graphics/Drawables/Drawable.cpp"
    "Sources/Graphics/Drawables/Triangle.h"
    "Sources/Graphics/Drawables/Triangle.cpp"
    "Sources/Graphics/Drawables/Quad.h"
    "Sources/Graphics/Drawables/Quad.cpp"

    "Sources/Graphics/Renderpass/Renderpass.h"
    "Sources/Graphics/Renderpass/Renderpass.cpp"
    "Sources/Graphics/Renderpass/Subpass.h"
    "Sources/Graphics/Renderpass/Subpass.cpp"
    "Sources/Graphics/Renderpass/Attachment.h"
    "Sources/Graphics/Renderpass/Attachment.cpp"

    "Sources/main.cpp"
    "Sources/Window.h"
    "Sources/Window.cpp"
)

set(SHADERS
    "${CMAKE_SOURCE_DIR}/Resources/Shaders/VertexShader.vert"
    "${CMAKE_SOURCE_DIR}/Resources/Shaders/FragmentShader.frag"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package("Vulkan" REQUIRED FATAL_ERROR)

add_executable("VulkanTesting" "${SRC}" "${SHADERS}")
target_include_directories("VulkanTesting" PRIVATE "${Vulkan_INCLUDE_DIRS}" "${CMAKE_SOURCE_DIR}/Extern/glfw/include" "Sources")
target_link_libraries("VulkanTesting" PRIVATE "${Vulkan_LIBRARIES}" "glfw")

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
