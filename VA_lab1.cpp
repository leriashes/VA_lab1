#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Rus");

	//открытие файла с исходными данными
	ifstream f("input.dat");

	//файл для чтения не открылся
	if (!f)
	{
		cout << " Не удалось открыть файл 'input.dat'";
		return -1;
	}

	int i = -1, str_start = 0, str_end = 1;
	double number;
	string str;

	vector <vector <double> > matrix_A;
	vector <double> matrix_B;

	//Заполнение матрицы системы уравнений
	while (str_start != str_end)
	{
		str_start = f.tellg();
		getline(f, str);
		str_end = f.tellg();
		str_end -= 2;
		f.seekg(str_start);

		if (str_start != str_end)
		{
			matrix_A.push_back(vector <double>());
			i++;
		}

		while (f.tellg() < str_end)
		{
			f >> number;
			matrix_A[i].push_back(number);
		}

		getline(f, str);
	}

	//Заполнение столбца свободных членов
	while (!f.eof())
	{
		f >> number;
		matrix_B.push_back(number);
	}

	f.close();
}
