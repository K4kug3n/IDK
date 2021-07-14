#include <Window/Window.hpp>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

namespace Idk {
	Window::Window() :
		m_handle{ nullptr },
		m_closed{ false }{}

	Window::Window(VideoMode const& mode, std::string const& title):
		m_closed{ false } {
		create(mode, title);
	}

	Window::~Window() {
		destroy();
	}

	bool Window::initialize() {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Could not initialize SDL_VIDEO" << std::endl;
			return false;
		}

		return true;
	}

	void Window::uninitialize() {
		SDL_Quit();
	}

	bool Window::create(VideoMode const& mode, std::string const& title) {
		m_handle = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.width, mode.height, 0);

		if (!m_handle) {
			std::cerr << "Could not create the window" << std::endl;
			return false;
		}

		SDL_AddEventWatch(handleInput, this);

		return true;
	}

	void Window::close(){
		m_closed = true;
	}

	bool Window::isOpen() {
		if (!m_handle) {
			return false;
		}

		if (m_closed) {
			destroy();
			return false;
		}

		return true;
	}

	void Window::processEvent() const {
		if (m_handle) {
			SDL_PumpEvents();
		}
	}

	EventHandler& Window::getEventHandler() {
		return m_eventHandler;
	}

	WindowHandle Window::getHandle() const {
		if (!m_handle) {
			return WindowHandle{};
		}

		SDL_SysWMinfo info;
		if (!SDL_GetWindowWMInfo(m_handle, &info)) {
			std::cerr << "Can't retrived window infos" << std::endl;
			return WindowHandle{};
		}

		WindowHandle handle;
		switch (info.subsystem) {
		#if defined(SDL_VIDEO_DRIVER_X11)
		case SDL_SYSWM_X11:
			handle.protocol = WindowProtocol::X11;
			handle.x11.dpy = info.info.x11.display;
			handle.x11.window = info.info.x11.window;
			break;
		
		#elif defined(SDL)
		case SDL_SYSWM_COCOA: // MacOSX Not supported
			handle.protocol = WindowProtocol::Unknow;
			break;

		#elif defined(SDL_VIDEO_DRIVER_WAYLAND)
		case SDL_SYSWM_WAYLAND: // Wayland not supported yet
			handle.protocol = WindowProtocol::Unknow;
			break;

		#elif defined(SDL_VIDEO_DRIVER_WINDOWS)
		case SDL_SYSWM_WINDOWS:
			handle.protocol = WindowProtocol::Windows;
			handle.windows.hinstance = info.info.win.hinstance;
			handle.windows.hwnd = info.info.win.window;
			break;

		#endif
		default:
			handle.protocol = WindowProtocol::Unknow;
			break;
		}

		return handle;
	}

	int Window::handleInput(void* userdata, SDL_Event* event) {
		try {
			auto window{ static_cast<Window*>(userdata) };

			WindowEvent wEvent;
			wEvent.type = WindowEventType::Max;
			switch (event->type) {
			case SDL_WINDOWEVENT:
				switch (event->window.event) {
				case SDL_WINDOWEVENT_CLOSE:
					wEvent.type = WindowEventType::Quit;
					break;
				case SDL_WINDOWEVENT_RESIZED:
					wEvent.type = WindowEventType::Resized;
					break;
				}
				break;

			case SDL_KEYDOWN:
				break;

			case SDL_KEYUP:
				break;

			case SDL_MOUSEBUTTONDOWN:
				break;

			case SDL_MOUSEBUTTONUP:
				break;

			case SDL_MOUSEMOTION:
				break;
			}

			if (wEvent.type != WindowEventType::Max) {
				window->handleEvent(wEvent);
			}

		}
		catch (std::exception const& e) {
			std::cerr << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Unknow error occured" << std::endl;
		}

		return 0;
	}

	void Window::handleEvent(WindowEvent const& event) {
		m_eventHandler.dispatch(event);
	}

	void Window::destroy() {
		if (m_handle) {
			SDL_DestroyWindow(m_handle);
			SDL_DelEventWatch(handleInput, this);
			m_handle = nullptr;
		}
	}
}
