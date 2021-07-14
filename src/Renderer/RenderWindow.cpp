#include <Renderer/RenderWindow.hpp>

namespace Idk {
	RenderWindow::RenderWindow(VideoMode const& mode, std::string const& title){
		create(mode, title);
	}

	bool RenderWindow::create(VideoMode const& mode, std::string const& title){
		return Window::create(mode, title);
	}
}

