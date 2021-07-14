#pragma once

#ifndef IDK_SEMAPHORE_HPP
#define IDK_SEMAPHORE_HPP

#include <vulkan/vulkan.h>

namespace Idk {
	class Device;

	class Semaphore {
	public:
		Semaphore();
		Semaphore(Semaphore const&) = delete;
		Semaphore(Semaphore&&) = delete;
		~Semaphore();

		bool create(Device const& device);

		VkSemaphore const& operator()() const;

		Semaphore& operator=(Semaphore const&) = delete;
		Semaphore& operator=(Semaphore&&) = delete;

	private:
		VkSemaphore m_sempahore;
		Device const* m_device;
	};
}

#endif
