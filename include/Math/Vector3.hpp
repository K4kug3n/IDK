#pragma once

#ifndef IDK_VECTOR3_HPP
#define IDK_VECTOR3_HPP

#include <string>

namespace Idk {

	template<typename T>
	class Vector3 {
	public:
		Vector3() = default;
		Vector3(T x, T y, T z);
		Vector3(const Vector3<T>& vec) = default;
		template<typename U> explicit Vector3(const Vector3<U>& vec);
		~Vector3() = default;

		T dotProduct(const Vector3 & vec);

		Vector3 operator+(const Vector3 & vec) const;
		Vector3 operator+(T scale) const;
		Vector3 operator-(const Vector3& vec) const;
		Vector3 operator-(T scale) const;
		Vector3 operator*(const Vector3& vec) const;
		Vector3 operator*(T scale) const;

		Vector3& operator+=(const Vector3& vec);
		Vector3& operator+=(T scale);
		Vector3& operator-=(const Vector3& vec);
		Vector3& operator-=(T scale);
		Vector3& operator*=(const Vector3& vec);
		Vector3& operator*=(T scale);

		bool operator==(const Vector3& vec) const;

		std::string toString() const;

		T x, y, z;
	};

	using Vector3i = Vector3<int>;
	using Vector3ui = Vector3<unsigned int>;
	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Idk::Vector3<T> & vec);

#include "Vector3.inl"

#endif


