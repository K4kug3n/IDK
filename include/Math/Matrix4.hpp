#pragma once

#ifndef IDK_MATRIX4_HPP
#define IDK_MATRIX4_HPP

#include <string>

namespace Idk {

	template<typename T>
	class Matrix4 {
	public:
		Matrix4() = default;
		Matrix4(T a11, T a12, T a13, T a14,
				T a21, T a22, T a23, T a24,
				T a31, T a32, T a33, T a34,
				T a41, T a42, T a43, T a44);
		Matrix4(const Matrix4<T>& mat) = default;
		template<typename U> explicit Matrix4(const Matrix4<U>& mat);
		~Matrix4() = default;

		Matrix4 operator+(const Matrix4 & mat) const;
		Matrix4 operator+(T scale) const;
		Matrix4 operator-(const Matrix4& mat) const;
		Matrix4 operator-(T scale) const;
		Matrix4 operator*(const Matrix4& mat) const;
		Matrix4 operator*(T scale) const;

		Matrix4& operator+=(const Matrix4& mat);
		Matrix4& operator+=(T scale);
		Matrix4& operator-=(const Matrix4& mat);
		Matrix4& operator-=(T scale);
		Matrix4& operator*=(const Matrix4& mat);
		Matrix4& operator*=(T scale);

		bool operator==(const Matrix4& mat) const;

		std::string toString() const;

		T	a11, a12, a13, a14,
			a21, a22, a23, a24,
			a31, a32, a33, a34,
			a41, a42, a43, a44;
	};

	using Vector2i = Matrix4<int>;
	using Vector2ui = Matrix4<unsigned int>;
	using Vector2f = Matrix4<float>;
	using Vector2d = Matrix4<double>;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Idk::Matrix4<T> & mat);

#include "Matrix4.inl"

#endif


