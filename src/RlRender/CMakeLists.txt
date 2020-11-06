set(SRC
    "Renderer3D.h"
    "Renderer3D.cpp"
)

set(INC
    "../"
)

set(LIB
    "RlDebug"
)


add_library("RlRender" "${SRC}")
target_include_directories("RlRender" PRIVATE "${INC}")
target_link_libraries("RlRender" PRIVATE "${LIB}")
target_link_directories("RlRender" PRIVATE "${CMAKE_BINARY_DIR}/lib")

add_dependencies("RlRender" "RlDebug")