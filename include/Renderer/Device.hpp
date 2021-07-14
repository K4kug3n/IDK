#pragma once

#ifndef IDK_DEVICE_HPP
#define IDK_DEVICE_HPP

#include <unordered_set>
#include <string>

#include <Renderer/Instance.hpp>

#include <vulkan/vulkan.h>

namespace Idk {

	class PhysicalDevice;

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

		PhysicalDevice const& getPhysicalDevice() const;

		void waitIdle() const;

		#define IDK_DEVICE_FUNCTION(fun) PFN_##fun fun;
		#include <Renderer/DeviceFunctions.inl>

		VkDevice const& operator()() const;

		Device& operator=(Device const&) = delete;
		Device& operator=(Device&&) = delete;

	private:
		PFN_vkVoidFunction loadDeviceFunction(const char* name);

		VkDevice m_device;
		PhysicalDevice const* m_physicalDevice;
		Instance const& m_instance;

		std::unordered_set<std::string> m_extensions;
		std::unordered_set<std::string> m_layers;
	};
}

#endif
