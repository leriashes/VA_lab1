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

	int k = -1, str_start = 0, str_end = 1;
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
			k++;
		}

		while (f.tellg() < str_end)
		{
			f >> number;
			matrix_A[k].push_back(number);
		}

		getline(f, str);
	}

	k = 0; 

	//Заполнение столбца свободных членов
	while (!f.eof())
	{
		f >> number;
		matrix_A[k++].push_back(number);
	}

	f.close();

	bool end = false;

	for (int i = 1; i < k && !end; i++) 
	{
		if (matrix_A[0].size() != matrix_A[i].size())
			end = true;
	}

	if (k + 1 != matrix_A[0].size())
		end = true;

	/*if (k != matrix_B.size() || k != matrix_A[0].size())
		end = true;*/

	if (end) 
	{
		cout << "Всё плохо!";
		return -1;
	}

	for (int t = 0; t < k; t++)
	{
		double max = fabs(matrix_A[t][t]);
		int num = t;

		for (int i = t + 1; i < k; i++)
		{
			if (fabs(matrix_A[i][t]) > max)
			{
				max = fabs(matrix_A[i][t]);
				num = i;
			}
		}

		matrix_A[t].swap(matrix_A[num]);

		//int 
		//matrix_B[t].swap(matrix_B[num]);

		for (int i = t + 1; i < k; i++)
		{
			double coef = matrix_A[i][t] / matrix_A[t][t];

			for (int j = t; j <= k; j++)
			{
				matrix_A[i][j] = matrix_A[i][j] - matrix_A[t][j] * coef;
			}
		}
	}

	return 0;
}
