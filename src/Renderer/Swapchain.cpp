#include <Renderer/Swapchain.hpp>

#include <Renderer/Device.hpp>
#include <Renderer/VkUtil.hpp>
#include <Renderer/Semaphore.hpp>
#include <Renderer/Image.hpp>

#include <iostream>

namespace Idk {
	Swapchain::Swapchain() :
		m_swapchain(VK_NULL_HANDLE),
		m_device(nullptr) {
	}

	Swapchain::Swapchain(Swapchain&& object) noexcept :
		m_swapchain(object.m_swapchain),
		m_device(std::move(object.m_device)) {
		object.m_swapchain = VK_NULL_HANDLE;
	}

	Swapchain::~Swapchain() {
		destroy();
	}

	VkResult Swapchain::aquireNextImage(VkSemaphore const& semaphore, VkFence const& fence, uint32_t & imageIndex) {
		return m_device->vkAcquireNextImageKHR((*m_device)(), m_swapchain, UINT64_MAX, semaphore, fence, &imageIndex);
	}

	bool Swapchain::create(Device const& device, VkSwapchainCreateInfoKHR const& infos) {
		VkResult result{ device.vkCreateSwapchainKHR(device(), &infos, nullptr, &m_swapchain) };
		if (result != VK_SUCCESS) {
			std::cerr << "Error: Can't create swapchain, " << toString(result) << std::endl;
			return false;
		}

		m_device = &device;

		return true;
	}

	void Swapchain::destroy() {
		if (m_swapchain != VK_NULL_HANDLE) {
			m_device->vkDestroySwapchainKHR((*m_device)(), m_swapchain, nullptr);
			m_swapchain = VK_NULL_HANDLE;
		}
	}

	bool Swapchain::getNumImages(uint32_t & imageCount) const {
		VkResult result{ m_device->vkGetSwapchainImagesKHR((*m_device)(), m_swapchain, &imageCount, nullptr) };
		if ((result != VK_SUCCESS) || (imageCount == 0)) {
			std::cout << "Error: Could not get the number of swap chain images, " << toString(result) << std::endl;
			return false;
		}

		return true;
	}

	bool Swapchain::getImages(std::vector<Image> & images) const {
		uint32_t imageCount;
		if (!getNumImages(imageCount)) {
			return false;
		}

		std::vector<VkImage> vkImages(imageCount, VK_NULL_HANDLE);
		VkResult result{ m_device->vkGetSwapchainImagesKHR((*m_device)(), m_swapchain, &imageCount, vkImages.data()) };
		if (result != VK_SUCCESS) {
			std::cout << "Error: Could not get swap chain images, " << toString(result) << std::endl;
			return false;
		}

		for (auto&& vkImage : vkImages) {
			images.emplace_back(Image(vkImage));
		}

		return true;
	}

	VkSwapchainKHR const& Swapchain::operator()() const {
		return m_swapchain;
	}

	Swapchain& Swapchain::operator=(Swapchain&& object) noexcept {
		std::swap(m_swapchain, object.m_swapchain);
		std::swap(m_device, object.m_device);

		return *this;
	}
}