cmake_minimum_required(VERSION 3.16)

project("VulkanTesting")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set all output directories of a specific target
macro(SetOutputDirectory target dir)
    if(MSVC)
        foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
            set(OUTCFGLOW "${OUTPUTCONFIG}")
            string( TOUPPER "${OUTPUTCONFIG}" OUTPUTCONFIG )

            set(${target}_OUTPUTPATH_${OUTCFGLOW} "${CMAKE_BINARY_DIR}/bin/${OUTCFGLOW}/${dir}")

            set_target_properties("${target}" PROPERTIES 
                LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${${target}_OUTPUTPATH_${OUTCFGLOW}}"
                RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${${target}_OUTPUTPATH_${OUTCFGLOW}}"
                ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${${target}_OUTPUTPATH_${OUTCFGLOW}}"
            )
        endforeach()
    else()
        set(${target}_OUTPUTPATH_${CMAKE_BUILD_TYPE} "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${dir}")
        set_target_properties("${target}" PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${${target}_OUTPUTPATH_${CMAKE_BUILD_TYPE}}"
            LIBRARY_OUTPUT_DIRECTORY "${${target}_OUTPUTPATH_${CMAKE_BUILD_TYPE}}"
            RUNTIME_OUTPUT_DIRECTORY "${${target}_OUTPUTPATH_${CMAKE_BUILD_TYPE}}"
        )
    endif()
endmacro()

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
    "Sources/Utils/Logger/Formatters/RlogLevelFormatter.h"
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

    "Sources/main.cpp"
    "Sources/Window.h"
    "Sources/Window.cpp"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package("Vulkan" REQUIRED FATAL_ERROR)

add_executable("VulkanTesting" "${SRC}")
target_include_directories("VulkanTesting" PRIVATE "${Vulkan_INCLUDE_DIRS}" "${CMAKE_SOURCE_DIR}/Extern/glfw/include")
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