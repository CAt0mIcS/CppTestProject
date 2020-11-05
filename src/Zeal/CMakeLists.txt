
# List of all source files
set(SRC
    "main.cpp"
)

# List of all include directories
set(INC
    "${CMAKE_SOURCE_DIR}/src/Zeal"
)

# List of all libraries to link to
set(LIB
    "Reyal"
)

AddExecutable("Zeal" "${SRC}" "${INC}" "${LIB}")



