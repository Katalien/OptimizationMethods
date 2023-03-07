// first version
#include<iostream>
#include<vector>
#include<cmath>
#include<string>
#include <iomanip>
#include<sstream>
#include<utility>
#include<stdlib.h>
#include"FindPotentials.cpp"


using namespace std;


class PotentialMethod {

public:
	PotentialMethod(vector<vector<double>> _costMatrix, vector<double> _storageCapacity, vector<double> _needsVolume) :
		costMatrix{ _costMatrix }, storage{ _storageCapacity }, needs{ _needsVolume } {
		if (!checkIsClosed()) {
		//	makeClosed();
		}
		createPotentialMatrix();
		curNeeds = needs;
		curStorage = storage;
		printTable(potentialMatrix, curStorage, curNeeds);
	};

	void firstPlan() {
		double curStr = 0;
		double curCol = 0;
		int str = 0;
		int col = 0;
		int filledCells = 0;
		while (str < potentialMatrix.size()  && col < potentialMatrix.at(0).size() ) {
			potentialMatrix.at(str).at(col) = min(curStorage.at(str), curNeeds.at(col));
			double cur_st = curStorage.at(str);
			curStorage.at(str) = max(0.0, curStorage.at(str) - curNeeds.at(col));
			curNeeds.at(col) = max(0.0, curNeeds.at(col) - cur_st);
			filledCells++;
			if (fabs(curStorage.at(str)) < 0.00000001) {
				fillZeroStr(str, col + 1);
				str++;
			}
			else if (fabs(curNeeds.at(col)) < 0.00000001) {
				fillZeroCol(col, str + 1);
				col++;
			}
			printTable(potentialMatrix, curStorage, curNeeds);
			
		}
		cout << checkPlan(filledCells) << endl;
	}

	bool checkPlan(int filledCells) {
		return filledCells = providers + consumers - 1;
	}


	void findPotentials() {
		//potentials = make_shared<FindPotentials>(potentialMatrix);
		//u = potentials->getU();
		//v = potentials->getV();
		u = { 8, 7 };
		v = {-4, -5, -3, 0};
	}

	void potentialMethod() {
		firstPlan();
		findPotentials();
		fillEmptyCells();
		findMinIndex();
		printTable(potentialMatrix, curStorage, curNeeds);
	}

	void isOptimal() {
		// if there is no negative delta -> it is optimal
	}

	void findMinIndex(){
		double minVal = INFINITY;
		for (int str = 0; str < potentialMatrix.size(); ++str) {
			for (int col = 0; col < potentialMatrix.at(0).size(); ++col) {
				if (potentialMatrix.at(str).at(col) < minVal) {
					minVal = potentialMatrix.at(str).at(col);
					minIndStr = str;
					minIndCol = col;
				}
			}
		}
	}

	void fillEmptyCells() {
		for (int str = 0; str < potentialMatrix.size(); ++str) {
			for (int col = 0; col < potentialMatrix.at(0).size(); ++col) {
				if (potentialMatrix.at(str).at(col) == -INFINITY) {
					potentialMatrix.at(str).at(col) = costMatrix.at(str).at(col) - (u.at(str) + v.at(col));
				}
			}
		}
	}

private:
	int consumers = 0;
	int providers = 0;
	vector<vector<double>> costMatrix = {};
	vector<vector<double>> potentialMatrix = {};
	vector<double> storage = {};
	vector<double> needs = {};
	vector<double> curStorage = {};
	vector<double> curNeeds = {};
	vector <double> u = {};
	vector <double> v = {};
	bool isClosed = true;
	int minIndStr = -1;
	int minIndCol = -1;
	pair<int, int> firstPos = { 0, 0 };

	shared_ptr<FindPotentials> potentials;
	shared_ptr<GaussMethod> gauss;


	double countTargetFunction(){}

	void createPotentialMatrix() {
		for (int i = 0; i < costMatrix.size(); ++i) {
			vector<double> str = {};
			for (int j = 0; j < costMatrix.at(0).size(); ++j) {
				str.push_back(-INFINITY);
			}
			potentialMatrix.push_back(str);
		}
	}

	bool checkTaskCorrectness() {};

	double sumVector(vector<double> vec) {
		double sum = 0.0;
		for (auto const& el : vec) {
			sum += el;
		}
		return sum;
	}

	bool checkIsClosed() {
		return sumVector(storage) == sumVector(needs);
	};

	void addFictionalProvider(double fictionalStorage) {
		vector<double> fictionalProvider = {};
		for (int i = 0; i < costMatrix.at(0).size(); ++i) {
			fictionalProvider.push_back(0.0);
		}
		costMatrix.push_back(fictionalProvider);
		storage.push_back(fictionalStorage);
	};

	void addFictionalClient(double fictionalNeed) {
		for (auto& el : costMatrix) {
			el.push_back(0.0);
		}
	};

	void makeClosed() {
		double diffVolume = sumVector(storage) - sumVector(storage); 
		if (diffVolume > 0) {
			addFictionalProvider(diffVolume);
		}
		else if (diffVolume < 0) {
			addFictionalClient(0.0);// ?
		}
	};

	void fillZeroCol(int col, int fromCellStr) {
		for (int str = fromCellStr; str < potentialMatrix.size(); ++str) {
			potentialMatrix.at(str).at(col) = -INFINITY;
		}
	};

	void fillZeroStr(int str, int fromCellCol) {
		for (int col = fromCellCol; col < potentialMatrix.at(0).size(); ++col) {
			potentialMatrix.at(str).at(col) = -INFINITY;
		}
	}

	void printTable(vector<vector<double>> const& mat, vector<double> stor, vector<double> need) {
		int width = 10;
		int precisionVal = 3;
		int amountOfCols = mat.at(0).size() + 2;
		cout.setf(ios::fixed);
		std::setprecision(precisionVal);
		int amountOfX = mat.at(0).size();

		// header
		std::cout << std::setfill('-') << std::setw(amountOfCols * width) << "" << std::endl;
		//cout << std::setfill(' ') << std::setw(width) << "Consumer|";
		cout << std::setfill(' ') << std::setw(width) << "|";
		for (int i = 0; i < mat.at(0).size(); ++i) {
			string str = "B";
			str.append(to_string(i));
			str.append("|");
			cout << std::setw(width) << str;
		}
		cout << std::setfill(' ') << std::setw(width) << "Storage|" << endl;
		std::cout << std::setfill('-') << std::setw(amountOfCols * width) << "" << std::endl;
		for (int str = 0; str < mat.size(); ++str) {
			string text = "A";
			text.append(to_string(str));
			text.append("|");
			cout << std::setfill(' ') << std::setw(width) << text;
			for (int col = 0; col < mat.at(0).size(); ++col) {
				string text2 = "";
				text2.append(to_string(mat.at(str).at(col)));
				text2.append("|");
				cout << std::setw(width) << text2;
			}
			string text3 = "";
			text3.append(to_string(stor.at(str)));
			text3.append("|");
			cout << std::setw(width) << text3 ;
			//std::cout << std::setfill('-') << std::setw(amountOfCols * width) << "";
			if (u.size() != 0) {
				string text4 = "";
				text4.append(to_string(u.at(str)));
				//text4.append("|");
				cout << std::setw(width) << text4 << endl;
			}
			else {
				cout << endl;
			}
			std::cout << std::setfill('-') << std::setw(amountOfCols * width) << "" << std::endl;
		}

		cout << std::setfill(' ') << setw(width) << "Needs|";

		for (auto const& el : need) {
			string str = "";
			std::stringstream stream;
			stream << std::fixed << std::setprecision(precisionVal) << el;
			str = stream.str();
			//str.append(to_string(el));
			str.append("|");
			cout << std::setfill(' ') << setw(width) << str;
		}
		cout << endl;
		std::cout << std::setfill('-') << std::setw(amountOfCols * width) << "" << std::endl;
		
		if (v.size() != 0) {
			cout << std::setfill(' ') << std::setw(width) << "|";
			for (auto const& el : v) {
				string str = "";
				std::stringstream stream;
				stream << std::fixed << std::setprecision(precisionVal) << el;
				str = stream.str();
				//str.append(to_string(el));
				//str.append("|");
				cout << std::setfill(' ') << setw(width) << str;
			}
		}

		//cout << std::setfill(' ') << setw(width) << "|";
		//std::cout << std::endl << std::setfill('-') << std::setw(amountOfCols * width) << "" << std::endl;
		cout << endl << endl;
	};
};