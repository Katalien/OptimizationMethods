#include<iostream>
#include<vector>
#include <cmath>
#include<string>
#include <iomanip>
#include <sstream>

using namespace std;


// Solving Linear Programming task by Tabular Symplex Method
class Simplex {
public:
	typedef enum Target {
		MIN,
		MAX
	} Target;

	// Get task in canonical form, point min\max and get basis
	Simplex(vector<vector<double>> _A, vector<double> _b, vector<double> _c, Target targ, vector<int> _basics) :
		A{ _A }, b{ _b }, H{ _b }, target{ targ }, coeffs{ _c }, basics{ _basics }{
		for (auto& el : _c) {
			c.push_back(0 - el);
		}
		amountOfX = A.at(0).size();
		checkInitialBasis();
	}

	/// <summary>
	/// Solve LP task by tabular simplex method
	/// Print result vector and value of the target function
	/// </summary>
	void simpexMethod() {
		printTable();
		while (!isOptimal()) {
			findPivotColumn();
			findPivotStr();
			basics.at(pivotStrInd) = pivotColInd;
			reorganaiseTable();
			printTable();
		}
		getResult();
		printVector(result);
		cout << targetFunction();
	}

private:
	vector<double> c = {};
	vector<double> b = {};
	vector<vector<double>> A = {};
	Target target = Target::MIN;

	vector<int> basics = {};
	vector<double> coeffs = {};

	int amountOfX = 0;
	int iteration = 1;
	vector<double> H = {};
	int pivotColInd = -1;
	int pivotStrInd = -1;

	vector<double> result = {};


	

	/// <summary>
	/// Check that the basis vectors are normalized e=(0,0,1,..0)
	/// </summary>
	void checkInitialBasis() {
		vector<int> basToChange = {};
		for (int i = 0; i < basics.size(); ++i) {
			if (A.at(i).at(basics.at(i)) != 1) {
				basToChange.push_back(basics.at(i));
				continue;
			}
			for (int j = 0; j < basics.size(); ++j) {
				if (j != i && A.at(j).at(basics.at(i)) != 0) {
					basToChange.push_back(basics.at(i));
					break;
				}
			}
			
		}
		findBasis(basToChange);
	}


	/// <summary>
	/// If some of the basis vector are not normalized, put them in _basis vector and normalize it by reorganaising the matrix (table) 
	/// </summary>
	void findBasis(vector<int> _basics) {
		for (int i = 0; i < _basics.size(); ++i) {
			pivotStrInd = i;
			pivotColInd = _basics.at(i);
			// if one resolving element equels to 0.0, choose the next appropriate column for basis vector 
			// if there is no such appropriate vector (column) so the task is unlimited and has no solution
			if (fabs(A.at(pivotStrInd).at(pivotColInd)) < 0.00001) {
				for (int add = 1; add < basics.size() - i; add++) {
					bool isEqual = false;
					for (int j = 0; j < basics.size(); j++)
						if (_basics.at(i) + add == basics.at(j)) {
							isEqual = true;
							break;
					}
					if (!isEqual) {
						basics.at(i) = _basics.at(i) + 1;
						_basics.at(i) = basics.at(i);
						pivotColInd = _basics.at(i);
						break;
					}
				}
			}			
			reorganaiseTable();
		}
		pivotStrInd = -1;
		pivotColInd = -1;
	}

	// find the min/max coefficient in target function, there will be the pivot column of the table
	void findPivotColumn() {
		double pivotColVal = c.at(0);
		pivotColInd = 0;
		if (target == Target::MAX) {
			for (int i = 0; i < c.size(); ++i) {
				if (c.at(i) < 0 && abs(c.at(i)) > abs(pivotColVal)) {
					pivotColVal = c.at(i);
					pivotColInd = i;
				}
			}
		}
		if (target == Target::MIN) {
			for (int i = 0; i < c.size(); ++i) {
				if (c.at(i) > 0 && abs(c.at(i)) > abs(pivotColVal)) {
					pivotColVal = c.at(i);
					pivotColInd = i;
				}
			}
		
		}
	};

	/// <summary>
	// Find table's string with minimum ratio. It is the strictes limitation of our matrix. 
	// The index of this string will be the pivot string 
	/// </summary>
	void findPivotStr() {
		double minRatio = INFINITY;
		for (int i = 0; i < A.size(); i++) {
			if (A[i][pivotColInd] <= 0) {
				continue;
			}
			double ratio = H[i] / A[i][pivotColInd];
			if (ratio < minRatio) {
				minRatio = ratio;
				pivotStrInd = i;
			}
		}
	};

	/// <summary>
	/// Print table on current iteration
	/// First column B is basis on current iteration
	/// Second column H is free members of the task
	/// x_1 - x_n are coefficients of the matrix on current iteration
	/// The last string F is coefficients of the target function on current iteration
	/// </summary>
	void printTable() {
		int width = 10;
		int precisionVal = 3;
		int amountOfCols = A.at(0).size() + 2;
		cout.setf(ios::fixed);
		std::setprecision(precisionVal);

		cout <<  "Iteration ¹" << iteration << endl;

		// header
		std::cout << std::setfill('-') << std::setw(amountOfCols * width) << "" << std::endl;
		cout << std::setfill(' ') << std::setw(width) <<  "B|" << std::setw(width) << "H|";

		for (int i = 0; i < amountOfX; ++i) {
			string str = "x";
			str.append(to_string(i));
			str.append("|");
			cout <<std::setw(width) << str;
		}
		cout << endl;
		std::cout << std::setfill('-') << std::setw(amountOfCols * width) << "" << std::endl;

		for (int i = 0; i < basics.size(); ++i) {
			string str1 = "x";
			str1.append(to_string(basics.at(i)));
			str1.append("|");
			cout << std::setfill(' ') << setw(width) << str1;
			str1 = "";
			std::stringstream stream;
			stream << std::fixed << std::setprecision(precisionVal) << H.at(i);
			str1 = stream.str();
			str1.append("|");
			cout << std::setfill(' ') << setw(width) << str1;
			for (auto const& el : A[i]) {
				str1 = "";
				std::stringstream stream;
				stream << std::fixed << std::setprecision(precisionVal) << el;
				str1 = stream.str();
				//str1.append(to_string(el));
				str1.append("|");
				cout  << setw(width) << str1;
			}
			str1 = "";
			std::cout << std:: endl << std::setfill('-') << std::setw(amountOfCols * width) << "" << std::endl;
			/*cout << endl;*/
		}
		cout << std::setfill(' ') << setw(width) << "F|";
		cout << std::setw(width) << "|";
		for (auto const& el : c) {
			string str = "";
			std::stringstream stream;
			stream << std::fixed << std::setprecision(precisionVal) << el;
			str = stream.str();
			//str.append(to_string(el));
			str.append("|");
			cout << std::setfill(' ') << setw(width) << str;
		}
		std::cout << std::endl << std::setfill('-') << std::setw(amountOfCols * width) << "" << std::endl;
		cout << endl << endl;
		iteration++;
	};

	
	/// <summary>
	/// Change matrix (table) coeffitionts on each iteration
	/// 
	/// </summary>
	void reorganaiseTable() {
		double resolvingEl = A.at(pivotStrInd).at(pivotColInd);
		vector<vector<double>> newA = {};
		vector<double> newH = {};
		vector<double> newC = {};

		vector<double> resolvingStr = A.at(pivotStrInd);

		// fill resolving string (divide pivot string elements on resolving element)
		for (int i = 0; i < resolvingStr.size(); ++i) {
			resolvingStr.at(i) = A.at(pivotStrInd).at(i) / resolvingEl;
		}
		
		// resolving element = 1
		resolvingStr.at(pivotColInd) = 1.0;

		for (int str = 0; str < A.size(); ++str) {
			if (str == pivotStrInd) {
				newA.push_back(resolvingStr);
				continue;
			}
			vector<double> newStr = {};
			for (int col = 0; col < A.at(str).size(); ++col) {
				if (col == pivotColInd) {
					newStr.push_back(0.0);
					continue;
				}
				newStr.push_back(A.at(str).at(col) - A.at(pivotStrInd).at(col) * A.at(str).at(pivotColInd) / resolvingEl);
			}
			newA.push_back(newStr);
		}

		// fill new c vector
		for (int i = 0; i < c.size(); ++i) {
			if (i == pivotColInd) {
				newC.push_back(0.0);
				continue;
			}
			//cout << c.at(i) << " " << c.at(pivotColInd) << " " << A.at(pivotStrInd).at(i) << " " << resolvingEl << endl;
			newC.push_back(c.at(i) - c.at(pivotColInd) * A.at(pivotStrInd).at(i) / resolvingEl);
		}

		//fill new H column
		for (int i = 0; i < H.size(); ++i) {
			if (i == pivotStrInd) {
				newH.push_back(H.at(i) / resolvingEl);
				continue;
			}
			newH.push_back(H.at(i) - H.at(pivotStrInd) * A.at(i).at(pivotColInd) / resolvingEl);
		}
		A = newA;
		H = newH;
		c = newC;
		H = newH;
	}

	/// <summary>
	/// Check if the solution on current iteration is opimal
	/// For MIN target function the condition is no negative elements. For MAX - mo positive ones
	/// </summary>
	bool isOptimal() {
		if (target == Target::MIN) {
			for (auto const& el : c) {
				if (el > 0) {
					return false;
				}
			}
		}
		else {
			for (auto const& el : c) {
				if (el < 0) {
					return false;
				}
			}
		}
		return true;
	};

	// Get result vector of the task
	void getResult() {
		for (int i = 0; i < amountOfX; ++i){
			result.push_back(0.0);
		}
		for (int i = 0; i < basics.size(); ++i) {
			result.at(basics.at(i)) = H.at(i);
		}
	}

	/// <summary>
	/// Get the value of the target funtion with current values
	/// </summary>
	double targetFunction() {
		double res = 0;
		for (int i = 0; i < coeffs.size(); ++i) {
			res += coeffs.at(i) * result.at(i);
		}
		return res;
	}

	/// <summary>
	/// Print vector which is fiven as a parametr
	/// </summary>
	void printVector(vector<double> vec) {
		for (auto const& el : vec) {
			cout << el << " ";
		}
		cout << endl;
	}
};