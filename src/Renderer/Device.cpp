#include <Renderer/Device.hpp>

namespace Idk {
    Device::Device(Instance const& instance) :
        m_device{ nullptr },
        m_instance{ instance } {
        #define IDK_DEVICE_FUNCTION(fun) fun = nullptr;
        #include <Renderer/DeviceFunctions.inl>
    }

    Device::~Device(){
        if (m_device) {
            vkDestroyDevice(m_device, nullptr);
        }
    }

    bool Device::isValid() const {
        return m_device != nullptr;
    }

    bool Device::isLoadedExtension(std::string const& name) const {
        return m_extensions.count(name) > 0;
    }

    bool Device::isLoadedLayer(std::string const& name) const {
        return m_layers.count(name) > 0;
    }
}
