#include <Window/EventHandler.hpp>

namespace Idk {

	void EventHandler::dispatch(WindowEvent const& event) {
		switch (event.type) {
		case WindowEventType::Quit:
			onQuit();
			break;
		case WindowEventType::Resized:
			break;
		}
	}

}

