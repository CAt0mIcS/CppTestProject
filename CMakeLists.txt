cmake_minimum_required(VERSION 3.18.3)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    if(NOT DEFINED IN_SOURCE_BUILD)
        message(FATAL_ERROR
          "CMake generation not allowed within the source directory!"
          "\n Remove \"${CMAKE_SOURCE_DIR}/CMakeCache.txt\" and try again from another folder, e.g.:"
          "\n "
          "\n rm CMakeCache.txt"
          "\n mkdir build"
          "\n cd build"
          "\n cmake ../"
          "\n "
          "\n Alternately define WITH_IN_SOURCE_BUILD to force this option (not recommended!)"
        )
    endif()
endif()


# Set the startup project for VS (TODO: Only for VS)
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "Zeal")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

include("BuildFiles/macros.cmake")
include("BuildFiles/functions.cmake")

# Initialize the Solution Project
project("NodePlanningEditor")

# Redirect Output Files
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" CACHE INTERNAL "" FORCE)


add_subdirectory("src/")






