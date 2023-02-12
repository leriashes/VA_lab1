#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//чтение файла
bool readFile(vector <vector <double> >& matrix)
{
	int str_start = 0, str_end = 1, str_num = -1;
	double number;
	bool result = false, end = false;
	string str;

	//открытие файла с исходными данными
	ifstream f("input.txt");

	//файл для чтения не открылся
	if (!f)
	{
		cout << " Не удалось открыть файл 'input.txt'" << endl;
		result = true;
	}

	if (!result)
	{
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
				str_num++;
			}

			while (f.tellg() < str_end)
			{
				f >> number;
				matrix[str_num].push_back(number);
			}

			getline(f, str);

			if (str_num > matrix[0].size()) {
				end = true;
			}
		}

		if (!end)
		{
			str_num = 0;

			//заполнение столбца свободных членов
			while (!f.eof())
			{
				f >> number;
				matrix[str_num++].push_back(number);
			}

			//проверка числа строк и столбцов
			for (int i = 1; i < str_num && !end; i++)
			{
				if (matrix[0].size() != matrix[i].size())
					end = true;
			}

			if (str_num + 1 != matrix[0].size())
				end = true;
		}

		f.close();

		if (end)
		{
			cout << "Неверный формат данных!" << endl;
			result = true;
		}

		int k = matrix.size();

		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < k; j++)
			{
				if (i == j)
				{
					matrix[i].push_back(1);
				}
				else
				{
					matrix[i].push_back(0);
				}
			}
		}
	}

	return result;
}

//вывод матрицы
void printMatrix(vector <vector <double> > matrix)
{
	int k = matrix.size();

	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			cout << matrix[i][j] << " ";
		}

		cout << endl;
	}

	return;
}

//вывод матрицы в файл
void fprintMatrix(vector <vector <double> > matrix, ofstream& file)
{
	int k = matrix.size();

	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			file << matrix[i][j] << " ";
		}

		file << endl;
	}

	return;
}

//вывод вектора
void printVector(vector <double> vec)
{
	int k = vec.size();

	for (int i = 0; i < k; i++)
	{
		cout << vec[i] << " " << endl;
	}

	cout << endl;

	return;
}

//вывод вектора
void printVector(vector <vector <double> > matrix, int column)
{
	for (int i = 0; i < column; i++)
	{
		cout << matrix[i][column] << " " << endl;
	}

	cout << endl;

	return;
}

//вывод вектора в файл
void fprintVector(vector <double> vec, ofstream& file)
{
	int k = vec.size();

	for (int i = 0; i < k; i++)
	{
		file << vec[i] << " " << endl;
	}

	file << endl;

	return;
}

//вывод вектора в файл
void fprintVector(vector <vector <double> > matrix, int column, ofstream& file)
{
	for (int i = 0; i < column; i++)
	{
		file << matrix[i][column] << " " << endl;
	}

	file << endl;

	return;
}


//приведение матрицы к треугольному виду с использованием выбора главного элемента
int matrixToTriangle(vector <vector <double> >& matrix)
{
	int k = matrix.size(), p = 0;

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

			for (int j = t; j <= k + k; j++)
			{
				matrix[i][j] = matrix[i][j] - matrix[t][j] * coef;
			}
		}
	}

	return p;
}

//обратный ход
vector <double> backSub(vector <vector <double> > triangle_matrix, int num)
{
	vector <double> x;
	int k = triangle_matrix.size();

	x.resize(k);

	for (int i = k - 1; i >= 0; i--)
	{
		double sum = 0;

		for (int j = k - 1; j > i; j--)
		{
			sum += triangle_matrix[i][j] * x[j];
		}

		x[i] = (triangle_matrix[i][num] - sum) / triangle_matrix[i][i];
	}

	return x;
}


//вычисление определителя
double determinant(vector <vector <double> > triangle_matrix, int num_change)
{
	double det = pow(-1, num_change);
	int k = triangle_matrix.size();

	for (int i = 0; i < k; i++)
	{
		det *= triangle_matrix[i][i];
	}

	return det;
}

//определение невязок
vector <double> residual(vector <vector <double> > matrix, vector <double> x)
{
	vector <double> r;
	int k = matrix.size();

	for (int i = 0; i < k; i++)
	{
		r.push_back(matrix[i][k]);

		for (int j = 0; j < k; j++)
		{
			r[i] -= matrix[i][j] * x[j];
		}
	}

	return r;
}

//поиск обратной матрицы
vector <vector <double> > inverseMatrix(vector <vector <double> > matrix)
{
	vector <vector <double> > inverse_matrix;

	int k = matrix.size();

	inverse_matrix.resize(k);

	for (int t = 0; t < k; t++)
	{
		vector <double> e;
		e = backSub(matrix, t + k + 1);

		for (int i = 0; i < k; i++)
		{
			inverse_matrix[i].push_back(e[i]);
		}
	}

	return inverse_matrix;
}

int main()
{
	setlocale(LC_ALL, "Rus");

	int k, p;

	vector <vector <double> > matrix;
	vector <vector <double> > triangle_matrix;
	vector <vector <double> > inverse_matrix;
	vector <double> x, r;

	if (readFile(matrix))
	{
		return -1;
	}

	ofstream result("result.txt");
	if (!result)
	{
		cout << "Не удалось открыть файл для записи результата!" << endl;
		return -1;
	}

	k = matrix.size();
	triangle_matrix = matrix;
	inverse_matrix = matrix;


	//вывод матрицы A
	cout << "Матрица A" << endl;
	printMatrix(matrix);

	result << "Матрица A" << endl;
	fprintMatrix(matrix, result);

	//вывод вектора B
	cout << "\nВектор B" << endl;
	printVector(matrix, k);

	result << "\nВектор B" << endl;
	fprintVector(matrix, k, result);

	p = matrixToTriangle(triangle_matrix); //приведение матрицы к треугольному виды

	//вывод треугольной матрицы
	cout << "Треугольная матрица" << endl;
	printMatrix(triangle_matrix);

	result << "Треугольная матрица" << endl;
	fprintMatrix(triangle_matrix, result);

	//вывод столбца свободных членов
	cout << "\nСтолбец свободных членов" << endl;
	printVector(triangle_matrix, k);

	result << "\nСтолбец свободных членов" << endl;
	fprintVector(triangle_matrix, k, result);

	//вычисление определителя
	double det = determinant(triangle_matrix, p);

	//вывод определителя
	if (det == 0)
	{
		cout << "Матрица вырождена! det A = 0" << endl;
		result << "Матрица вырождена! det A = 0" << endl;
	}
	else
	{
		cout << "det A = " << det << endl;
		result << "det A = " << det << endl;

		x = backSub(triangle_matrix, k); //обратный ход - поиск x

		//вывод вектора X
		cout << "\nВектор X" << endl;
		printVector(x);

		result << "\nВектор X" << endl;
		fprintVector(x, result);

		r = residual(matrix, x); //определение невязок

		//вывод невязок
		cout << "\nНевязки " << endl;
		printVector(r);

		result << "Невязки " << endl;
		fprintVector(r, result);

		//обратная матрица
		inverse_matrix = inverseMatrix(triangle_matrix);

		//вывод обратной матрицы
		cout << "\nОбратная матрица" << endl;
		printMatrix(inverse_matrix);

		result << "Обратная матрица" << endl;
		fprintMatrix(inverse_matrix, result);
	}

	result.close();

	return 0;
}
