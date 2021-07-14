#pragma once

#ifndef IDK_VECTOR4_HPP
#define IDK_VECTOR4_HPP

#include <string>

namespace Idk {

	template<typename T>
	class Vector4 {
	public:
		Vector4() = default;
		Vector4(T x, T y, T z, T w);
		Vector4(const Vector4<T>& vec) = default;
		template<typename U> explicit Vector4(const Vector4<U>& vec);
		~Vector4() = default;

		T dotProduct(const Vector4 & vec);

		Vector4 operator+(const Vector4 & vec) const;
		Vector4 operator+(T scale) const;
		Vector4 operator-(const Vector4& vec) const;
		Vector4 operator-(T scale) const;
		Vector4 operator*(const Vector4& vec) const;
		Vector4 operator*(T scale) const;

		Vector4& operator+=(const Vector4& vec);
		Vector4& operator+=(T scale);
		Vector4& operator-=(const Vector4& vec);
		Vector4& operator-=(T scale);
		Vector4& operator*=(const Vector4& vec);
		Vector4& operator*=(T scale);

		bool operator==(const Vector4& vec) const;

		std::string toString() const;

		T x, y, z, w;
	};

	using Vector4i = Vector4<int>;
	using Vector4ui = Vector4<unsigned int>;
	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Idk::Vector4<T> & vec);

#include "Vector4.inl"

#endif


