#pragma once

#ifndef IDK_IMAGE_HPP
#define IDK_IMAGE_HPP

#include <vulkan/vulkan.h>

namespace Idk {

	class Image {
	public:
		Image(VkImage const& image);
		Image(Image const&) = delete;
		Image(Image&& object) noexcept;
		~Image() = default;

		VkImage const& operator()() const;

		Image& operator=(Image const&) = delete;
		Image& operator=(Image&&) = delete;

	private:
		VkImage m_image;
	};
}

#endif
