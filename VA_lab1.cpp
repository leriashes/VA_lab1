﻿#define _CRT_SECURE_NO_WARNINGS
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

	int k = -1, str_start = 0, str_end = 1, p = 0;
	double number;
	string str;

	vector <vector <double> > matrix;
	vector <vector <double> > matrix_copy;
	vector <double> x, r;

	//заполнение матрицы системы уравнений
	while (str_start != str_end)
	{
		str_start = f.tellg();
		getline(f, str);
		str_end = f.tellg();
		str_end -= 2;
		f.seekg(str_start);

		if (str_start != str_end)
		{
			matrix.push_back(vector <double>());
			k++;
		}

		while (f.tellg() < str_end)
		{
			f >> number;
			matrix[k].push_back(number);
		}

		getline(f, str);
	}

	k = 0; 

	//заполнение столбца свободных членов
	while (!f.eof())
	{
		f >> number;
		matrix[k++].push_back(number);
	}

	f.close();

	matrix_copy = matrix;

	//проверка числа строк и столбцов
	bool end = false;

	for (int i = 1; i < k && !end; i++) 
	{
		if (matrix[0].size() != matrix[i].size())
			end = true;
	}

	if (k + 1 != matrix[0].size())
		end = true;

	if (end) 
	{
		cout << "Всё плохо!";
		return -1;
	}

	//приведение к треугольной матрице с использованием выбора главного элемента
	for (int t = 0; t < k - 1; t++)
	{
		double max = fabs(matrix[t][t]);
		int num = t;

		//поиск главного элемента в столбце (наибольшего по модулю)
		for (int i = t + 1; i < k; i++)
		{
			if (fabs(matrix[i][t]) > max)
			{
				max = fabs(matrix[i][t]);
				num = i;
			}
		}

		if (t != num)
		{
			matrix[t].swap(matrix[num]); //перестановка строк
			p++;
		}

		//прямой ход
		for (int i = t + 1; i < k; i++)
		{
			double coef = matrix[i][t] / matrix[t][t];

			for (int j = t; j <= k; j++)
			{
				matrix[i][j] = matrix[i][j] - matrix[t][j] * coef;

			}
		}
	}

	double det = pow(-1, p);

	for (int i = 0; i < k; i++)
	{
		det *= matrix[i][i];
	}

	x.resize(k);
	r.resize(k);

	//обратный ход
	for (int i = k - 1; i >= 0; i--) 
	{
		double sum = 0;

		for (int j = k - 1; j > i; j--)
		{
			sum += matrix[i][j] * x[j];
		}

		x[i] = (matrix[i][k] - sum) / matrix[i][i];
	}

	for (int i = 0; i < k; i++)
	{
		r[i] = matrix[i][k];

		for (int j = 0; j < k; j++)
		{
			r[i] -= matrix[i][j] * x[j];
		}
	}

	for (int i = 0; i < k; i++)
	{
		r[i] = matrix_copy[i][k];

		for (int j = 0; j < k; j++)
		{
			r[i] -= matrix_copy[i][j] * x[j];
		}
	}

	return 0;
}
