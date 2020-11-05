function(AddLib libname libsrc libinc liblink)
    add_library("${libname}" "${libsrc}")
    target_include_directories("${libname}" PRIVATE "${libinc}")
    target_link_libraries("${libname}" "${liblink}")
endfunction()


function(AddExecutable execname execsrc execinc execlink)
    add_executable("${execname}" "${execsrc}")
    target_include_directories("${execname}" PRIVATE "${execinc}")
    target_link_libraries("${execname}" "${execlink}")
endfunction()