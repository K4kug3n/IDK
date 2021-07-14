#pragma once

#ifndef IDK_WINDOWEVENT_HPP
#define IDK_WINDOWEVENT_HPP

namespace Idk {
	enum class WindowEventType {
		Quit,
		Resized,

		Max
	};

	struct WindowEvent {
		WindowEventType type;
	};
}

#endif