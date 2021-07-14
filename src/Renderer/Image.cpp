#include <Renderer/Image.hpp>

#include <iostream>

namespace Idk {
	Image::Image(VkImage const& image) :
		m_image(image) {
	}

	Image::Image(Image&& object) noexcept :
		m_image(object.m_image){
	}

	VkImage const& Image::operator()() const {
		return m_image;
	}
}
