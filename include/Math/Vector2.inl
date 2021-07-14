#include "Vector2.hpp"

#include <sstream>

namespace Idk{
	template<typename T>
	Vector2<T>::Vector2(T x, T y) :
		x(x), y(y){}

	template<typename T>
	template<typename U>
	Vector2<T>::Vector2(const Vector2<U>& vec) :
		x(static_cast<U>(vec.x)), y(static_cast<U>(vec.y)) {}


	template<typename T>
	T Vector2<T>::dotProduct(const Vector2& vec){
		return x * vec.x + y * vec.y;
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator+(const Vector2& vec) const {
		return Vector2<T>{ x + vec.x, y + vec.y };
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator+(T scale) const {
		return Vector2<T>{ x + scale, y + scale };
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator-(const Vector2& vec) const {
		return Vector2<T>{ x - vec.x, y - vec.y };
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator-(T scale) const {
		return Vector2<T>{ x - scale, y - scale };
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator*(const Vector2& vec) const {
		return Vector2<T>{ x * vec.x, y * vec.y };
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator*(T scale) const {
		return Vector2<T>{ x * scale, y * scale };
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator+=(const Vector2& vec) {
		x += vec.x;
		y += vec.y;

		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator+=(T scale) {
		x += scale;
		y += scale;

		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator-=(const Vector2& vec) {
		x -= vec.x;
		y -= vec.y;

		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator-=(T scale) {
		x -= scale;
		y -= scale;

		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator*=(const Vector2& vec) {
		x *= vec.x;
		y *= vec.y;

		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator*=(T scale) {
		x *= scale;
		y *= scale;

		return *this;
	}

	template<typename T>
	bool Vector2<T>::operator==(const Vector2& vec) const {
		return vec.x == x && vec.y == y; // Epsilon for float
	}

	template<typename T>
	inline bool Vector2<T>::operator!=(const Vector2& vec) const {
		return !(*this == vec);
	}

	template<typename T>
	std::string Vector2<T>::toString() const {
		std::stringstream stream;

		stream << "Vector2(" << x << "," << y << ")";

		return stream.str();
	}

}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Idk::Vector2<T> & vec){
	return out << vec.toString();
}