#pragma once

#ifndef IDK_PHYSICALDEVICE_HPP
#define IDK_PHYSICALDEVICE_HPP

#include <Renderer/Instance.hpp>

#include <vulkan/vulkan.h>

namespace Idk {

	class PhysicalDevice {
	public:
		PhysicalDevice(Instance const& instance, VkPhysicalDevice const& physicalDevice);

		VkPhysicalDeviceProperties getProperties() const;
		VkPhysicalDeviceFeatures getFeatures() const;
		VkPhysicalDeviceMemoryProperties getMemoryProperties() const;

		std::vector<VkExtensionProperties> getExtensionsProperties(const char* layerName = nullptr) const;

		VkPhysicalDevice const& operator()() const;

	private:
		Instance const& m_instance;
		VkPhysicalDevice m_physicalDevice;
	};
}

#endif
