#pragma once

#ifndef IDK_VKUTIL_HPP
#define IDK_VKUTIL_HPP

#include <string>

#include <vulkan/vulkan.h>

namespace Idk {
	std::string toString(VkResult const& result);
}

#endif
