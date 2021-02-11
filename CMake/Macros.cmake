# File contains all self-defined CMake functions

# Preserves directory structure by creating Visual Studio filters for folders
function(AddRecursiveFilters targetSrc)
    foreach(source IN ITEMS ${targetSrc})
        get_filename_component(sourcePath "${source}" PATH)
        if(IS_ABSOLUTE source)
            file(RELATIVE_PATH relSourcePath "${_src_root_path}" "${sourcePath}")
        else()
            set(relSourcePath "${sourcePath}")
        endif()

        string(REPLACE "/" "\\" groupPath "${relSourcePath}")
        source_group("${groupPath}" FILES "${source}")
    endforeach()

    GeneratePCHFilters()
endfunction()

# Add CMake-generated pch files into their separate filter
function(GeneratePCHFilters)
    get_directory_property(targets BUILDSYSTEM_TARGETS)
    set(configList "${CMAKE_CONFIGURATION_TYPES}")

    foreach(target "${targets}")
        foreach(configuration IN LISTS CMAKE_CONFIGURATION_TYPES ITEMS ${CMAKE_BUILD_TYPE})
            source_group("CMake" FILES "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir/${configuration}/cmake_pch.hxx")
        endforeach()
        source_group("CMake" FILES "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir/cmake_pch.cxx")
    endforeach()
endfunction()

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

# Set Working directory (VS ONLY)
function(SetWorkingDirectory target dir)
    if(dir STREQUAL "")
    set_target_properties(
        "${target}" PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$(Configuration)"
    )
    else()
    set_target_properties(
        "${target}" PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${dir}"
    )
    endif()
endfunction()

# Set specific glsl shaders to automatically compile to SpirV
# Use an optional 3rd argument to specify the name of the custom target created
function(CompileGLSLShaders target shaders)
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
      set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
    else()
      set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
    endif()

    foreach(GLSL ${shaders})
        get_filename_component(FILE_NAME "${GLSL}" NAME)
        get_filename_component(SHADEROUTDIR "${GLSL}" DIRECTORY)
        set(SPIRV "${SHADEROUTDIR}/${FILE_NAME}.spv")
        add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
        list(APPEND SPIRV_BINARY_FILES ${SPIRV})
    endforeach(GLSL)

    if("${ARGC}" STREQUAL "2")
        add_custom_target(
            "CompileShaders" 
            DEPENDS "${SPIRV_BINARY_FILES}"
        )
    add_dependencies("${target}" "CompileShaders")
    else()
        add_custom_target(
            "${ARGN}" 
            DEPENDS "${SPIRV_BINARY_FILES}"
        )
    add_dependencies("${target}" "${ARGN}")
    endif()

    endfunction()