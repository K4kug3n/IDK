#include <Renderer/PhysicalDevice.hpp>

#include <Renderer/VkUtil.hpp>

#include <iostream>

namespace Idk {
    
    PhysicalDevice::PhysicalDevice(Instance const& instance, VkPhysicalDevice const& physicalDevice) :
        m_physicalDevice{ physicalDevice },
        m_instance{ instance } { }

    VkPhysicalDeviceProperties PhysicalDevice::getProperties() const {
        VkPhysicalDeviceProperties properties;
        m_instance.vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);

        return properties;
    }

    VkPhysicalDeviceFeatures PhysicalDevice::getFeatures() const {
        VkPhysicalDeviceFeatures features;
        m_instance.vkGetPhysicalDeviceFeatures(m_physicalDevice, &features);

        return features;
    }

    VkPhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties() const {
        VkPhysicalDeviceMemoryProperties memoryPropeties;
        m_instance.vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memoryPropeties);

        return memoryPropeties;
    }

    std::vector<VkExtensionProperties> PhysicalDevice::getExtensionsProperties(const char* layerName) const {
        uint32_t count{ 0 };
        VkResult result{ m_instance.vkEnumerateDeviceExtensionProperties(m_physicalDevice, layerName, &count, nullptr) };
        if (result != VkResult::VK_SUCCESS || count == 0) {
            std::cerr << "Error: Can't get number of extensions propeties : " << toString(result) << std::endl;
            return std::vector<VkExtensionProperties>();
        }
        
        std::vector<VkExtensionProperties> extensionsProperties(count);
        result = m_instance.vkEnumerateDeviceExtensionProperties(m_physicalDevice, layerName, &count, extensionsProperties.data());
        if (result != VkResult::VK_SUCCESS) {
            std::cerr << "Error: Can't enumerate extensions propeties : " << toString(result) << std::endl;
            return std::vector<VkExtensionProperties>();
        }

        return extensionsProperties;
    }

    VkPhysicalDevice const& PhysicalDevice::operator()() const {
        return m_physicalDevice;
    }
}
