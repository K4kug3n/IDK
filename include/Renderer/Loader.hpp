#pragma once

#ifndef IDK_LOADER_HPP
#define IDK_LOADER_HPP

#include <string>
#include <vector>

#include <Util/Lib.hpp>

#include <vulkan/vulkan.h>

#if defined(VK_USE_PLATFORM_WIN32_KHR)
	#include <Windows.h>
#elif defined(VK_USE_PLATFORM_XCB_KHR)
	#include <xcb/xcb.h>
	#include <dlfcn.h>
	#include <cstdlib>
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
	#include <dlfcn.h>
	#include <cstdlib>
#endif

namespace Idk {

	class Loader {
	public:
		Loader() = delete;
		~Loader() = delete;

		static bool initialize();
		static void unitialize();

		static std::vector<VkLayerProperties> enumerateLayerProperties();
		static std::vector<VkExtensionProperties> enumerateExtensionProperties(char const* layerName = nullptr);

		static PFN_vkVoidFunction getInstanceProcAddr(VkInstance const& instance, std::string const& name);
		
		#define IDK_GLOBAL_FUNCTION(fun) static PFN_##fun fun;
		#define IDK_GLOBAL_FUNCTION_OPTIONAL(fun) IDK_GLOBAL_FUNCTION(fun)
		#include <Renderer/GlobalFunctions.inl>
	private:
		static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

		static Lib m_vulkan;
	};
}

#endif
