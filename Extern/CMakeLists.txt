add_subdirectory("glfw")
add_subdirectory("glm")

SetOutputDirectory("glfw" "")

set_target_properties("glfw" PROPERTIES FOLDER "Extern")
set_target_properties("glm" PROPERTIES FOLDER "Extern")