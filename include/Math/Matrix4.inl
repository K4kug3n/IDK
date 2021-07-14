#include "Matrix4.hpp"

#include <sstream>

namespace Idk{
	template<typename T>
	Matrix4<T>::Matrix4(T a11, T a12, T a13, T a14,
						T a21, T a22, T a23, T a24,
						T a31, T a32, T a33, T a34,
						T a41, T a42, T a43, T a44) :
		a11(a11), a12(a12), a13(a13), a14(a14),
		a21(a21), a22(a22), a23(a23), a24(a24),
		a31(a31), a32(a32), a33(a33), a34(a34),
		a41(a41), a42(a42), a43(a43), a44(a44){}

	template<typename T>
	template<typename U>
	Matrix4<T>::Matrix4(const Matrix4<U>& mat):
		a11(static_cast<T>(mat.a11)), a12(static_cast<T>(mat.a12)), a13(static_cast<T>(mat.a13)), a14(static_cast<T>(mat.a14)),
		a21(static_cast<T>(mat.a21)), a22(static_cast<T>(mat.a22)), a23(static_cast<T>(mat.a23)), a24(static_cast<T>(mat.a24)),
		a31(static_cast<T>(mat.a31)), a32(static_cast<T>(mat.a32)), a33(static_cast<T>(mat.a33)), a34(static_cast<T>(mat.a34)),
		a41(static_cast<T>(mat.a41)), a42(static_cast<T>(mat.a42)), a43(static_cast<T>(mat.a43)), a44(static_cast<T>(mat.a44)) {}

	template<typename T>
	Matrix4<T> Matrix4<T>::operator+(const Matrix4& mat) const {
		return Matrix4<T>{ a11 + mat.a11, a12 + mat.a12, a13 + mat.a13, a14 + mat.a14,
						a21 + mat.a21, a22 + mat.a22, a23 + mat.a23, a24 + mat.a24, 
						a31 + mat.a31, a32 + mat.a32, a33 + mat.a33, a34 + mat.a34, 
						a41 + mat.a41, a42 + mat.a42, a43 + mat.a43, a44 + mat.a44};
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::operator+(T scale) const {
		return Matrix4<T>{ a11 + scale, a12 + scale, a13 + scale, a14 + scale,
			a21 + scale, a22 + scale, a23 + scale, a24 + scale,
			a31 + scale, a32 + scale, a33 + scale, a34 + scale,
			a41 + scale, a42 + scale, a43 + scale, a44 + scale};
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::operator-(const Matrix4& mat) const {
		return Matrix4<T>{ a11 - mat.a11, a12 - mat.a12, a13 - mat.a13, a14 - mat.a14,
			a21 - mat.a21, a22 - mat.a22, a23 - mat.a23, a24 - mat.a24,
			a31 - mat.a31, a32 - mat.a32, a33 - mat.a33, a34 - mat.a34,
			a41 - mat.a41, a42 - mat.a42, a43 - mat.a43, a44 - mat.a44};
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::operator-(T scale) const {
		return Matrix4<T>{ a11 - scale, a12 - scale, a13 - scale, a14 - scale,
			a21 - scale, a22 - scale, a23 - scale, a24 - scale,
			a31 - scale, a32 - scale, a33 - scale, a34 - scale,
			a41 - scale, a42 - scale, a43 - scale, a44 - scale};
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::operator*(const Matrix4& mat) const {
		return Matrix4<T>{ 
			a11 * mat.a11 + a12 * mat.a21 + a13 * mat.a31 + a14 * mat.a41,
			a11 * mat.a12 + a12 * mat.a22 + a13 * mat.a32 + a14 * mat.a42,
			a11 * mat.a13 + a12 * mat.a23 + a13 * mat.a33 + a14 * mat.a43,
			a11 * mat.a14 + a12 * mat.a24 + a13 * mat.a34 + a14 * mat.a44,

			a21* mat.a11 + a22 * mat.a21 + a23 * mat.a31 + a24 * mat.a41,
			a21* mat.a12 + a22 * mat.a22 + a23 * mat.a32 + a24 * mat.a42,
			a21* mat.a13 + a22 * mat.a23 + a23 * mat.a33 + a24 * mat.a43,
			a21* mat.a14 + a22 * mat.a24 + a23 * mat.a34 + a24 * mat.a44,

			a31* mat.a11 + a32 * mat.a21 + a33 * mat.a31 + a34 * mat.a41,
			a31* mat.a12 + a32 * mat.a22 + a33 * mat.a32 + a34 * mat.a42,
			a31* mat.a13 + a32 * mat.a23 + a33 * mat.a33 + a34 * mat.a43,
			a31* mat.a14 + a32 * mat.a24 + a33 * mat.a34 + a34 * mat.a44,

			a41* mat.a11 + a42 * mat.a21 + a43 * mat.a31 + a44 * mat.a41,
			a41* mat.a12 + a42 * mat.a22 + a43 * mat.a32 + a44 * mat.a42,
			a41* mat.a13 + a42 * mat.a23 + a43 * mat.a33 + a44 * mat.a43,
			a41* mat.a14 + a42 * mat.a24 + a43 * mat.a34 + a44 * mat.a44,
		};
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::operator*(T scale) const {
		return Matrix4<T>{ a11 * scale, a12 * scale, a13 * scale, a14 * scale,
			a21 * scale, a22 * scale, a23 * scale, a24 * scale,
			a31 * scale, a32 * scale, a33 * scale, a34 * scale,
			a41 * scale, a42 * scale, a43 * scale, a44 * scale};
	}

	template<typename T>
	Matrix4<T>& Matrix4<T>::operator+=(const Matrix4& mat) {
		a11 += mat.a11;
		a12 += mat.a12;
		a13 += mat.a13;
		a14 += mat.a14;

		a21 += mat.a21;
		a22 += mat.a22;
		a23 += mat.a23;
		a24 += mat.a24;

		a31 += mat.a31;
		a32 += mat.a32;
		a33 += mat.a33;
		a34 += mat.a34;

		a41 += mat.a41;
		a42 += mat.a42;
		a43 += mat.a43;
		a44 += mat.a44;

		return *this;
	}

	template<typename T>
	Matrix4<T>& Matrix4<T>::operator+=(T scale) {
		a11 += scale;
		a12 += scale;
		a13 += scale;
		a14 += scale;

		a21 += scale;
		a22 += scale;
		a23 += scale;
		a24 += scale;

		a31 += scale;
		a32 += scale;
		a33 += scale;
		a34 += scale;

		a41 += scale;
		a42 += scale;
		a43 += scale;
		a44 += scale;

		return *this;
	}

	template<typename T>
	Matrix4<T>& Matrix4<T>::operator-=(const Matrix4& mat) {
		a11 -= mat.a11;
		a12 -= mat.a12;
		a13 -= mat.a13;
		a14 -= mat.a14;

		a21 -= mat.a21;
		a22 -= mat.a22;
		a23 -= mat.a23;
		a24 -= mat.a24;

		a31 -= mat.a31;
		a32 -= mat.a32;
		a33 -= mat.a33;
		a34 -= mat.a34;

		a41 -= mat.a41;
		a42 -= mat.a42;
		a43 -= mat.a43;
		a44 -= mat.a44;

		return *this;
	}

	template<typename T>
	Matrix4<T>& Matrix4<T>::operator-=(T scale) {
		a11 -= scale;
		a12 -= scale;
		a13 -= scale;
		a14 -= scale;

		a21 -= scale;
		a22 -= scale;
		a23 -= scale;
		a24 -= scale;

		a31 -= scale;
		a32 -= scale;
		a33 -= scale;
		a34 -= scale;

		a41 -= scale;
		a42 -= scale;
		a43 -= scale;
		a44 -= scale;

		return *this;
	}

	template<typename T>
	Matrix4<T>& Matrix4<T>::operator*=(const Matrix4& mat) {
		Matrix4 copy{ *this };

		a11 = copy.a11 * mat.a11 + copy.a12 * mat.a21 + copy.a13 * mat.a31 + copy.a14 * mat.a41;
		a12 = copy.a11 * mat.a12 + copy.a12 * mat.a22 + copy.a13 * mat.a32 + copy.a14 * mat.a42;
		a13 = copy.a11 * mat.a13 + copy.a12 * mat.a23 + copy.a13 * mat.a33 + copy.a14 * mat.a43;
		a14 = copy.a11 * mat.a14 + copy.a12 * mat.a24 + copy.a13 * mat.a34 + copy.a14 * mat.a44;

		a21 = copy.a21 * mat.a11 + copy.a22 * mat.a21 + copy.a23 * mat.a31 + copy.a24 * mat.a41;
		a22 = copy.a21 * mat.a12 + copy.a22 * mat.a22 + copy.a23 * mat.a32 + copy.a24 * mat.a42;
		a23 = copy.a21 * mat.a13 + copy.a22 * mat.a23 + copy.a23 * mat.a33 + copy.a24 * mat.a43;
		a24 = copy.a21 * mat.a14 + copy.a22 * mat.a24 + copy.a23 * mat.a34 + copy.a24 * mat.a44;

		a31 = copy.a31 * mat.a11 + copy.a32 * mat.a21 + copy.a33 * mat.a31 + copy.a34 * mat.a41;
		a32 = copy.a31 * mat.a12 + copy.a32 * mat.a22 + copy.a33 * mat.a32 + copy.a34 * mat.a42;
		a33 = copy.a31 * mat.a13 + copy.a32 * mat.a23 + copy.a33 * mat.a33 + copy.a34 * mat.a43;
		a34 = copy.a31 * mat.a14 + copy.a32 * mat.a24 + copy.a33 * mat.a34 + copy.a34 * mat.a44;

		a41 = copy.a41 * mat.a11 + copy.a42 * mat.a21 + copy.a43 * mat.a31 + copy.a44 * mat.a41;
		a42 = copy.a41 * mat.a12 + copy.a42 * mat.a22 + copy.a43 * mat.a32 + copy.a44 * mat.a42;
		a43 = copy.a41 * mat.a13 + copy.a42 * mat.a23 + copy.a43 * mat.a33 + copy.a44 * mat.a43;
		a44 = copy.a41 * mat.a14 + copy.a42 * mat.a24 + copy.a43 * mat.a34 + copy.a44 * mat.a44;

		return *this;
	}

	template<typename T>
	Matrix4<T>& Matrix4<T>::operator*=(T scale) {
		a11 *= scale;
		a12 *= scale;
		a13 *= scale;
		a14 *= scale;

		a21 *= scale;
		a22 *= scale;
		a23 *= scale;
		a24 *= scale;

		a31 *= scale;
		a32 *= scale;
		a33 *= scale;
		a34 *= scale;

		a41 *= scale;
		a42 *= scale;
		a43 *= scale;
		a44 *= scale;

		return *this;
	}

	template<typename T>
	bool Matrix4<T>::operator==(const Matrix4& mat) const {
		return a11 == mat.a11 && a12 == mat.a12 && a13 == mat.a13 && a14 == mat.a14
			&& a21 == mat.a21 && a22 == mat.a22 && a23 == mat.a23 && a24 == mat.a24
			&& a31 == mat.a31 && a32 == mat.a32 && a33 == mat.a33 && a34 == mat.a34
			&& a41 == mat.a41 && a42 == mat.a42 && a43 == mat.a43 && a44 == mat.a44; // Epsilon for float
	}

	template<typename T>
	std::string Matrix4<T>::toString() const {
		std::stringstream stream;

		stream << "Matrix4(" << a11 << "," << a12 << "," << a13 << "," << a14
			<< "|" << a21 << "," << a22 << "," << a23 << "," << a24
			<< "|" << a31 << "," << a32 << "," << a33 << "," << a34
			<< "|" << a41 << "," << a42 << "," << a43 << "," << a44 << ")";

		return stream.str();
	}

}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Idk::Matrix4<T> & mat){
	return out << mat.toString();
}