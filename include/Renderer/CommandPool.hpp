#pragma once

#ifndef IDK_COMMANDPOOL_HPP
#define IDK_COMMANDPOOL_HPP

#include <vulkan/vulkan.h>

#include <vector>

namespace Idk {
	class Device;
	class CommandBuffer;

	class CommandPool {
	public:
		CommandPool();
		CommandPool(CommandPool const&) = delete;
		CommandPool(CommandPool&&) = delete;
		~CommandPool();

		bool create(Device const& device, uint32_t index, VkCommandPoolCreateFlags flags);

		bool allocateCommandBuffer(uint32_t nbBuffer, VkCommandBufferLevel level, std::vector<CommandBuffer> & commandBuffers) const;

		Device const* getDevice() const;
		VkCommandPool const& operator()() const;

		CommandPool& operator=(CommandPool const&) = delete;
		CommandPool& operator=(CommandPool&&) = delete;

	private:
		VkCommandPool m_commandPool;
		Device const* m_device;
	};
}

#endif
