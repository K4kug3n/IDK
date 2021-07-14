#include "Vector4.hpp"

#include <sstream>

namespace Idk{
	template<typename T>
	Vector4<T>::Vector4(T x, T y, T z, T w) :
		x(x), y(y), z(z), w(w){}

	template<typename T>
	template<typename U>
	Vector4<T>::Vector4(const Vector4<U>& vec) :
		x(static_cast<U>(vec.x)), y(static_cast<U>(vec.y)),
		z(static_cast<U>(vec.z)), w(static_cast<U>(vec.w)) {}


	template<typename T>
	T Vector4<T>::dotProduct(const Vector4& vec){
		return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
	}

	template<typename T>
	Vector4<T> Vector4<T>::operator+(const Vector4& vec) const {
		return Vector4<T>{ x + vec.x, y + vec.y, z + vec.z, w + vec.w };
	}

	template<typename T>
	Vector4<T> Vector4<T>::operator+(T scale) const {
		return Vector4<T>{ x + scale, y + scale, z + scale, w + scale };
	}

	template<typename T>
	Vector4<T> Vector4<T>::operator-(const Vector4& vec) const {
		return Vector4<T>{ x - vec.x, y - vec.y, z - vec.z, w - vec.w };
	}

	template<typename T>
	Vector4<T> Vector4<T>::operator-(T scale) const {
		return Vector4<T>{ x - scale, y - scale, z - scale, w - scale };
	}

	template<typename T>
	Vector4<T> Vector4<T>::operator*(const Vector4& vec) const {
		return Vector4<T>{ x * vec.x, y * vec.y, z * vec.z, w * vec.w };
	}

	template<typename T>
	Vector4<T> Vector4<T>::operator*(T scale) const {
		return Vector4<T>{ x * scale, y * scale, z * scale, w * scale };
	}

	template<typename T>
	Vector4<T>& Vector4<T>::operator+=(const Vector4& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;

		return *this;
	}

	template<typename T>
	Vector4<T>& Vector4<T>::operator+=(T scale) {
		x += scale;
		y += scale;
		z += scale;
		w += scale;

		return *this;
	}

	template<typename T>
	Vector4<T>& Vector4<T>::operator-=(const Vector4& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;

		return *this;
	}

	template<typename T>
	Vector4<T>& Vector4<T>::operator-=(T scale) {
		x -= scale;
		y -= scale;
		z -= scale;
		w -= scale;

		return *this;
	}

	template<typename T>
	Vector4<T>& Vector4<T>::operator*=(const Vector4& vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		w *= vec.w;

		return *this;
	}

	template<typename T>
	Vector4<T>& Vector4<T>::operator*=(T scale) {
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;

		return *this;
	}

	template<typename T>
	bool Vector4<T>::operator==(const Vector4& vec) const {
		return vec.x == x && vec.y == y
			&& vec.z == z && vec.w == w; // Epsilon for float
	}

	template<typename T>
	std::string Vector4<T>::toString() const {
		std::stringstream stream;

		stream << "Vector4(" << x << "," << y << "," << z << "," << w << ")";

		return stream.str();
	}

}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Idk::Vector4<T> & vec){
	return out << vec.toString();
}