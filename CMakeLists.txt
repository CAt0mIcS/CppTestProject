cmake_minimum_required(VERSION 3.18)

project("TestProject")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(SRC
	"src/main.cpp"
	"src/Window.cpp"
	"src/Window.h"
)

add_executable("Test" "${SRC}")
target_link_libraries("Test" PRIVATE "-lX11")
target_include_directories("Test" PRIVATE "${CMAKE_SOURCE_DIR}/src")

install(TARGETS "Test" DESTINATION "bin")
