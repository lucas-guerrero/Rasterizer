#pragma once
#include "vector.h"

namespace aline {
	template <typename T, std::size_t M, std::size_t N>
	class Matrix {
		Vector<Vector<T, N>, M> matrix;

	public:
		Matrix() {
			matrix = Vector<Vector<T, N>, M>();
		}

		Matrix(std::initializer_list<Vector<T, N>> list): Matrix() {
			if (list.size() > M)
				throw std::runtime_error("Initializer element more than " + N);

			int id = 0;
			for (const Vector<T, N> i : list) {
				matrix[id] = i;
				++id;
			}
		}

		Matrix(const Matrix<T, M, N>& m): Matrix() {
			for (std::size_t i = 0; i < M; ++i)
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
			return matrix[l];
		}

		Vector<T, N>& operator[](std::size_t l) {
			if (l >= M)
				throw std::runtime_error("parameter more than " + M);
			return matrix[l];
		}

		Matrix<T, M, N>& operator+=(const Matrix<T, M, N>& m) {
			for (std::size_t l = 0; l < M; ++l)
				matrix[l] += m[l];
			return *this;
		}

		Matrix<T, M, N>& operator=(const Matrix<T, M, N>& m) {
			for (std::size_t l = 0; l < M; ++l)
				matrix[l] = m[l];
			return *this;
		}
	};

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> inverse(const Matrix<T, M, N>& m) {
		Matrix<T, M, N> mPrime = Matrix<T, M, N>(m);
		// Cr�ation de la matrice identit� pour faire le pivot de Gauss-Jordan
		Matrix<T, M, N> id = Matrix<T, M, N>();
		for (std::size_t i = 0; i < M; ++i)
			id[i][i] = 1;

		for (std::size_t j = 0; j < N; ++j) {
			// Recherche de la plus grande valeur de la colonne
			std::size_t rowMax = j;
			T valueMax = mPrime[rowMax][j];
			for (std::size_t i = j+1; i < M; ++i) {
				T valueTmp = mPrime[i][j];
				if (valueTmp != 0 && abs(valueMax) < abs(valueTmp)) {
					rowMax = i;
					valueMax = valueTmp;
				}
			}
			
			if (valueMax == 0) {
				id[rowMax][id] = (T)NAN;
				return id;
			}
			// Changement des lignes si besoin
			if (j != rowMax) {
				std::swap(mPrime[j], mPrime[rowMax]);
				std::swap(id[j], id[rowMax]);
			}

			// Transformation de la ligne j pour la neutralis�
			mPrime[j] = mPrime[j] / valueMax;
			id[j] = id[j] / valueMax;

			// Simplification des autres lignes
			for (std::size_t r = 0; r < M; ++r) {
				if (r != j) {
					T div = (-mPrime[r][j]);
					mPrime[r] = mPrime[r] + mPrime[j] * div;
					id[r] = id[r] + id[j] * div;
				}
			}
		}

		return id;
	}

	template <typename T, std::size_t M, std::size_t N>
	bool isnan(const Matrix<T, M, N>& m) {
		for (std::size_t l = 0; l < M; ++l)
			if (isnan(m[l]))
				return true;
		return false;
	}

	template <typename T, std::size_t M, std::size_t N>
	bool operator==(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
		for (std::size_t l = 0; l < M; ++l)
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
		for (std::size_t l = 0; l < M; ++l) {
			Vector<T, N> v1 = m1[l];
			Vector<T, N> v2 = m2[l];
			if (!nearly_equal(v1, v2))
				return false;
		}
		return true;
	}

	template <typename T, std::size_t M, std::size_t N>
	std::ostream& operator<<(std::ostream& out, const Matrix<T, M, N>& m) {
		out << to_string(m) << std::endl;
		return out;
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> operator+(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
		Matrix<T, M, N> result;
		for (std::size_t l = 0; l < M; ++l)
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
		for (std::size_t l = 0; l < M; ++l)
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
		for (std::size_t l = 0; l < M; ++l) {
			T t = T();
			for (std::size_t c = 0; c < N; ++c)
				t += m[l][c] * vector[c];
			result[l] = t;
		}
		return result;
	}

	template <typename T, std::size_t M, std::size_t N, std::size_t O>
	Matrix<T, M, O> operator*(const Matrix<T, M, N>& m1, const Matrix<T, N, O>& m2) {
		Matrix<T, M, O> result;
		for (std::size_t l1 = 0; l1 < M; ++l1) {
			for (std::size_t c2 = 0; c2 < O; ++c2) {
				T t = T();
				for (std::size_t n = 0; n < N; ++n)
					t += m1[l1][n] * m2[n][c2];
				result[l1][c2] = t;
			}
		}
		return result;
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, M, N> operator/(const Matrix<T, M, N>& m, const T& scalar) {
			return (1 / scalar) * m;
	}

	template <typename T, std::size_t M, std::size_t N>
	std::string to_string(const Matrix<T, M, N>& m) {
		std::stringstream ss;
		ss << "(" << m[0];
		for (std::size_t i = 1; i < M; ++i)
			ss << ", " << m[i];
		ss << ")";
		return ss.str();
	}

	template <typename T, std::size_t M, std::size_t N>
	Matrix<T, N, M> transpose(const Matrix<T, M, N>& m) {
		Matrix<T, N, M> result;
		for (std::size_t l = 0; l < M; ++l)
			for (std::size_t c = 0; c < N; ++c)
				result[c][l] = m[l][c];
		return result;
	}
}