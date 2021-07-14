#pragma once

#ifndef IDK_RENDERWINDOW_HPP
#define IDK_RENDERWINDOW_HPP

#include <Window/Window.hpp>

#include <Renderer/Surface.hpp>
#include <Renderer/Swapchain.hpp>
#include <Renderer/Semaphore.hpp>
#include <Renderer/Queue.hpp>
#include <Renderer/CommandPool.hpp>
#include <Renderer/CommandBuffer.hpp>

#include <Math/Vector2.hpp>

#include <vector>

namespace Idk {

	class Renderer;

	class RenderWindow : public Window {
	public:
		RenderWindow(Renderer & renderer);
		RenderWindow(Renderer & renderer, VideoMode const& mode, std::string const& title);
		RenderWindow(RenderWindow const&) = delete;
		RenderWindow(RenderWindow&&) = delete;
		~RenderWindow();

		bool create(VideoMode const& mode, std::string const& title);

		bool draw();

	private:
		bool createSwapchain();
		bool createCommandBuffers();
		bool recordCommandBuffers();

		VkSurfaceFormatKHR getSwapchainFormat(std::vector<VkSurfaceFormatKHR> const& surfaceFormats) const;
		uint32_t getSwapchainNumImages(VkSurfaceCapabilitiesKHR const& capabilities) const;
		VkExtent2D getSwapchainExtent(VkSurfaceCapabilitiesKHR const& capabilities, Vector2ui const& size) const;
		VkImageUsageFlags getSwapchainUsageFlags(VkSurfaceCapabilitiesKHR const& capabilities) const;
		VkSurfaceTransformFlagBitsKHR getSwapchainTransform(VkSurfaceCapabilitiesKHR const& capabilities) const;
		VkPresentModeKHR getSwapchainPresentMode(std::vector<VkPresentModeKHR> const& presentModes) const;

		Renderer & m_renderer;
		Surface m_surface;
		Swapchain m_swapchain;
		std::shared_ptr<Device> m_device;
		Semaphore m_imageAvailableSemaphore;
		Semaphore m_renderingFinishedSemaphore;
		Queue m_presentQueue;
		Queue m_graphicsQueue;
		CommandPool m_presentQueueCmdPool;
		std::vector<CommandBuffer> m_presentQueueCmdBuffers;
		bool m_needNewSwapchain;
		Vector2ui m_swapchainSize;
	};
}

#endif
