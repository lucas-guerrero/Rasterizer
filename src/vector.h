#ifndef VECTOR_DEF
#define VECTOR_DEF
#include <array>
#include <string>
#include <iostream>

namespace aline {
	template <typename T, std::size_t N>
	class Vector {
		std::array<T, N> vector;
	public:
		Vector() {
			vector = std::array<T, N>();
		}

		Vector(std::initializer_list<T> list) {
			if (list.size() > N)
				throw std::runtime_error("Initializer element more than " + N);

			vector = std::array<T, N>();
			int id = 0;
			for (const T i : list) {
				vector[id] = i;
				++id;
			}
		}

		Vector(const Vector<T, N>& v) {
			vector = std::array<T, N>();

			for (int i = 0; i < N; ++i)
				vector[i] = v[i];
		}

		T at(std::size_t id) {
			if (id >= N) {
				throw std::runtime_error("parameter more than " + N);
			}
			return vector[id];
		}

		T operator[](std::size_t id) const {
			if (id >= N)
				return vector[0];
			return vector[id];
		}

		T& operator[](std::size_t id) {
			if (id >= N)
				throw std::runtime_error("parameter more than " + N);
			return vector[id];
		}

		Vector<T, N>& operator+=(const Vector<T, N>& v) {
			for (int i = 0; i < N; ++i)
				vector[i] += v[i];
			return *this;
		}
	};

	// TODO à completer et envoyer mail o prof pour savoir si il faut suivre les le sujet ou les tests avec exemple de la norm

	template <typename T, std::size_t N>
	T norm(const Vector<T, N>& v) {
		return v[0];
	}

	template <typename T, std::size_t N>
	T sq_norm(const Vector<T, N>& v) {
		return v[0];
	}

	template <typename T, std::size_t N>
	Vector<T, N> cross(const Vector<T, N>& v1, const Vector<T, N>& v2) {
		return Vector<T, N>();
	}

	template <typename T, std::size_t N>
	T dot(const Vector<T, N> &v1, const Vector<T, N> &v2) {
		return v1[0];
	}

	template <typename T, std::size_t N>
	bool isnan(const Vector<T,N>& v) {
		return false;
	}

	template <typename T, std::size_t N>
	bool is_unit(const Vector<T, N>& v) {
		return false;
	}

	template <typename T, std::size_t N>
	bool nearly_equal(const Vector<T, N> &v1, const Vector<T, N> &v2) {
		return false;
	}

	template <typename T, std::size_t N>
	bool operator==(const Vector<T, N>& v1, const Vector<T, N>& v2) {
		return false;
	}

	template <typename T, std::size_t N>
	bool operator!=(const Vector<T, N>& v1, const Vector<T, N>& v2) {
		return false;
	}

	template <typename T, std::size_t N>
	std::ostream& operator<<(std::ostream &out, const Vector<T, N> v) {
		out << "{ " << v[0];
		for (int i = 1; i < N; ++i)
			out << ", " << v[i];
		out << " }" << std::endl;
		return out;
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator+(const Vector<T, N> &v1, const Vector<T, N> &v2) {
		return Vector<T, N>();
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator-(const Vector<T, N> &v) {
		return Vector<T, N>();
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator-(const Vector<T, N> &v1, const Vector<T, N> &v2) {
		return Vector<T, N>();
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator*(const T& scalar, const Vector<T, N>& v) {
		return Vector<T, N>();
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator*(const Vector<T, N>& v, const T& scalar) {
		return Vector<T, N>();
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator*(const Vector<T, N>& v1, const Vector<T, N>& v2) {
		return Vector<T, N>();
	}

	template <typename T, std::size_t N>
	Vector<T, N> operator/(const Vector<T, N>& v, const T& s) {
		return Vector<T, N>();
	}

	template <typename T, std::size_t N>
	std::string to_string(const Vector<T, N>& v) {
		return "";
	}

	template <typename T, std::size_t N>
	Vector<T, N> unit_vector(const Vector<T, N>& v) {
		return Vector<T, N>();
	}
}


#endif // !VECTOR_DEF
