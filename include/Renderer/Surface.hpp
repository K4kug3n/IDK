#pragma once

#ifndef IDK_SURFACE_HPP
#define IDK_SURFACE_HPP

#include <Renderer/Instance.hpp>

#include <vulkan/vulkan.h>

namespace Idk {

	class PhysicalDevice;
	struct WindowHandle;

	class Surface {
	public:
		Surface(Instance const& instance);
		Surface(Surface const&) = delete;
		~Surface();

		bool create(WindowHandle const& infos);
		//void destroy();

		bool getPhysicalDeviceSurfaceSupport(PhysicalDevice const& device, uint32_t queueFamilyIndex, bool & supported) const;
		bool getCapabilities(PhysicalDevice const& physicalDevice, VkSurfaceCapabilitiesKHR & capabilities) const;
		bool getFormats(PhysicalDevice const& physicalDevice, std::vector<VkSurfaceFormatKHR> & formats) const;
		bool getPresentModes(PhysicalDevice const& physicalDevice, std::vector<VkPresentModeKHR> & presentModes) const;

		bool isValid() const;

		VkSurfaceKHR const& operator()() const;

		Surface& operator=(Surface const&) = delete;

	private:
		Instance const& m_instance;
		VkSurfaceKHR m_surface;
	};
}

#endif
