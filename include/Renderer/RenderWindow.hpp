#pragma once

#ifndef IDK_RENDERWINDOW_HPP
#define IDK_RENDERWINDOW_HPP

#include <Window/Window.hpp>

namespace Idk {

	class RenderWindow : public Window {
	public:
		RenderWindow();
		RenderWindow(VideoMode const& mode, std::string const& title);
		RenderWindow(RenderWindow const&) = delete;
		RenderWindow(RenderWindow&&) = delete;
		~RenderWindow() = default;

		bool create(VideoMode const& mode, std::string const& title);

	private:

	};
}

#endif
