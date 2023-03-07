#include<iostream>
#include<vector>
#include<cmath>
#include<string>
#include <iomanip>
#include<sstream>
#include<utility>
#include<stdlib.h>

using namespace std;

class GaussMethod {
private: 
    int n = 0;
	vector<vector<double>> A = {};
	vector<double> b = {};
	vector<double> res = {};
public:
	GaussMethod(vector<vector<double>> _A, vector<double> b) : b{ b }{
        for (int str = 0; str < _A.size(); ++str) {
            vector<double> newStr = {};
            for (int col = 0; col < _A.at(0).size(); ++col) {
                if (col == 0) {
                    continue;
                }
                newStr.push_back(_A.at(str).at(col));
            }
            A.push_back(newStr);
        }
        printMatrix(A);
        n = A.at(0).size();
        solveSlau();
    };

    void printMatrix(vector<vector<double>> mat) {
        for (int str = 0; str < mat.size(); ++str) {
            for (int col = 0; col < mat.at(0).size(); ++col) {
                cout << mat.at(str).at(col) << " ";
            }
            cout << endl;
        }
        cout << endl << endl;
    }

	void solveSlau() {
        for (int i = 0; i < n; i++) {
            // Partial pivoting
            int max_row = i;
            for (int j = i + 1; j < n; j++) {
                if (abs(A[j][i]) > abs(A[max_row][i])) {
                    max_row = j;
                }
            }
            swap(A[i], A[max_row]);
            swap(b[i], b[max_row]);

            // Elimination
            for (int j = i + 1; j < n; j++) {
                double factor = A[j][i] / A[i][i];
                for (int k = i; k < n; k++) {
                    A[j][k] -= factor * A[i][k];
                }
                b[j] -= factor * b[i];
            }
        }

        // Back-substitution
        vector<double> x(n, 0.0);
        x[0] = 0.0;  // First variable is equal to zero
        for (int i = n - 1; i >=0; i--) {
            double sum = 0.0;
            for (int j = i + 1; j < n; j++) {
                sum += A[i][j] * x[j];
            }
            x[i] = (b[i] - sum) / A[i][i];
        }
        res.push_back(0.0);
        for (auto const& el : x) {
            res.push_back(el);
        }
        //res = x;
        //res.push_back(0.0);
	}

    void printResult() {
        for (int i = 0; i < n; i++) {
            cout << "x[" << i << "] = " << res[i] << endl;
        }
    }

    vector<double> getResult() { return res; }
};