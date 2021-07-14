#include <Renderer/CommandPool.hpp>

#include <Renderer/Device.hpp>
#include <Renderer/VkUtil.hpp>
#include <Renderer/CommandBuffer.hpp>

#include <iostream>

namespace Idk {
	CommandPool::CommandPool() :
		m_commandPool(VK_NULL_HANDLE),
		m_device(nullptr) {
	}

	CommandPool::~CommandPool() {
		if (m_commandPool != VK_NULL_HANDLE) {
			m_device->vkDestroyCommandPool((*m_device)(), m_commandPool, nullptr);
		}	
	}

	bool CommandPool::create(Device const& device, uint32_t index, VkCommandPoolCreateFlags flags) {
		VkCommandPoolCreateInfo cmdPoolCreateInfo = {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,     // VkStructureType              sType
			nullptr,                                        // const void*                  pNext
			flags,                                          // VkCommandPoolCreateFlags     flags
			index                                           // uint32_t                     queueFamilyIndex
		};

		VkResult result{ device.vkCreateCommandPool(device(), &cmdPoolCreateInfo, nullptr, &m_commandPool) };
		if (result != VK_SUCCESS) {
			std::cerr << "Error: Can't create command pool, " << toString(result) << std::endl;
			return false;
		}

		m_device = &device;

		return true;
	}

	bool CommandPool::allocateCommandBuffer(uint32_t nbBuffer, VkCommandBufferLevel level, std::vector<CommandBuffer> & commandBuffers) const {
		VkCommandBufferAllocateInfo cmd_buffer_allocate_info = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, // VkStructureType              sType
			nullptr,                                        // const void*                  pNext
			m_commandPool,                                  // VkCommandPool                commandPool
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,                // VkCommandBufferLevel         level
			nbBuffer                                        // uint32_t                     bufferCount
		};

		std::vector<VkCommandBuffer> vkCommandBuffers(nbBuffer, VK_NULL_HANDLE);
		VkResult result{ m_device->vkAllocateCommandBuffers((*m_device)(), &cmd_buffer_allocate_info, vkCommandBuffers.data()) };
		if (result != VK_SUCCESS) {
			std::cout << "Error: Could not allocate command buffers, " << toString(result) << std::endl;
			return false;
		}

		for (auto&& vkCommandBuffer : vkCommandBuffers) {
			commandBuffers.emplace_back(CommandBuffer(this, vkCommandBuffer));
		}

		return true;
	}

	Device const* CommandPool::getDevice() const {
		return m_device;
	}

	VkCommandPool const& CommandPool::operator()() const {
		return m_commandPool;
	}
}
