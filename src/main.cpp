#include "matrix.h"

using namespace std;

int main() {
	aline::Matrix<int, 2, 2> m1 = { {1, 2} };
	aline::Matrix<int, 2, 2> m2(m1);

	cout << m1 << endl;
	cout << m2 << endl;

	cout << "apres Changement" << endl;

	m1[1][0] = 5;

	cout << m1 << endl;
	cout << m2 << endl;
}