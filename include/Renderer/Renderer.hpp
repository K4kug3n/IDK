#pragma once

#ifndef IDK_RENDERER_HPP
#define IDK_RENDERER_HPP

#include <Renderer/Instance.hpp>
#include <Renderer/PhysicalDevice.hpp>

#include <vector>

#include <vulkan/vulkan.h>

namespace Idk {

	class Renderer {
	public:
		Renderer() = default;
		~Renderer() = default;

		bool initialize();

		bool isInitialized();
		
	private:
		Instance m_instance;
		std::vector<PhysicalDevice> m_physicalDevices;
	};
}

#endif
