#include <Util/Lib.hpp>

#include <Util/String.hpp>
#include <iostream>

#if defined(IDK_UNIX)
	#include <dlfcn.h>
#endif

namespace Idk{
	Lib::Lib(): 
		m_handle{ nullptr } {}

	Lib::Lib(std::filesystem::path const& path){
		load(path);
	}

	Lib::~Lib(){
		this->free();
	}

	bool Lib::load(std::filesystem::path const& path){
		#if defined IDK_WINDOWS
			m_handle = LoadLibrary(toWString(path.generic_u8string()).data());
		#elif defined IDK_UNIX
			m_handle = dlopen(path.c_str(), RTLD_NOW);
		//#elif defined IDK_MACOSX
		#endif

		if (!m_handle) {
			std::cerr << "Error occured during loading " << path.string() << std::endl;
			return false;
		}

		return true;
	}

	bool Lib::isValid() const {
		return m_handle;
	}

	LibFunction Lib::getSymbol(std::string const& name) const {
		#if defined IDK_WINDOWS
			return reinterpret_cast<LibFunction>(GetProcAddress(m_handle, name.data()));
		#elif defined IDK_UNIX
			return reinterpret_cast<LibFunction>(dlsym(m_handle, name.c_str()));
		//#elif defined IDK_MACOSX
		#endif
	}

	void Lib::free(){
		if (m_handle) {
			#if defined IDK_WINDOWS
				FreeLibrary(m_handle);
			#elif defined IDK_UNIX
				dlclose(m_handle);
			//#elif defined IDK_MACOSX
			#endif

			m_handle = nullptr;
		}
	}
}

