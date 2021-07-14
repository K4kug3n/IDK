#pragma once

#ifndef IDK_DEVICE_HPP
#define IDK_DEVICE_HPP

#include <unordered_set>
#include <string>

#include <Renderer/Instance.hpp>
#include <Renderer/PhysicalDevice.hpp>

#include <vulkan/vulkan.h>

/*#if defined(VK_USE_PLATFORM_WIN32_KHR)
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
#endif*/

namespace Idk {

	class Device {
	public:
		Device(Instance const& instance);
		Device(Device const&) = delete;
		Device(Device&&) = delete;
		~Device();

		bool create(PhysicalDevice const& physicalDevice, VkDeviceCreateInfo const& infos);

		bool isValid() const;
		bool isLoadedExtension(std::string const& name) const;
		bool isLoadedLayer(std::string const& name) const;

		#define IDK_DEVICE_FUNCTION(fun) PFN_##fun fun;
		#include <Renderer/DeviceFunctions.inl>

		Device& operator=(Device const&) = delete;
		Device& operator=(Device&&) = delete;
	private:
		VkDevice m_device;
		Instance const& m_instance;
		std::unordered_set<std::string> m_extensions;
		std::unordered_set<std::string> m_layers;
	};
}

#endif
