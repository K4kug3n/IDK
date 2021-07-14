#pragma once

#ifndef IDK_RENDERER_HPP
#define IDK_RENDERER_HPP

#include <Renderer/Instance.hpp>
#include <Renderer/PhysicalDevice.hpp>
#include <Renderer/Device.hpp>
#include <Renderer/Queue.hpp>

#include <vector>
#include <memory>

#include <vulkan/vulkan.h>

namespace Idk {
	
	class Surface;

	class Renderer {
	public:
		Renderer() = default;
		~Renderer() = default;

		bool initialize();
		bool isInitialized();

		std::shared_ptr<Device> createDevice(Surface & surface, uint32_t & presentQueueFamilyIndex, uint32_t & graphicsQueueFamilyIndex);

		Instance & getInstance();
	private:
		struct QueueFamily {
			uint32_t index;
			float priority;
		};

		bool checkPhysicalDeviceProperties(PhysicalDevice& physicalDevice, Surface & surface, uint32_t & graphicsQueueFamilyIndex, uint32_t & presentQueueFamilyIndex);

		Instance m_instance;
		std::vector<PhysicalDevice> m_physicalDevices;
		std::shared_ptr<Device> m_device;
	};
}

#endif