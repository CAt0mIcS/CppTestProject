set(SRC
	"include/assimp/aabb.h"
	"include/assimp/ai_assert.h"   
	"include/assimp/anim.h"        
	"include/assimp/BaseImporter.h"
	"include/assimp/Bitmap.h"      
	"include/assimp/BlobIOSystem.h"
	"include/assimp/ByteSwapper.h" 
	"include/assimp/camera.h"      
	"include/assimp/cexport.h"     
	"include/assimp/cfileio.h"
	"include/assimp/cimport.h"
	"include/assimp/ColladaMetaData.h"
	"include/assimp/color4.h"
	"include/assimp/color4.inl"
	"include/assimp/commonMetaData.h"
	"include/assimp/config.h"
	"include/assimp/CreateAnimMesh.h"
	"include/assimp/DefaultIOStream.h"
	"include/assimp/DefaultIOSystem.h"
	"include/assimp/DefaultLogger.hpp"
	"include/assimp/Defines.h"
	"include/assimp/defs.h"
	"include/assimp/Exceptional.h"
	"include/assimp/Exporter.hpp"
	"include/assimp/fast_atof.h"
	"include/assimp/GenericProperty.h"
	"include/assimp/Hash.h"
	"include/assimp/Importer.hpp"
	"include/assimp/importerdesc.h"
	"include/assimp/IOStream.hpp"
	"include/assimp/IOStreamBuffer.h"
	"include/assimp/IOSystem.hpp"
	"include/assimp/light.h"
	"include/assimp/LineSplitter.h"
	"include/assimp/LogAux.h"
	"include/assimp/Logger.hpp"
	"include/assimp/LogStream.hpp"
	"include/assimp/material.h"
	"include/assimp/material.inl"
	"include/assimp/MathFunctions.h"
	"include/assimp/matrix3x3.h"
	"include/assimp/matrix3x3.inl"
	"include/assimp/matrix4x4.h"
	"include/assimp/matrix4x4.inl"
	"include/assimp/MemoryIOWrapper.h"
	"include/assimp/mesh.h"
	"include/assimp/metadata.h"
	"include/assimp/NullLogger.hpp"
	"include/assimp/ParsingUtils.h"
	"include/assimp/pbrmaterial.h"
	"include/assimp/postprocess.h"
	"include/assimp/Profiler.h"
	"include/assimp/ProgressHandler.hpp"
	"include/assimp/qnan.h"
	"include/assimp/quaternion.h"
	"include/assimp/quaternion.inl"
	"include/assimp/RemoveComments.h"
	"include/assimp/scene.h"
	"include/assimp/SceneCombiner.h"
	"include/assimp/SGSpatialSort.h"
	"include/assimp/SkeletonMeshBuilder.h"
	"include/assimp/SmallVector.h"
	"include/assimp/SmoothingGroups.h"
	"include/assimp/SmoothingGroups.inl"
	"include/assimp/SpatialSort.h"
	"include/assimp/StandardShapes.h"
	"include/assimp/StreamReader.h"
	"include/assimp/StreamWriter.h"
	"include/assimp/StringComparison.h"
	"include/assimp/StringUtils.h"
	"include/assimp/Subdivision.h"
	"include/assimp/texture.h"
	"include/assimp/TinyFormatter.h"
	"include/assimp/types.h"
	"include/assimp/vector2.h"
	"include/assimp/vector2.inl"
	"include/assimp/vector3.h"
	"include/assimp/vector3.inl"
	"include/assimp/version.h"
	"include/assimp/Vertex.h"
	"include/assimp/XmlParser.h"
	"include/assimp/XMLTools.h"
	"include/assimp/ZipArchiveIOSystem.h"
	"include/assimp/Compiler/poppack1.h"
	"include/assimp/Compiler/pstdint.h"
	"include/assimp/Compiler/pushpack1.h"
	"include/assimp/port/AndroidJNI/AndroidJNIIOSystem.h"
)


set(INC
	"include"
)

if(WIN32)
set(LIB
	"assimp-vc142-mt"
)
else()
set(LIB
	"libassimp.so"
)
endif()

add_library("assimp" INTERFACE)

add_custom_target("Iassimp" SOURCES "${SRC}")
set_target_properties("Iassimp" PROPERTIES FOLDER "Extern")

target_link_directories("assimp" INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/")
target_link_libraries("assimp" INTERFACE "${LIB}")
target_include_directories("assimp" INTERFACE "${INC}")
AddRecursiveFilters("${SRC}")


