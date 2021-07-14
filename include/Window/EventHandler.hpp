#pragma once

#ifndef IDK_EVENTHANDLER_HPP
#define IDK_EVENTHANDLER_HPP

#include <Util/Signal.hpp>
#include <Window/WindowEvent.hpp>

namespace Idk {
	class EventHandler {
	public:
		EventHandler() = default;
		~EventHandler() = default;

		void dispatch(WindowEvent const& event);

		Signal<> onQuit;
	};
}

#endif