#include <iostream>

#include <Window/Window.hpp>
#include <Renderer/Renderer.hpp>

int main(){
	Idk::Renderer renderer;
	if (!renderer.initialize()) {
		std::cerr << "Error: Can't initialize renderer" << std::endl;
		return 1;
	}

	Idk::Window::initialize();

	Idk::Window window{ Idk::VideoMode{ 640, 480 }, "Test" };

	Idk::EventHandler & eventHandler{ window.getEventHandler() };
	eventHandler.onQuit.connect([&]() {
		window.close();
	});

	while (window.isOpen()){
		window.processEvent();
	}
	

	Idk::Window::uninitialize();

	return 0;
}