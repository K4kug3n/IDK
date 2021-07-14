#pragma once

#ifndef IDK_STRING_HPP
#define IDK_STRING_HPP

#include <string>

namespace Idk {
	std::wstring toWString(std::string_view const& str);
}

#endif
