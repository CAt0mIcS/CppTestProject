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
endfunction()