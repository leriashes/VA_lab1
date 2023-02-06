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
		cout << " Не удалось открыть файл 'input.dat'" << endl;
		return -1;
	}

	int k = -1, str_start = 0, str_end = 1, p = 0;
	double number;
	string str;
	bool end = false;

	vector <vector <double> > matrix;
	vector <vector <double> > matrix_copy;
	vector <vector <double> > matrix_reversed;
	vector <double> x, r;

	//заполнение матрицы системы уравнений
	while (str_start != str_end && !end)
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

		if (k > matrix[0].size()) {
			end = true;
		}
	}

	if (!end)
	{
		k = 0;

		//заполнение столбца свободных членов
		while (!f.eof())
		{
			f >> number;
			matrix[k++].push_back(number);
		}

		//проверка числа строк и столбцов
		for (int i = 1; i < k && !end; i++)
		{
			if (matrix[0].size() != matrix[i].size())
				end = true;
		}

		if (k + 1 != matrix[0].size())
			end = true;
	}

	f.close();

	if (end) 
	{
		cout << "Неверный формат данных!" << endl;
		return -1;
	}

	matrix_copy = matrix;
	matrix_reversed = matrix;

	cout << "Матрица A (матрица системы)" << endl;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			cout << matrix[i][j] << " ";

			if (i == j)
			{
				matrix[i].push_back(1);
			}
			else
			{
				matrix[i].push_back(0);
			}
		}

		cout << endl;
	}

	cout << "\nВектор B" << endl;
	for (int i = 0; i < k; i++)
	{
		cout << matrix[i][k] << " " << endl;
	}
	cout << endl;


	//приведение матрицы к треугольному виду с использованием выбора главного элемента
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

		//перестановка строк
		if (t != num)
		{
			matrix[t].swap(matrix[num]); 
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

			matrix[i][t] = coef;
		}
	}

	/*cout << "Матрица" << endl;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			cout << matrix[i][j] << " ";
		}

		cout << endl;
	}

	cout << "\nМатрица A (матрица системы)" << endl;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < i; j++)
		{
			cout << "0 ";
		}

		for (int j = i; j < k; j++)
		{
			cout << matrix[i][j] << " ";
		}

		cout << endl;
	}

	cout << "\nВектор B" << endl;
	for (int i = 0; i < k; i++)
	{
		cout << matrix[i][k] << " " << endl;
	}
	cout << endl;*/

	//вычисление определителя
	double det = pow(-1, p);

	for (int i = 0; i < k; i++)
	{
		det *= matrix[i][i];
	}

	if (det == 0)
	{
		cout << "Матрица вырождена! det A = 0" << endl;
		return 0;
	}
	else
	{
		cout << "det A = " << det << endl;
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

	cout << "\nВектор X" << endl;
	for (int i = 0; i < k; i++)
	{
		cout << x[i] << " " << endl;
	}
	cout << endl;

	/*for (int i = 0; i < k; i++)
	{
		r[i] = matrix[i][k];

		for (int j = 0; j < k; j++)
		{
			r[i] -= matrix[i][j] * x[j];
		}
	}

	cout << "\nНевязки 1" << endl;
	for (int i = 0; i < k; i++)
	{
		cout << r[i] << " " << endl;
	}
	cout << endl;*/

	for (int i = 0; i < k; i++)
	{
		r[i] = matrix_copy[i][k];

		for (int j = 0; j < k; j++)
		{
			r[i] -= matrix_copy[i][j] * x[j];
		}
	}

	cout << "\nНевязки" << endl;
	for (int i = 0; i < k; i++)
	{
		cout << r[i] << " " << endl;
	}
	cout << endl;

	//обратная матрица
	for (int t = k + 1; t < 2 * k + 1; t++)
	{
		/*vector <double> e;
		e.resize(k, 0);
		e[t] = 1;*/

		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < i; j++)
			{
				matrix[i][t] -= matrix[i][j] * matrix[j][t];
			}
		}

		int a, b;
		//обратный ход
		for (int i = k - 1; i >= 0; i--)
		{
			double sum = 0;

			for (int j = k - 1; j > i; j--)
			{
				sum += matrix[i][j] * matrix_reversed[j][t - k - 1];
			}
			a = (matrix[i][t] - sum) / matrix[i][i];
			matrix_reversed[i][t - k - 1] = (matrix[i][t] - sum) / matrix[i][i];
			
		}
	}


	cout << "\nОбратная матрица" << endl;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			cout << matrix_reversed[i][j] << " ";
		}

		cout << endl;
	}

	return 0;
}
