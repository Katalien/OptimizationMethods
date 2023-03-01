#include <iostream>
#include <vector>
#include "Simplex.cpp"



int main() {

	//                          x1 x2  x3  x4 x5  x6 x7 x8 x9 x10 x11 x12 x13
	/*std::vector<double> str1 = { 3, -3, 1, -1, 1, -1, -2, 2, 5, -1, 0 };
	std::vector<double> str2 = { 5, -5, 3, -3, 6, -6, -3, 1, 5, 0, 0 };
	std::vector<double> str3 = { 1, -1, 2, -2, 3, -3, -9, 2, 2, 0, -1 };
	std::vector<double> str4 = { 9, -9, 4, -4, 1, -1, -8, 1, 6, 0, 0 };
	std::vector<double> str5 = { 3, -3, 5, -5, 8, -8, -8, 4, 2, 0, 0 };
	std::vector<double> str6 = { 6, -6, 1, -1, 5, -5, -1, 1, 2, 0, 0 };


	std::vector<std::vector<double>> A = { str1, str2, str3, str4, str5, str6 };

	std::vector<double> c = { 2, -2, 1, -1, 5, -5, -8, 3, 7, 0, 0 };

	std::vector<double> b = { 8, 1, 3, 5, 6, 5};*/


	// Прямая задача 
	std::vector<double> str1 = { 3, 5, -5, 1, 9, -9, 3, -3, 6, -6, 0, 0, 0 };
	std::vector<double> str2 = { 1, 3, -3, 2, 4, -4, 5, -5, 1, -1, 0, 0, 0 };
	std::vector<double> str3 = { 1, 6, -6, 3, 1, -1, 8, -8, 5, -5, 0, 0, 0 };
	std::vector<double> str4 = { 2, 3, -3, 9, 8, -8, 8, -8, 1, -1, -1, 0, 0 };
	std::vector<double> str5 = { 2, 1, -1, 2, 1, -1, 4, -4, 1, -1, 0, 1, 0 };
	std::vector<double> str6 = { 5, 5, -5, 2, 6, -6, 2, -2, 2, -2, 0, 0, 1 };


	std::vector < std::vector < double>> A = { str1, str2, str3, str4, str5, str6 };

	std::vector<double> c = { 8, 1, -1, 3, 5, -5, 6, -6, 5, -5, 0, 0, 0 };

	std::vector<double> b = { 2, 1, 5, 8, 3, 7, };

	std::vector<int> basics = { 0, 1, 2, 10, 11, 12 };

	Simplex* simplex = new Simplex(A, b, c, Simplex::Target::MIN, basics);
	simplex->simpexMethod();

	// lnz 

}

//if (fabs(A.at(pivotStrInd).at(pivotColInd)) < 0.00001) {
//	for (int add = 1; add < basics.size() - i; add++) {
//		bool isEqual = false;
//		for (int j = 0; j < basics.size(); j++)
//			if (_basics.at(i) + add == basics.at(j)) {
//				isEqual = true;
//				break;
//			}
//		if (!isEqual) {
//			basics.at(i) = _basics.at(i) + 1;
//			_basics.at(i) = basics.at(i);
//			pivotColInd = _basics.at(i);
//			break;
//		}
//
//	}
//}