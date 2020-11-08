cmake_minimum_required(VERSION 3.19.0)

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

# Required for Solution Folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set the startup project for VS (TODO: Only for VS)
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "Zeal")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Set default string stuff to be unicode
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

# Set C++ Version requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

include("BuildFiles/macros.cmake")

# Initialize the Solution Project
project("NodePlanningEditor")

# ------------------------------------------------
# Redirect Output Files (Remove VS /Debug folder)
# ------------------------------------------------

# For Single-Configuration Projects
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}")

# ------------------------------------------------
# Redirect Output Files for Multi-Configuration Projects
# ------------------------------------------------
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    set(OUTCFGLOW "${OUTPUTCONFIG}")
    string( TOUPPER "${OUTPUTCONFIG}" OUTPUTCONFIG )
    set("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}" "${CMAKE_BINARY_DIR}/bin/${OUTCFGLOW}-${CMAKE_SYSTEM_NAME}")
    set("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}" "${CMAKE_BINARY_DIR}/bin/${OUTCFGLOW}-${CMAKE_SYSTEM_NAME}")
    set("CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG}" "${CMAKE_BINARY_DIR}/bin/${OUTCFGLOW}-${CMAKE_SYSTEM_NAME}")
endforeach()


add_subdirectory("src/")









