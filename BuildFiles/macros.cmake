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
            source_group("CMake" FILES "${CMAKE_SOURCE_DIR}/build/src/${target}/CMakeFiles/${target}.dir/${configuration}/cmake_pch.hxx")
        endforeach()
        source_group("CMake" FILES "${CMAKE_SOURCE_DIR}/build/src/${target}/CMakeFiles/${target}.dir/cmake_pch.cxx")
    endforeach()
endfunction()
