#include <Renderer/CommandBuffer.hpp>

#include <Renderer/CommandPool.hpp>
#include <Renderer/Device.hpp>

#include <iostream>

namespace Idk {
	CommandBuffer::CommandBuffer(CommandPool const* pool, VkCommandBuffer & commandBuffer):
		m_commandBuffer(commandBuffer),
		m_pool(pool) {
	}

	CommandBuffer::CommandBuffer(CommandBuffer&& object) noexcept :
		m_commandBuffer(object.m_commandBuffer),
		m_pool(object.m_pool) {
	}

	void CommandBuffer::free() {
		if (m_commandBuffer != VK_NULL_HANDLE) {
			m_pool->getDevice()->vkFreeCommandBuffers((*m_pool->getDevice())(), (*m_pool)(), 1, &m_commandBuffer);
		}
	}

	VkCommandBuffer const& CommandBuffer::operator()() const {
		return m_commandBuffer;
	}

}
