#include <Renderer/RenderWindow.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/VkUtil.hpp>
#include <Renderer/Image.hpp>

#include <iostream>

namespace Idk {
	RenderWindow::RenderWindow(Renderer& renderer) :
		m_renderer( renderer ),
		m_surface(renderer.getInstance()),
		m_needNewSwapchain(false),
		m_swapchainSize(0, 0),
		m_presentQueue(),
		m_graphicsQueue(){ }

	RenderWindow::RenderWindow(Renderer& renderer, VideoMode const& mode, std::string const& title) : 
		m_renderer{ renderer },
		m_surface(renderer.getInstance()),
		m_needNewSwapchain(false) {
		this->create(mode, title);
	}

	RenderWindow::~RenderWindow() {
		if (m_device) {
			m_device.get()->waitIdle();
		}

		m_swapchain.destroy();
	}

	bool RenderWindow::create(VideoMode const& mode, std::string const& title){
		if (!Window::create(mode, title)) {
			return false;
		}

		if (!m_surface.create(getHandle())) {
			std::cerr << "Error: Can't create surface" << std::endl;
			return false;
		}

		uint32_t graphicsQueueFamilyIndex;
		uint32_t presentQueueFamilyIndex;
		m_device = m_renderer.createDevice(m_surface, presentQueueFamilyIndex, graphicsQueueFamilyIndex);
		if (!m_device) {
			std::cerr << "Error: Can't create surface" << std::endl;
			return false;
		}
		
		if (!m_presentQueue.create(*m_device.get(), presentQueueFamilyIndex)) {
			std::cerr << "Error: Can't create present queue" << std::endl;
			return false;
		}

		if (!m_graphicsQueue.create(*m_device.get(), graphicsQueueFamilyIndex)) {
			std::cerr << "Error: Can't create graphics queue" << std::endl;
			return false;
		}

		if (!m_presentQueueCmdPool.create(*m_device.get(), presentQueueFamilyIndex, 0)) {
			std::cerr << "Error: Can't create present queue command pool" << std::endl;
			return false;
		}

		if (!m_imageAvailableSemaphore.create(*m_device.get())) {
			std::cerr << "Error: Can't create image available semaphore" << std::endl;
			return false;
		}

		if (!m_renderingFinishedSemaphore.create(*m_device.get())) {
			std::cerr << "Error: Can't create rendering finished semaphore" << std::endl;
			return false;
		}

		if (!createSwapchain()) {
			std::cerr << "Error: Can't create swapchain" << std::endl;
			return false;
		}

		if (!createCommandBuffers()) {
			std::cerr << "Error: Can't create command buffer" << std::endl;
			return false;
		}

		std::cout << "It work fine for you my friend" << std::endl;

		return true;
	}

	bool RenderWindow::draw() {
		if (m_needNewSwapchain || (m_swapchainSize != this->size())) {

			m_device.get()->waitIdle();

			if (!createSwapchain()) {
				std::cerr << "Error: Can't re-create swapchain" << std::endl;
				return false;
			}

			for (auto&& commandBuffer : m_presentQueueCmdBuffers) {
				commandBuffer.free();
			}
			m_presentQueueCmdBuffers.clear();
			if (!createCommandBuffers()) {
				std::cerr << "Error: Can't re-create command buffer" << std::endl;
				return false;
			}

			std::cout << "Swapchain recreated" << std::endl;

			m_needNewSwapchain = false;
		}

		uint32_t imageIndex;
		VkResult result{ m_swapchain.aquireNextImage(m_imageAvailableSemaphore(), VK_NULL_HANDLE, imageIndex) };
		switch (result) {
			case VK_SUCCESS:
				break;

			case VK_SUBOPTIMAL_KHR:
				m_needNewSwapchain = true;
				return draw();

			case VK_ERROR_OUT_OF_DATE_KHR:
				m_needNewSwapchain = true;
				break;

			default:
				std::cerr << "Error: Can't aquire next image, " << toString(result) << std::endl;
				return false;
		}

		VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
		VkSubmitInfo submitInfo = {
			VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
			nullptr,                                      // const void                  *pNext
			1,                                            // uint32_t                     waitSemaphoreCount
			&m_imageAvailableSemaphore(),                 // const VkSemaphore           *pWaitSemaphores
			&waitDstStageMask,                            // const VkPipelineStageFlags  *pWaitDstStageMask;
			1,                                            // uint32_t                     commandBufferCount
			&m_presentQueueCmdBuffers[imageIndex](),       // const VkCommandBuffer       *pCommandBuffers
			1,                                            // uint32_t                     signalSemaphoreCount
			&m_renderingFinishedSemaphore()               // const VkSemaphore           *pSignalSemaphores
		};

		if (!m_presentQueue.submit(submitInfo)) {
			return false;
		}

		VkPresentInfoKHR presentInfo = {
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,   // VkStructureType              sType
			nullptr,                              // const void                  *pNext
			1,                                    // uint32_t                     waitSemaphoreCount
			&m_renderingFinishedSemaphore(),      // const VkSemaphore           *pWaitSemaphores
			1,                                    // uint32_t                     swapchainCount
			&m_swapchain(),                       // const VkSwapchainKHR        *pSwapchains
			&imageIndex,                          // const uint32_t              *pImageIndices
			nullptr                               // VkResult                    *pResults
		};

		result = m_presentQueue.present(presentInfo);

		switch (result) {
			case VK_SUCCESS:
				break;
			case VK_ERROR_OUT_OF_DATE_KHR:
			case VK_SUBOPTIMAL_KHR:
				m_needNewSwapchain = true;
				break;
			default:
				std::cerr << "Error: Can't present image" << std::endl;
				return false;
		}

		return true;
	}

	bool RenderWindow::createSwapchain() {
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		if (!m_surface.getCapabilities(m_device.get()->getPhysicalDevice(), surfaceCapabilities)) {
			std::cerr << "Error: Can't get surface capabilities" << std::endl;
			return false;
		}

		uint32_t imageCount{ getSwapchainNumImages(surfaceCapabilities) };
		VkImageUsageFlags swapchainUsageFlag{ getSwapchainUsageFlags(surfaceCapabilities) };
		VkSurfaceTransformFlagBitsKHR swapchainTransform{ getSwapchainTransform(surfaceCapabilities) };
		Vector2ui size = this->size();
		VkExtent2D swapchainExtend{ getSwapchainExtent(surfaceCapabilities, size) };


		std::vector<VkSurfaceFormatKHR> surfaceFormats;
		if (!m_surface.getFormats(m_device.get()->getPhysicalDevice(), surfaceFormats)) {
			std::cerr << "Error: Can't get surface formats" << std::endl;
			return false;
		}

		VkSurfaceFormatKHR surfaceFormat{ getSwapchainFormat(surfaceFormats) };

		std::vector<VkPresentModeKHR> presentModes;
		if (!m_surface.getPresentModes(m_device.get()->getPhysicalDevice(), presentModes)) {
			std::cerr << "Error: Can't get surface present modes" << std::endl;
			return false;
		}

		VkPresentModeKHR presentMode{ getSwapchainPresentMode(presentModes) };

		if (static_cast<int>(swapchainUsageFlag) == -1) {
			return false;
		}
		if (static_cast<int>(presentMode) == -1) {
			return false;
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  // VkStructureType                sType
			nullptr,                                      // const void                    *pNext
			0,                                            // VkSwapchainCreateFlagsKHR      flags
			m_surface(),                                  // VkSurfaceKHR                   surface
			imageCount,                                   // uint32_t                       minImageCount
			surfaceFormat.format,                         // VkFormat                       imageFormat
			surfaceFormat.colorSpace,                     // VkColorSpaceKHR                imageColorSpace
			swapchainExtend,                              // VkExtent2D                     imageExtent
			1,                                            // uint32_t                       imageArrayLayers
			swapchainUsageFlag,                           // VkImageUsageFlags              imageUsage
			VK_SHARING_MODE_EXCLUSIVE,                    // VkSharingMode                  imageSharingMode
			0,                                            // uint32_t                       queueFamilyIndexCount
			nullptr,                                      // const uint32_t                *pQueueFamilyIndices
			swapchainTransform,                           // VkSurfaceTransformFlagBitsKHR  preTransform
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,            // VkCompositeAlphaFlagBitsKHR    compositeAlpha
			presentMode,                                  // VkPresentModeKHR               presentMode
			VK_TRUE,                                      // VkBool32                       clipped
			m_swapchain()                                 // VkSwapchainKHR                 oldSwapchain
		};

		Swapchain newSwapchain;
		if (!newSwapchain.create(*m_device.get(), swapchainCreateInfo)) {
			std::cerr << "Error: Can't create swapchain" << std::endl;
			return false;
		}

		m_swapchain = std::move(newSwapchain);
		m_swapchainSize = size;

		return true;
	}

	bool RenderWindow::createCommandBuffers() {
		uint32_t imageCount{ 0 };
		if (!m_swapchain.getNumImages(imageCount)) {
			return false;
		}

		if (!m_presentQueueCmdPool.allocateCommandBuffer(imageCount, VK_COMMAND_BUFFER_LEVEL_PRIMARY, m_presentQueueCmdBuffers)) {
			return false;
		}

		if (!recordCommandBuffers()) {
			std::cerr << "Error: Can't record command buffers" << std::endl;
			return false;
		}

		return true;
	}

	bool RenderWindow::recordCommandBuffers() {
		std::vector<Image> swapchainImages;
		if (!m_swapchain.getImages(swapchainImages)) {
			return false;
		}

		VkCommandBufferBeginInfo cmdBufferBeginInfo = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,  // VkStructureType                        sType
			nullptr,                                      // const void                            *pNext
			VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, // VkCommandBufferUsageFlags              flags
			nullptr                                       // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
		};

		VkClearColorValue clearColor = {
			{ 1.0f, 0.8f, 0.4f, 0.0f }
		};

		VkImageSubresourceRange imageSubresourceRange = {
			VK_IMAGE_ASPECT_COLOR_BIT, // VkImageAspectFlags   aspectMask
			0,                         // uint32_t             baseMipLevel
			1,                         // uint32_t             levelCount
			0,                         // uint32_t             baseArrayLayer
			1                          // uint32_t             layerCount
		};

		uint32_t imageCount; // TODO : Duplicate with Swapchain::getImages()
		if (!m_swapchain.getNumImages(imageCount)) {
			return false;
		}


		for (uint32_t i = 0; i < imageCount; ++i) {
			VkImageMemoryBarrier barrierFromPresentToClear = {
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
				nullptr,                                    // const void                            *pNext
				VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          srcAccessMask
				VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          dstAccessMask
				VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                          oldLayout
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          newLayout
				m_presentQueue.index(),                     // uint32_t                               srcQueueFamilyIndex
				m_presentQueue.index(),                     // uint32_t                               dstQueueFamilyIndex
				swapchainImages[i](),                       // VkImage                                image
				imageSubresourceRange                       // VkImageSubresourceRange                subresourceRange
			};

			VkImageMemoryBarrier barrierFromClearToPresent = {
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
				nullptr,                                    // const void                            *pNext
				VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          srcAccessMask
				VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          dstAccessMask
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          oldLayout
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout                          newLayout
				m_presentQueue.index(),                     // uint32_t                               srcQueueFamilyIndex
				m_presentQueue.index(),                     // uint32_t                               dstQueueFamilyIndex
				swapchainImages[i](),                       // VkImage                                image
				imageSubresourceRange                       // VkImageSubresourceRange                subresourceRange
			};

			m_device.get()->vkBeginCommandBuffer(m_presentQueueCmdBuffers[i](), &cmdBufferBeginInfo);
			m_device.get()->vkCmdPipelineBarrier(m_presentQueueCmdBuffers[i](), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrierFromPresentToClear);

			m_device.get()->vkCmdClearColorImage(m_presentQueueCmdBuffers[i](), swapchainImages[i](), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &imageSubresourceRange);

			m_device.get()->vkCmdPipelineBarrier(m_presentQueueCmdBuffers[i](), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrierFromClearToPresent);
			if (m_device.get()->vkEndCommandBuffer(m_presentQueueCmdBuffers[i]()) != VK_SUCCESS) {
				std::cout << "Could not record command buffers!" << std::endl;
				return false;
			}
		}

		return true;
	}

	VkSurfaceFormatKHR RenderWindow::getSwapchainFormat(std::vector<VkSurfaceFormatKHR> const& surfaceFormats) const {
		if ((surfaceFormats.size() == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED)) {
			return{ VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		}

		for (VkSurfaceFormatKHR const& surfaceFormat : surfaceFormats) {
			if (surfaceFormat.format == VK_FORMAT_R8G8B8A8_UNORM) {
				return surfaceFormat;
			}
		}

		return surfaceFormats[0];
	}

	uint32_t RenderWindow::getSwapchainNumImages(VkSurfaceCapabilitiesKHR const& capabilities) const {
		uint32_t imageCount = capabilities.minImageCount + 1;
		if ((capabilities.maxImageCount > 0) && (imageCount > capabilities.maxImageCount)) {
			imageCount = capabilities.maxImageCount;
		}

		return imageCount;
	}

	VkExtent2D RenderWindow::getSwapchainExtent(VkSurfaceCapabilitiesKHR const& capabilities, Vector2ui const& size) const {
		if (capabilities.currentExtent.width == -1) {
			VkExtent2D swapchainExtent = { size.x, size.y };
			if (swapchainExtent.width < capabilities.minImageExtent.width) {
				swapchainExtent.width = capabilities.minImageExtent.width;
			}
			if (swapchainExtent.height < capabilities.minImageExtent.height) {
				swapchainExtent.height = capabilities.minImageExtent.height;
			}
			if (swapchainExtent.width > capabilities.maxImageExtent.width) {
				swapchainExtent.width = capabilities.maxImageExtent.width;
			}
			if (swapchainExtent.height > capabilities.maxImageExtent.height) {
				swapchainExtent.height = capabilities.maxImageExtent.height;
			}
			return swapchainExtent;
		}

		return capabilities.currentExtent;
	}

	VkImageUsageFlags RenderWindow::getSwapchainUsageFlags(VkSurfaceCapabilitiesKHR const& capabilities) const {
		if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
			return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}
		std::cerr << "Error: VK_IMAGE_USAGE_TRANSFER_DST image usage is not supported by the swap chain!" << std::endl
			<< "Supported swap chain's image usages include:" << std::endl
			<< (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT ? "    VK_IMAGE_USAGE_TRANSFER_SRC\n" : "")
			<< (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT ? "    VK_IMAGE_USAGE_TRANSFER_DST\n" : "")
			<< (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT ? "    VK_IMAGE_USAGE_SAMPLED\n" : "")
			<< (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT ? "    VK_IMAGE_USAGE_STORAGE\n" : "")
			<< (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_COLOR_ATTACHMENT\n" : "")
			<< (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT\n" : "")
			<< (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT\n" : "")
			<< (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_INPUT_ATTACHMENT" : "")
			<< std::endl;

		return static_cast<VkImageUsageFlags>(-1);
	}

	VkSurfaceTransformFlagBitsKHR RenderWindow::getSwapchainTransform(VkSurfaceCapabilitiesKHR const& capabilities) const {
		if (capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
			return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else {
			return capabilities.currentTransform;
		}
	}

	VkPresentModeKHR RenderWindow::getSwapchainPresentMode(std::vector<VkPresentModeKHR> const& presentModes) const {
		// FIFO present mode is always available
		// MAILBOX is the lowest latency V-Sync enabled mode (something like triple-buffering) so use it if available
		for (VkPresentModeKHR const& presentMode : presentModes) {
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return presentMode;
			}
		}
		for (VkPresentModeKHR const& presentMode : presentModes) {
			if (presentMode == VK_PRESENT_MODE_FIFO_KHR) {
				return presentMode;
			}
		}

		std::cerr << "Error: FIFO present mode is not supported by the swapchain" << std::endl;
		return static_cast<VkPresentModeKHR>(-1);
	}

}

