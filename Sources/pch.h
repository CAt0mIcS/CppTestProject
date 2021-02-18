#pragma once

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#if !(GLM_COMPILER & GLM_COMPILER_CUDA)
	#include <glm/gtx/string_cast.hpp>
#endif

#include <entt/entt.hpp>

#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <array>
#include <memory>
#include <filesystem>

