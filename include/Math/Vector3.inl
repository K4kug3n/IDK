#include "Vector3.hpp"

#include <sstream>

namespace Idk{
	template<typename T>
	Vector3<T>::Vector3(T x, T y, T z) :
		x(x), y(y), z(z){}

	template<typename T>
	template<typename U>
	Vector3<T>::Vector3(const Vector3<U>& vec) :
		x(static_cast<U>(vec.x)), y(static_cast<U>(vec.y)), z(static_cast<U>(vec.z)) {}


	template<typename T>
	T Vector3<T>::dotProduct(const Vector3& vec){
		return x * vec.x + y * vec.y + z * vec.z;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3& vec) const {
		return Vector3<T>{ x + vec.x, y + vec.y, z + vec.z };
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator+(T scale) const {
		return Vector3<T>{ x + scale, y + scale, z + scale };
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3& vec) const {
		return Vector3<T>{ x - vec.x, y - vec.y, z - vec.z };
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator-(T scale) const {
		return Vector3<T>{ x - scale, y - scale, z - scale };
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(const Vector3& vec) const {
		return Vector3<T>{ x * vec.x, y * vec.y, z * vec.z };
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(T scale) const {
		return Vector3<T>{ x * scale, y * scale, z * scale };
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator+=(T scale) {
		x += scale;
		y += scale;
		z += scale;

		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator-=(const Vector3& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator-=(T scale) {
		x -= scale;
		y -= scale;
		z -= scale;

		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator*=(const Vector3& vec) {
		x *= vec.x;
		y *= vec.y;

		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator*=(T scale) {
		x *= scale;
		y *= scale;
		z *= scale;

		return *this;
	}

	template<typename T>
	bool Vector3<T>::operator==(const Vector3& vec) const {
		return vec.x == x && vec.y == y && vec.z == z; // Epsilon for float
	}

	template<typename T>
	inline bool Vector3<T>::operator!=(const Vector3& vec) const {
		return !(*this == vec);
	}

	template<typename T>
	std::string Vector3<T>::toString() const {
		std::stringstream stream;

		stream << "Vector3(" << x << "," << y << "," << z << ")";

		return stream.str();
	}

}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Idk::Vector3<T> & vec){
	return out << vec.toString();
}