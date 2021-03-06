#pragma once

#ifndef IDK_WINDOW_HPP
#define IDK_WINDOW_HPP

#include <Window/VideoMode.hpp>
#include <Window/WindowEvent.hpp>
#include <Window/EventHandler.hpp>
#include <Window/WindowHandle.hpp>

#include <Math/Vector2.hpp>

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

#include <string>
#include <queue>

namespace Idk {

	class Window
	{
	public:
		Window();
		Window(VideoMode const& mode, std::string const& title);
		~Window();

		static bool initialize();
		static void uninitialize();

		bool create(VideoMode const& mode, std::string const& title);
		void close();

		bool isOpen();

		void processEvent() const;

		Vector2ui size() const;

		EventHandler& getEventHandler();
		WindowHandle getHandle() const;

	private:
		static int handleInput(void* userdata, SDL_Event* event);

		void handleEvent(WindowEvent const& event);
		void destroy();

		EventHandler m_eventHandler;
		SDL_Window * m_handle;
		bool m_closed;
		Vector2ui m_size;
	};

}

#endif