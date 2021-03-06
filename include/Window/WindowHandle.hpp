#pragma once

#ifndef IDK_WINDOWHANDLE_HPP
#define IDK_WINDOWHANDLE_HPP

namespace Idk {
	enum class WindowProtocol {
		X11,
		Windows,
		Unknow
	};

	struct X11Handle {
		void * dpy; // Display
		unsigned long window; // Window
	};

	struct Win32Handle {
		void * hinstance; // HINSTANCE
		void * hwnd; // HWND
	};

	struct WindowHandle{
		WindowProtocol protocol = WindowProtocol::Unknow;

		union{
			X11Handle x11;
			Win32Handle windows;
		};
	};
}

#endif