#include<iostream>
#include<vector>
#include<cmath>
#include<string>
#include <iomanip>
#include<sstream>
#include<utility>
#include<stdlib.h>
#include"GaussMethod.cpp"
#include"Cell.cpp"

using namespace std;

class FindPotentials {
private:
	vector<vector<Cell>> potentialMatrix = {};
	vector<vector<double>> costMatrix = {};

	vector<vector<double>> A = {};
	vector<double> b = {};
	vector<double> x = {};

	vector<double> u = {};
	vector<double> v = {};
	int n = 0;
	int eq_n = 0;

	shared_ptr<GaussMethod> gauss;


	void countNotEmptyCells() {
		for (int str = 0; str < potentialMatrix.size(); ++str) {
			for (int col = 0; col < potentialMatrix.at(0).size(); ++col) {
				if (potentialMatrix.at(str).at(col).isEmpty == false) {
					eq_n++;
				}
			}
		}
	}
	void convertToSlauForm() {
		int i = 0;
		for (int str = 0; str < potentialMatrix.size(); ++str) {
			for (int col = 0; col < potentialMatrix.at(0).size(); ++col) {
				if (potentialMatrix.at(str).at(col).isEmpty == false) {
					A.at(i).at(str) = 1.0;
					A.at(i).at(potentialMatrix.size() + col) = 1.0;
					b.push_back(costMatrix.at(str).at(col));
					i++;
				}
			}
		}
	}

	void printMatrix(vector<vector<double>> mat) {
		for (int str = 0; str < mat.size(); ++str) {
			for (int col = 0; col < mat.at(0).size(); ++col) {
				cout << mat.at(str).at(col) << " ";
			}
			cout << endl;
		}
		cout << endl << endl;
	}

	void splitPotentials(vector<double> x) {
		for (int i = 0; i < potentialMatrix.size(); ++i) {
			u.push_back(x.at(i));
		}
		for (int i = 0; i < potentialMatrix.at(0).size(); ++i) {
			v.push_back(x.at(i + potentialMatrix.size()));
		}
	}

public:
	FindPotentials(vector<vector<Cell>> potentialMatrix, vector<vector<double>> costMatrix) :
		potentialMatrix{ potentialMatrix }, costMatrix{costMatrix}{
		n = potentialMatrix.size() + potentialMatrix.at(0).size(); 
		countNotEmptyCells();
		//fill A with zeros
		for (int str = 0; str < eq_n; ++str) {
			vector<double> eq = {};
			for (int col = 0; col < n; ++col) {
				eq.push_back(0.0);
			}
			A.push_back(eq);
		}
		convertToSlauForm();
		//printMatrix(A);
		findPotentials();
	};

	void findPotentials() {
		gauss = make_shared<GaussMethod>(A, b);
		vector<double> potentials = gauss->getResult();
		splitPotentials(potentials);
	}

	vector<vector<double>> getA() { return A; };
	vector<double> getB() { return b; };
	vector<double> getU() { return u; };
	vector<double> getV() { return v; };

};