#include <Renderer/Renderer.hpp>

#include <iostream>
#include <unordered_set>
#include <string>
#include <array>

#include <Renderer/Loader.hpp>
#include <Renderer/Surface.hpp>

#include <vulkan/vulkan.h>

namespace Idk {

	bool Renderer::initialize(){
		if (isInitialized()) {
			std::cerr << "Error: Renderer already initialized" << std::endl;
			return false;
		}

		if (!Loader::initialize()) {
			std::cerr << "Error: Can't load Vulkan" << std::endl;
			return false;
		}

		uint32_t supportedApi{ VK_MAKE_VERSION(1, 0, 0) };
		if (Loader::vkEnumerateInstanceVersion) {
			Loader::vkEnumerateInstanceVersion(&supportedApi);
		}

		std::unordered_set<std::string> availableLayer{};
		for (VkLayerProperties const& layerProp : Loader::enumerateLayerProperties()) {
			availableLayer.insert(layerProp.layerName);
		}

		std::vector<const char*> enabledLayer;
		#if defined(IDK_DEBUG)
		if (availableLayer.count("VK_LAYER_KHRONOS_validation")) {
			enabledLayer.push_back("VK_LAYER_KHRONOS_validation");
		}
		#endif

		std::unordered_set<std::string> availableExtensions{};
		for (VkExtensionProperties const& extensionsProp : Loader::enumerateExtensionProperties()) {
			availableExtensions.insert(extensionsProp.extensionName);
		}

		std::vector<const char*> enabledExtensions;
		#ifdef VK_USE_PLATFORM_XCB_KHR
		enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
		#endif
		#ifdef VK_USE_PLATFORM_XLIB_KHR
		enabledExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
		#endif
		#ifdef VK_USE_PLATFORM_WIN32_KHR
		enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		#endif

		enabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

		//if (availableExtensions.count(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
		//	enabledExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

		//if (availableExtensions.count(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
		//	enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		if (!m_instance.create("IDK_RENDERER", VK_MAKE_VERSION(0, 0, 1), "IDK", VK_MAKE_VERSION(0, 0, 1), supportedApi, enabledLayer, enabledExtensions)) {
			std::cerr << "Error: Can't create Instance" << std::endl;
			return false;
		}

		m_physicalDevices = m_instance.enumeratePhysicalDevices();
		if (m_physicalDevices.empty()) {
			std::cerr << "Error: No physical device detected" << std::endl;
			return false;
		}

		for (auto const& device : m_physicalDevices) {
			std::cout << "Device: " << device.getProperties().deviceName << std::endl;
		}

		return true;
	}

	std::shared_ptr<Device> Renderer::createDevice(Surface & surface, uint32_t & presentQueueFamilyIndex, uint32_t & graphicsQueueFamilyIndex){
		presentQueueFamilyIndex =  UINT32_MAX;
		graphicsQueueFamilyIndex =  UINT32_MAX;

		size_t selected_index{ m_physicalDevices.size() };
		for (size_t i{ 0 }; i < m_physicalDevices.size(); ++i) {
			if (checkPhysicalDeviceProperties(m_physicalDevices[i], surface, presentQueueFamilyIndex, graphicsQueueFamilyIndex)) {
				selected_index = i;
			};
		}
		
		if (selected_index == m_physicalDevices.size()) {
			std::cerr << "Error : Can't find physical with needed properties" << std::endl;
			return {};
		}

		PhysicalDevice& physicalDevice{ m_physicalDevices[selected_index] };

		std::array<QueueFamily, 2> queueFamilies{
			{
				{ graphicsQueueFamilyIndex, 1.0f },
				{ presentQueueFamilyIndex, 1.0f }
			}
		};

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		queueCreateInfos.reserve(queueFamilies.size());

		for (size_t i{ 0 }; i < queueFamilies.size(); ++i) {
			auto it = std::find_if(queueCreateInfos.begin(), queueCreateInfos.end(), [&](VkDeviceQueueCreateInfo const& info) {
				return info.queueFamilyIndex == queueFamilies[i].index;
			});

			if (it == queueCreateInfos.end()) {
				queueCreateInfos.push_back({
					VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,       // VkStructureType              sType
					nullptr,                                          // const void                  *pNext
					0,                                                // VkDeviceQueueCreateFlags     flags
					queueFamilies[i].index,                           // uint32_t                     queueFamilyIndex
					1,                                                // uint32_t                     queueCount
					&queueFamilies[i].priority                        // const float                 *pQueuePriorities
				});
			}
		}

		std::vector<const char*> extensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VkDeviceCreateInfo deviceCreateInfo = {
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,             // VkStructureType                    sType
			nullptr,                                          // const void                        *pNext
			0,                                                // VkDeviceCreateFlags                flags
			static_cast<uint32_t>(queueCreateInfos.size()),   // uint32_t                           queueCreateInfoCount
			queueCreateInfos.data(),                          // const VkDeviceQueueCreateInfo     *pQueueCreateInfos
			0,                                                // uint32_t                           enabledLayerCount
			nullptr,                                          // const char * const                *ppEnabledLayerNames
			static_cast<uint32_t>(extensions.size()),         // uint32_t                           enabledExtensionCount
			extensions.data(),                                // const char * const                *ppEnabledExtensionNames
			nullptr                                           // const VkPhysicalDeviceFeatures    *pEnabledFeatures
		};

		m_device = std::make_shared<Device>(m_instance);
		if (!m_device->create(physicalDevice, deviceCreateInfo)) {
			std::cerr << "Error : Canno't create device" << std::endl;
			return {};
		}

		return m_device;
	}

	bool Renderer::isInitialized(){
		return m_instance.isValid();
	}

	Instance & Renderer::getInstance(){
		return m_instance;
	}

	bool Renderer::checkPhysicalDeviceProperties(PhysicalDevice& physicalDevice, Surface & surface, uint32_t& graphicsQueueFamilyIndex, uint32_t& presentQueueFamilyIndex) {
		std::vector<char const*> devicesExtensionsNames = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		for (auto const& name : devicesExtensionsNames) {
			if (!physicalDevice.isSupportedExtension(name)) {
				std::cerr << "Warning: physical device " << physicalDevice() << " don't support " << name << std::endl;
				return false;
			}
		}

		VkPhysicalDeviceProperties properties{ physicalDevice.getProperties() };
		VkPhysicalDeviceFeatures features{ physicalDevice.getFeatures() };

		uint32_t majorVersion = VK_VERSION_MAJOR(properties.apiVersion);
		if ((majorVersion < 1) && (properties.limits.maxImageDimension2D < 4096)) {
			std::cerr << "Warning: physical device " << physicalDevice() << "don't support require parameters" << std::endl;
			return false;
		}

		std::vector<VkQueueFamilyProperties> queueFamiliesProperities{ physicalDevice.getQueueFamilyProperties() };

		for (size_t i{ 0 }; i < queueFamiliesProperities.size(); ++i) {
			bool supported{ false };
			if (!surface.getPhysicalDeviceSurfaceSupport(physicalDevice, i, supported)) {
				std::cerr << "Warning: Can't get support of queue " << i << std::endl;
			}

			if (queueFamiliesProperities[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { // Support graphics operations
				if (supported) { // Prefer a queue that do everythings
					graphicsQueueFamilyIndex = i;
					presentQueueFamilyIndex = i;
					return true; // We have all we want
				}
				else if (graphicsQueueFamilyIndex == UINT32_MAX) {
					graphicsQueueFamilyIndex = i;
				}
			}
			else if (supported) {
				presentQueueFamilyIndex = i;
			}
		}

		if (graphicsQueueFamilyIndex == UINT32_MAX) {
			std::cerr << "Warning: Physical device " << physicalDevice() << " haven't graphics queue" << std::endl;
			return false;
		}

		if (presentQueueFamilyIndex == UINT32_MAX) {
			std::cerr << "Warning: Physical device " << physicalDevice() << " doesn't support this surface" << std::endl;
			return false;
		}

		return true;
	}
}
