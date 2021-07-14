#pragma once

#ifndef IDK_LIB_HPP
#define IDK_LIB_HPP

#include <filesystem>

#if defined IDK_WINDOWS
	#include <Windows.h>

	using LibHandle = HMODULE;
#elif defined IDK_UNIX
	using LibHandle = void*;
#elif defined IDK_MACOSX
	#error "MACOSX reconignized but not supported"
#else
	#error "OS not supported"
#endif

namespace Idk {

	using LibFunction = void (*)(void);

	class Lib {
	public:
		Lib();
		Lib(std::filesystem::path const& path);
		~Lib();

		bool load(std::filesystem::path const& path);
		bool isValid() const;

		LibFunction getSymbol(std::string const& name) const;

		void free();

	private:
		LibHandle m_handle;
	};
		
}

#endif
