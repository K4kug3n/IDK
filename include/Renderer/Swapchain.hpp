#pragma once

#ifndef IDK_SWAPCHAIN_HPP
#define IDK_SWAPCHAIN_HPP

#include <vulkan/vulkan.h>

#include <vector>

namespace Idk {
	class Device;
	class Semaphore;
	class Image;

	class Swapchain {
	public:
		Swapchain();
		Swapchain(Swapchain const&) = delete;
		Swapchain(Swapchain&& object) noexcept;
		~Swapchain();

		VkResult aquireNextImage(VkSemaphore const& semaphore, VkFence const& fence, uint32_t & imageIndex);

		bool create(Device const& device, VkSwapchainCreateInfoKHR const& infos);
		void destroy();

		bool getNumImages(uint32_t & imageCount) const;
		bool getImages(std::vector<Image> & images) const;

		VkSwapchainKHR const& operator()() const;

		Swapchain& operator=(Swapchain const&) = delete;
		Swapchain& operator=(Swapchain&& object) noexcept;

	private:
		VkSwapchainKHR m_swapchain;
		Device const* m_device;
	};
}

#endif
