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
		for (int l = 0; l < M; ++l)
			if (isnan(m[l]))
				return true;
		return false;
	}

	template <typename T, std::size_t M, std::size_t N>
	bool operator==(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
		for (int l = 0; l < M; ++l)
			if (m1[l] != m2[l])
				return false;
		return true;
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
	std::ostream& operator<<(std::ostream& out, const Matrix<T, M, N>& m) {
		out << to_string(m) << std::endl;
		return out;
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> operator+(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
		Matrix<T, M, N> result;
		for (int l = 0; l < M; ++l)
			result[l] = m1[l] + m2[l];
		return result;
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
		Matrix<T, M, N> result;
		for (int l = 0; l < M; ++l)
			result[l] = m[l] * scalar;
		return result;
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> operator*(const Matrix<T, M, N>& m, const T& scalar) {
		return scalar * m;
	}

	template <typename T, std::size_t M, std::size_t N>
	Vector<T, M> operator*(const Matrix<T, M, N>& m, const Vector<T,N>& vector) {
		Vector<T, M> result;
		for (int l = 0; l < M; ++l) {
			T t = T();
			for (int c = 0; c < N; ++c)
				t += m[l][c] * vector[c];
			result[l] = t;
		}
		return result;
	}

	template <typename T, std::size_t M, std::size_t N, std::size_t O>
	Matrix<T, M, O> operator*(const Matrix<T, M, N>& m1, const Matrix<T, N, O>& m2) {
		Matrix<T, M, O> result;
		for (int l1 = 0; l1 < M; ++l1) {
			for (int c2 = 0; c2 < O; ++c2) {
				T t = T();
				for (int n = 0; n < N; ++n)
					t += m1[l1][n] * m2[n][c2];
				result[l1][c2] = t;
			}
		}
		return result;
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
	std::string to_string(const Matrix<T, M, N>& m) {
		std::stringstream ss;
		ss << "(" << m[0];
		for (int i = 1; i < M; ++i)
			ss << ", " << m[i];
		ss << ")";
		return ss.str();
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, N, M> transpose(const Matrix<T, M, N>& m) {
		Matrix<T, N, M> result;
		for (int l = 0; l < M; ++l)
			for (int c = 0; c < N; ++c)
				result[c][l] = m[l][c];
		return result;
	}
}

#endif // !MATRIX_DEF