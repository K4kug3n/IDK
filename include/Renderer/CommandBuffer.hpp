#pragma once

#ifndef IDK_COMMANDBUFFER_HPP
#define IDK_COMMANDBUFFER_HPP

#include <vulkan/vulkan.h>

namespace Idk {
	class CommandPool;

	class CommandBuffer {
	public:
		CommandBuffer(CommandPool const* pool, VkCommandBuffer & commandBuffer);
		CommandBuffer(CommandBuffer const& object) = delete;
		CommandBuffer(CommandBuffer&& object) noexcept;
		~CommandBuffer() = default;

		void free();

		VkCommandBuffer const& operator()() const;

		CommandBuffer& operator=(CommandBuffer const& object) = delete;
		CommandBuffer& operator=(CommandBuffer&& object) = delete;

	private:
		VkCommandBuffer m_commandBuffer;
		CommandPool const* m_pool;
	};
}

#endif
