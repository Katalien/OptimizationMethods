#include<iostream>
#include<vector>
#include<cmath>
#include<string>
#include <iomanip>
#include<sstream>
#include<utility>
#include<stdlib.h>
#include"FindPotentials.cpp"
//#include"Cell.cpp"

using namespace std;

typedef enum printAtribute{
	VALUE,
	DELTA
}printAtribute;

class PotentialsMethod {

public:
	PotentialsMethod(vector<vector<double>> _costMatrix, vector<double> _storageCapacity, vector<double> _needsVolume) :
		costMatrix{ _costMatrix }, storage{ _storageCapacity }, needs{ _needsVolume } {
		if (!checkIsClosed()) {
			//	makeClosed();
		}
		createPotentialMatrix();
		curNeeds = needs;
		curStorage = storage;
		printTable(potentialMatrix, curStorage, curNeeds, printAtribute::VALUE);
		for (int i = 0; i < storage.size(); ++i) {
			u.push_back(INFINITY);
		}
		for (int i = 0; i < needs.size(); ++i) {
			v.push_back(INFINITY);
		}
	};

	void firstPlan() {
		double curStr = 0;
		double curCol = 0;
		int str = 0;
		int col = 0;
		int filledCells = 0;
		while (str < potentialMatrix.size() && col < potentialMatrix.at(0).size()) {
			potentialMatrix.at(str).at(col).value = min(curStorage.at(str), curNeeds.at(col));
			potentialMatrix.at(str).at(col).isEmpty = false;
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
		}
	}

	bool checkPlan(int filledCells) {
		return filledCells = providers + consumers - 1;
	}


	void findPotentials() {
		potentials = make_shared<FindPotentials>(potentialMatrix, costMatrix);
		u = potentials->getU();
		v = potentials->getV();
		/*u = { 8, 7 };
		v = { -4, -5, -3, 0 };*/
	}

	void potentialMethod() {
		firstPlan();
		cout << "FIRST PLAN" << endl;
		printTable(potentialMatrix, curStorage, curNeeds, printAtribute::VALUE);
		cout << "Target function = " << countTargetFunction() << endl;
		while (!isOptimal()) {
			findPotentials();
			fillEmptyCells();
			findMinIndex();
			vector<pair<int, int>> cycle = findCycle();
			//printTable(potentialMatrix, curStorage, curNeeds, printAtribute::VALUE);
			//printTable(potentialMatrix, curStorage, curNeeds, printAtribute::DELTA);		
			reorganaisePlan(cycle);
			cout << "Target function = " << countTargetFunction() << endl;
			printTable(potentialMatrix, curStorage, curNeeds, printAtribute::VALUE);		
		}
		optimalPlan = potentialMatrix;
		cout << endl << "RESULT:" << endl;
		printTable(optimalPlan, curStorage, curNeeds, printAtribute::VALUE);
		cout << "Target function = " << countTargetFunction() << endl;
	}

	double countTargetFunction() {
		double res = 0.0;
		for (int str = 0; str < potentialMatrix.size(); ++str) {
			for (int col = 0; col < potentialMatrix.at(0).size(); ++col) {
				res += costMatrix.at(str).at(col) * potentialMatrix.at(str).at(col).value;
			}
		}
		return res;
	}

	void reorganaisePlan(vector<pair<int, int>> cycle) {
		double delta = min(potentialMatrix.at(cycle.at(1).first).at(cycle.at(1).second).value,
							potentialMatrix.at(cycle.at(3).first).at(cycle.at(3).second).value);

		potentialMatrix.at(cycle.at(0).first).at(cycle.at(0).second).value += delta;
		potentialMatrix.at(cycle.at(1).first).at(cycle.at(1).second).value -= delta;
		potentialMatrix.at(cycle.at(2).first).at(cycle.at(2).second).value += delta;
		potentialMatrix.at(cycle.at(3).first).at(cycle.at(3).second).value -= delta;

		potentialMatrix.at(cycle.at(0).first).at(cycle.at(0).second).isEmpty = false;

		if (potentialMatrix.at(cycle.at(1).first).at(cycle.at(1).second).value == 0) {
			potentialMatrix.at(cycle.at(1).first).at(cycle.at(1).second).isEmpty = true;
		}
		else if (potentialMatrix.at(cycle.at(3).first).at(cycle.at(3).second).value == 0) {
			potentialMatrix.at(cycle.at(3).first).at(cycle.at(3).second).isEmpty = true;
		}
		else{
			cout << "SOMETHING WENT WRONG";
		}
	}

	bool isOptimal() {
		for (int str = 0; str < potentialMatrix.size(); ++str) {
			for (int col = 0; col < potentialMatrix.at(0).size(); ++col) {
				if (potentialMatrix.at(str).at(col).isEmpty != false &&
					costMatrix.at(str).at(col) - (u.at(str) + v.at(col) ) < 0 ) {
					return false;
				}
			}
		}
		return true;
	}

	void findMinIndex() {
		double minVal = INFINITY;
		for (int str = 0; str < potentialMatrix.size(); ++str) {
			for (int col = 0; col < potentialMatrix.at(0).size(); ++col) {
				if (potentialMatrix.at(str).at(col).isEmpty ==true && potentialMatrix.at(str).at(col).delta < minVal) {
					minVal = potentialMatrix.at(str).at(col).delta;
					minIndStr = str;
					minIndCol = col;
				}
			}
		}
	}

	void fillEmptyCells() {
		for (int str = 0; str < potentialMatrix.size(); ++str) {
			for (int col = 0; col < potentialMatrix.at(0).size(); ++col) {
				if (potentialMatrix.at(str).at(col).isEmpty == true) {
					potentialMatrix.at(str).at(col).delta = costMatrix.at(str).at(col) - (u.at(str) + v.at(col));
					potentialMatrix.at(str).at(col).isEmpty = true;
				}
				else {
					potentialMatrix.at(str).at(col).isEmpty = false;
				}
			}
		}
	}

private:
	int consumers = 0;
	int providers = 0;
	vector<vector<double>> costMatrix = {};
	vector<vector<Cell>> potentialMatrix = {};
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
	vector<vector<Cell>> optimalPlan = {};

	shared_ptr<FindPotentials> potentials;
	shared_ptr<GaussMethod> gauss;


	void createPotentialMatrix() {
		for (int i = 0; i < costMatrix.size(); ++i) {
			vector<Cell> str = {};
			for (int j = 0; j < costMatrix.at(0).size(); ++j) {
				Cell cell = {};
				cell.value = -INFINITY;
				str.push_back(cell);
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
			potentialMatrix.at(str).at(col).value = 0.0;
			potentialMatrix.at(str).at(col).isEmpty = true;
		}
	};

	void fillZeroStr(int str, int fromCellCol) {
		for (int col = fromCellCol; col < potentialMatrix.at(0).size(); ++col) {
			potentialMatrix.at(str).at(col).value = 0.0;
			potentialMatrix.at(str).at(col).isEmpty = true;
		}
	}


	// fill cycle from the first element (with min str and col index ) consequansely
	vector<pair<int, int>> findCycle() {
		vector<pair<int, int>> cycle = { {minIndStr, minIndCol} };
		int curStr = cycle.at(cycle.size() - 1).first;
		int curCol = cycle.at(cycle.size() - 1).second;
		int newStr = -1;
		int newCol = -1;
		for (int col = 0; col < potentialMatrix.at(0).size(); ++col) {
			if (col == curCol) {
				continue;
			}
			if (!potentialMatrix.at(curStr).at(col).isEmpty) {
				for (int str = 0; str < potentialMatrix.size(); ++str) {
					if (str == curStr) {
						continue;
					}
					if (!potentialMatrix.at(str).at(curCol).isEmpty && !potentialMatrix.at(str).at(col).isEmpty) {
						cycle.push_back({ curStr, col });
						cycle.push_back({ str, col });
						cycle.push_back({ str, curCol });
						break;
					}
					
				}
			}
			if (cycle.size() == 4) {
				break;
			}
			
		}	
		return cycle;
	}



	void printTable(vector<vector<Cell>> const& mat, vector<double> stor, vector<double> need, printAtribute atribute) {
		int width = 15;
		int precisionVal = 3;
		int amountOfCols = mat.at(0).size() + 2;
		std::cout.setf(ios::fixed);
		std::setprecision(precisionVal);
		int amountOfX = mat.at(0).size();

		if (atribute == printAtribute::DELTA) {
			cout << "DELTA TABLE" << endl;
		}
		else {
			cout << "VALUE TABLE" << endl;
		}

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
				if (atribute == printAtribute::DELTA && mat.at(str).at(col).delta != INFINITY) {
					text2.append("X     ");
					text2.append(to_string(mat.at(str).at(col).delta));
					
				}
				else if((atribute == printAtribute::VALUE && fabs(mat.at(str).at(col).value)>0.000001 )) {
					text2.append(to_string(mat.at(str).at(col).value));
				}
				text2.append("|");
				cout << std::setw(width) << text2;
			}
			string text3 = "";
			text3.append(to_string(stor.at(str)));
			text3.append("|");
			cout << std::setw(width) << text3;
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

		
		cout << endl << endl;
	};
};