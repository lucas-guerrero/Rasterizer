#pragma once
#include <array>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

namespace aline {

	template <typename T, std::size_t N>
	class Vector {
		std::array<T, N> vector;
	public:

		Vector() { vector = std::array<T, N>(); }

		Vector(std::initializer_list<T> list): Vector() {
			if (list.size() > N)
				throw std::runtime_error("Initializer element more than " + N);

			int id = 0;
			for (const T i : list) {
				vector[id] = i;
				++id;
			}
		}

		Vector(const Vector<T, N>& v): Vector() {
			for (std::size_t i = 0; i < N; ++i)
				vector[i] = v[i];
		}

		T at(std::size_t id) {
			if (id >= N) {
				throw std::runtime_error("parameter more than " + N);
			}
			return vector[id];
		}

		T operator[](std::size_t id) const {
			return vector[id];
		}

		T& operator[](std::size_t id) {
			if (id >= N)
				throw std::runtime_error("parameter more than " + N);
			return vector[id];
		}

		Vector<T, N>& operator+=(const Vector<T, N>& v) {
			for (std::size_t i = 0; i < N; ++i)
				vector[i] += v[i];
			return *this;
		}

		Vector<T, N>& operator=(const Vector<T, N>& v) {
			vector = std::array<T, N>();
			for (std::size_t i = 0; i < N; ++i)
				vector[i] = v[i];
			return *this;
		}
	};

	using uint = unsigned int;
	using real = double;
	using Vec2i = Vector<int, 2>;
	using Vec2r = Vector<real, 2>;
	using Vec3r = Vector<real, 3>;
	using Vec4r = Vector<real, 4>;

	template <typename T, std::size_t N>
	T norm(const Vector<T, N>& v) {
		return sqrt(sq_norm(v));
	}

	template <typename T, std::size_t N>
	T sq_norm(const Vector<T, N>& v) {
		T result = T();
		for (std::size_t i = 0; i < N; ++i)
			result = result + v[i] * v[i];
		return result;
	}

	template <typename T, std::size_t N>
	Vector<T, N> cross(const Vector<T, N>& v1, const Vector<T, N>& v2) {
		if (N < 3)
			throw std::runtime_error("Vector less than 3 elements");

		T e1 = v1[1] * v2[2] - v1[2] * v2[1];
		T e2 = v1[2] * v2[0] - v1[0] * v2[2];
		T e3 = v1[0] * v2[1] - v1[1] * v2[0];

		return Vector<T, N> { e1, e2, e3 };
	}

	template <typename T, std::size_t N>
	T dot(const Vector<T, N> &v1, const Vector<T, N> &v2) {
		T result = T();
		for (std::size_t i = 0; i < N; ++i)
			result += v1[i] * v2[i];
		return result;
	}

	template <typename T, std::size_t N>
	bool isnan(const Vector<T,N>& v) {
		for (std::size_t i = 0; i < N; ++i)
			if (std::isnan(v[i]))
				return true;
		return false;
	}

	template <typename T, std::size_t N>
	bool is_unit(const Vector<T, N>& v) {
		return round(norm(v)) == (T)1;
	}

	template <typename T, std::size_t N>
	bool nearly_equal(const Vector<T, N> &v1, const Vector<T, N> &v2) {
		const T epsilon = std::numeric_limits<T>::epsilon();
		for (std::size_t i = 0; i < N; ++i) {
			T dif = std::abs(v1[i] - v2[i]);

			T max = std::max(v1[i], v2[i]);

			if (dif / max > epsilon)
				return false;

		}
		return true;
	}

	template <typename T, std::size_t N>
	bool operator==(const Vector<T, N>& v1, const Vector<T, N>& v2) {
		for (std::size_t i = 0; i < N; ++i)
			if (v1[i] != v2[i])
				return false;
		return true;
	}

	template <typename T, std::size_t N>
	bool operator!=(const Vector<T, N>& v1, const Vector<T, N>& v2) {
		return !(v1 == v2);
	}

	template <typename T, std::size_t N>
	std::ostream& operator<<(std::ostream &out, const Vector<T, N>& v) {
		out << to_string(v);
		return out;
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator+(const Vector<T, N> &v1, const Vector<T, N> &v2) {
		Vector<T, N> result;
		for (std::size_t i = 0; i < N; ++i)
			result[i] = v1[i] + v2[i];
		return result;
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator-(const Vector<T, N> &v) {
		return (T)-1 * v;
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator-(const Vector<T, N> &v1, const Vector<T, N> &v2) {
		Vector<T, N> result;
		for (std::size_t i = 0; i < N; ++i)
			result[i] = v1[i] - v2[i];
		return result;
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator*(const T& scalar, const Vector<T, N>& v) {
		Vector<T, N> result;
		for (std::size_t i = 0; i < N; ++i)
			result[i] = scalar * v[i];
		return result;
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator*(const Vector<T, N>& v, const T& scalar) {
		return scalar * v;
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator*(const Vector<T, N>& v1, const Vector<T, N>& v2) {
		Vector<T, N> result;
		for (std::size_t i = 0; i < N; ++i)
			result[i] = v1[i] * v2[i];
		return result;
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator/(const Vector<T, N>& v, const T& s) {
			return (1/s) * v;
	}

	template <typename T, std::size_t N>
	std::string to_string(const Vector<T, N>& v) {
		std::stringstream res;
		res << std::fixed << std::setprecision(6) << "(" << v[0];
		for (std::size_t i = 1; i < N; ++i)
			res << ", " << v[i];
		res << ")";
		return res.str();
	}

	template <typename T, std::size_t N>
	Vector<T, N> unit_vector(const Vector<T, N>& v) {
		return v / norm(v);
	}
}
