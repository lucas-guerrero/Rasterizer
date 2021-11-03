#ifndef MATRIX_DEF
#define MATRIX_DEF
#include "vector.h"

namespace aline {
	template <typename T, std::size_t M, std::size_t N>
	class Matrix {
		Vector<Vector<T, N>, M> matrix;

	public:
		Matrix() {
			matrix = Vector<Vector<T, N>, M>();
		}

		Matrix(std::initializer_list<Vector<T, N>> list) {
			if (list.size() > M)
				throw std::runtime_error("Initializer element more than " + N);

			matrix = Vector<Vector<T, N>, M>();
			int id = 0;
			for (const Vector<T, N> i : list) {
				matrix[id] = i;
				++id;
			}
		}

		Matrix(const Matrix<T, M, N>& m) {
			matrix = Vector<Vector<T, N>, M>();
			for (int i = 0; i < M; ++i)
				matrix[i] = m[i];
		}

		Vector<T, N> at(std::size_t l) {
			if (l >= M)
				throw std::runtime_error("parameter more than " + M);
			return matrix[l];
		}

		T at(std::size_t l, std::size_t c) {
			if (l >= M)
				throw std::runtime_error("parameter more than " + M);
			if (c >= N)
				throw std::runtime_error("parameter more than " + N);
			return matrix[l][c];
		}

		Vector<T, N> operator[](std::size_t l) const {
			if (l >= M)
				return matrix[0];
			return matrix[l];
		}

		Vector<T, N>& operator[](std::size_t l) {
			if (l >= M)
				throw std::runtime_error("parameter more than " + M);
			return matrix[l];
		}

		Matrix<T, M, N>& operator+=(const Matrix<T, M, N>& m) {
			for (int l = 0; l < M; ++l)
				for (int c = 0; c < N; ++c)
					matrix[l][c] += m[l][c];
			return *this;
		}
	};

	template <typename T, std::size_t M, std::size_t N>
	bool inverse(const Matrix<T, M, N>& m) {
		return false;
	}

	template <typename T, std::size_t M, std::size_t N>
	bool isnan(const Matrix<T, M, N>& m) {
		return false;
	}

	template <typename T, std::size_t M, std::size_t N>
	bool operator==(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
		return false;
	}

	template <typename T, std::size_t M, std::size_t N>
	bool operator!=(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
		return !(m1==m2);
	}

	template <typename T, std::size_t M, std::size_t N>
	bool nearly_equal(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
		return false;
	}

	template <typename T, std::size_t M, std::size_t N>
	std::ostream& operator<<(std::ostream& out, const Matrix<T, M, N> m) {
		out << to_string(m) << std::endl;
		return out;
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> operator+(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
		return Matrix<T, M, N>();
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> operator-(const Matrix<T, M, N>& m) {
		return (T)-1 * m;
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> operator-(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
		return m1 + (-m2);
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> operator*(const T& scalar, const Matrix<T, M, N>& m) {
		return Matrix<T, M, N>();
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> operator*(const Matrix<T, M, N>& m, const T& scalar) {
		return Matrix<T, M, N>();
	}

	template <typename T, std::size_t M, std::size_t N>
	Vector<T, M> operator*(const Matrix<T, M, N>& m, const Vector<T,N>& vector) {
		return Vector<T, M>();
	}

	template <typename T, std::size_t M, std::size_t N, std::size_t O>
	Matrix<T, M, O> operator*(const Matrix<T, M, N>& m1, const Matrix<T, N, O>& m2) {
		return Matrix<T, M, O>();
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> operator/(const Matrix<T, M, N>& m, const T& scalar) {
		if (round(scalar) == 0) {
			Matrix<T, M, N> r;
			for (int l = 0; l < M; ++l)
				for (int c = 0; c < N; ++c)
					r[l][c] = (T)NAN;
			return r;
		}
		else
			return (1 / scalar) * m;
	}

	template <typename T, std::size_t M, std::size_t N>
	std::string to_string(const Matrix<T, M, N> m) {
		std::stringstream ss;
		ss << "(" << m[0];
		for (int i = 1; i < M; ++i)
			ss << ", " << m[i];
		ss << ")";
		return ss.str();
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> transpose(const Matrix<T, M, N>& m) {
		return Matrix<T, M, N>();
	}
}

#endif // !MATRIX_DEF