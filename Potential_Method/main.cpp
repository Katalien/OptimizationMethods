#include<iostream>
#include<cmath>
#include"PotentialsMethod.cpp"

int main() {
	vector<double> str1 = {1, 2, 4, 3};
	vector<double> str2 = {4, 3, 8, 5};
	vector<double> str3 = {2, 7, 6, 3};

	vector<double> storage = {8, 8, 10 };
	vector<double> needs = { 4, 6, 8, 8};
	vector<vector<double>> A = { str1, str2, str3 };
	PotentialsMethod* method = new PotentialsMethod(A, storage, needs);
	method->potentialMethod();
}