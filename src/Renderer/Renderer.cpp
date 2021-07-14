#include <Renderer/Renderer.hpp>

#include <iostream>
#include <unordered_set>
#include <string>

#include <Renderer/Loader.hpp>

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
        // Add needed layers

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

        /*if (availableExtensions.count(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
            enabledExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

        if (availableExtensions.count(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
            enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);*/

        if (!m_instance.create("IDK_RENDERER", VK_MAKE_VERSION(0, 0, 1), "IDK", VK_MAKE_VERSION(0, 0, 1), supportedApi, enabledLayer, enabledExtensions)) {
            std::cerr << "Error: Can't create Instance" << std::endl;
            return false;
        }

        m_physicalDevices = m_instance.enumeratePhysicalDevices();
        if (m_physicalDevices.empty()) {
            std::cerr << "Error: No physical device detected" << std::endl;
        }

        for (auto const& device : m_physicalDevices) {
            std::cout << "Device: " << device.getProperties().deviceName << std::endl;
        }

        return true;
    }

    bool Renderer::isInitialized(){
        return m_instance.isValid();
    }
}
