#pragma once

#ifndef IDK_VIDEOMODE_HPP
#define IDK_VIDEOMODE_HPP

#include <vector>

namespace Idk {
	class VideoMode
	{
	public:
		VideoMode();
		VideoMode(unsigned int width, unsigned int height);

		static VideoMode getDesktopMode();
		static std::vector<VideoMode> getDisplayModes();

		unsigned int width;
		unsigned int height;
	};

	bool operator==(VideoMode const& l, VideoMode const& r);
	bool operator>(VideoMode const& l, VideoMode const& r);

}

#endif

