add_subdirectory("glfw")
add_subdirectory("glm")
add_subdirectory("glslang")
add_subdirectory("assimp")

SetOutputDirectory("glfw" "")

SetOutputDirectory("assimp" "")
SetOutputDirectory("assimp_cmd" "")
SetOutputDirectory("UpdateAssimpLibsDebugSymbolsAndDLLs" "")
SetOutputDirectory("zlibstatic" "")

SetOutputDirectory("glslang" "")
SetOutputDirectory("OGLCompiler" "")
SetOutputDirectory("OSDependent" "")
SetOutputDirectory("SPIRV" "")
SetOutputDirectory("GenericCodeGen" "")
SetOutputDirectory("MachineIndependent" "")

set_target_properties("glfw" PROPERTIES FOLDER "Extern")
set_target_properties("glm" PROPERTIES FOLDER "Extern")

set_target_properties("assimp" PROPERTIES FOLDER "Extern")
set_target_properties("assimp_cmd" PROPERTIES FOLDER "Extern")
set_target_properties("UpdateAssimpLibsDebugSymbolsAndDLLs" PROPERTIES FOLDER "Extern")
set_target_properties("zlibstatic" PROPERTIES FOLDER "Extern")

set_target_properties("glslang" PROPERTIES FOLDER "Extern/glslang")
set_target_properties("OGLCompiler" PROPERTIES FOLDER "Extern/glslang")
set_target_properties("OSDependent" PROPERTIES FOLDER "Extern/glslang")
set_target_properties("SPIRV" PROPERTIES FOLDER "Extern/glslang")
set_target_properties("GenericCodeGen" PROPERTIES FOLDER "Extern/glslang")
set_target_properties("MachineIndependent" PROPERTIES FOLDER "Extern/glslang")