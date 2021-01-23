#pragma once
#include <iostream>
#include "CError.h"
#include "Exception.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
static const int DEFAULT_COLUMNS_NUMBER = 3;
static const int DEFAULT_ROWS_NUMBER = 3;
static const double DEFAULT_MATRIX_VALUE = 0.0;


//sprawdz czy wszedzie wskazniki sa ok
using namespace std;
template <typename T> class CMatrix {
private:
	int rows, columns;
	T** pcMatrix;
public:
	~CMatrix();
	CMatrix();
	CMatrix(int rows, int columns, CError* pcError);
	CMatrix(const CMatrix<T>& copy);
	CMatrix(CMatrix<T>&& cOther);
	bool readFile(string fileName, CError *pcError);
	bool appendFromFile(string fileName, CError* pcError, T number);
	bool resizeFill(int rows, int columns, T val, CError* pcError);

	int iGetRows() { return rows; };
	int iGetColumns() { return columns; };
	bool bSetRows(int rows);
	bool bSetColumns(int columns);
	void wypelnienie();
	void wyswietl();
	bool bNotNumber(string value);

	bool resize(int rows, int columns, CError* pcError);
	CMatrix<T> add(const CMatrix<T>& cOther, CError* pcError);
	CMatrix<T> sub(const CMatrix<T>& cOther, CError* pcError);
	CMatrix<T> multNumber(T& number);
	CMatrix<T> mult(const CMatrix<T>& cOther, CError* pcError);
	CMatrix<T> transp();
	CMatrix<T> vectorCol(int column, CError* error);
	CMatrix<T> vectorRow(int row, CError* error);
	T scalar(CMatrix<T>& cOther, CError* pcError);
	T getVal(int row, int column, CError* pcError);
	bool setVal(int row, int column, T value, CError* error);
	bool identity();

	CMatrix<T> operator+(const CMatrix<T>& cOther);
	CMatrix<T> operator-(const CMatrix<T>& cOther);
	CMatrix<T> operator*(T& number);
	CMatrix<T> operator*(const CMatrix<T>& cOther);
	void operator=(CMatrix<T>&& cOther);

};


template <typename T>
CMatrix<T> :: ~CMatrix() {											//destruktor
	for (int i = 0; i < rows; i++) {
		delete[] pcMatrix[i];
	}
	delete[] pcMatrix;
}

template <typename T>												//konstruktor domyœlny
CMatrix<T> ::CMatrix() {
	rows = DEFAULT_ROWS_NUMBER;
	columns = DEFAULT_COLUMNS_NUMBER;
	pcMatrix = new T * [rows];
	for (int i = 0; i < rows; ++i)
		pcMatrix[i] = new T[columns];
}

template <typename T>												//konstruktor tworzenia o danych wymiarach
CMatrix<T> :: CMatrix(int rows, int columns, CError* pcError) {
	pcError->setError(NO_ERROR);
	if (rows <= 0) pcError->setError(INVALID_NEW_X);
	if (columns <= 0) pcError->setError(INVALID_NEW_Y);
	this->rows = rows;
	this->columns = columns;
	pcMatrix = new T * [rows];
	for (int i = 0; i < rows; ++i)
		pcMatrix[i] = new T[columns];
}


template <typename T>												//konstruktor kopiuj¹cy
CMatrix<T> :: CMatrix(const CMatrix<T>& copy) {
	rows = copy.rows;
	columns = copy.columns;
	pcMatrix = new T * [rows];
	for (int i = 0; i < rows; i++)
	{
		pcMatrix[i] = new T[columns];
		for (int j = 0; j < columns; j++) {
			pcMatrix[i][j] = copy.pcMatrix[i][j];
		}
	}
}

template <typename T>						//setter do kolumn
bool CMatrix<T> ::bSetColumns(int newColumns) {
	if (newColumns <= 0) return false;
	
	CMatrix<T> cNew(rows, newColumns);

	if (cNew.columns > columns) {
		for (int i = 0; i < cNew.rows; i++) {
			for (int j = columns; j < cNew.columns; j++) {
				cNew.pcMatrix[i][j] = T();
			}
		}
	}
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cNew.pcMatrix[i][j] = pcMatrix[i][j];
		}
	}

	
	
	for (int i = 0; i < rows; i++) {
		delete[] pcMatrix[i];
	}

	delete[] pcMatrix;

	columns = cNew.columns;
	rows = cNew.rows;
	*this = move(cNew);
	cNew.pcMatrix = nullptr;
	return true;
}

template <typename T>						//setter do rzedow
bool CMatrix<T> ::bSetRows(int newRows) {
	if (newRows <= 0) return false;

	CMatrix<T> cNew(newRows, columns);


	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cNew.pcMatrix[i][j] = pcMatrix[i][j];
		}
	}

	if (cNew.rows > rows) {
		for (int i = rows; i < cNew.rows; i++) {
			for (int j = 0; j < cNew.columns; j++) {
				cNew.pcMatrix[i][j] = T();
			}
		}
	}

	for (int i = 0; i < rows; i++) {
		delete[] pcMatrix[i];
	}

	delete[] pcMatrix;

	columns = cNew.columns;
	rows = cNew.rows;
	*this = move(cNew);
	cNew.pcMatrix = nullptr;
	return true;

}

template <typename T>												//konstruktor przenoszacy
CMatrix<T> :: CMatrix(CMatrix<T>&& cOther) {
	rows = cOther.rows;
	columns = cOther.columns;
	pcMatrix = cOther.pcMatrix;

	cOther.pcMatrix = nullptr;
	cOther.columns = 0;
	cOther.rows = 0;
}

template <typename T>
void CMatrix<T> ::wypelnienie() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			pcMatrix[i][j] = i+j;
		}
	}
}

template <typename T>
void CMatrix<T> ::wyswietl() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cout << pcMatrix[i][j]<< " ";
		}
		cout << endl;
	}
}

template<typename T>
CMatrix<T> CMatrix<T>::add(const CMatrix<T>& cOther, CError* pcError) {									//metoda dodawania macierzy
	pcError->setError(NO_ERROR);
	CMatrix<T> cNew(rows, columns, pcError);
	if (rows != cOther.rows || columns != cOther.columns) {
		pcError->setError(INVALID_DIMENSION);
		return move(cNew);
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cNew.pcMatrix[i][j] = pcMatrix[i][j] + cOther.pcMatrix[i][j];
		}
	}
	return move(cNew);
}

template <typename T>														//operator do dodawania macierzy
CMatrix<T> CMatrix<T> :: operator+(const CMatrix<T>& cOther) {
	CError error;
	CMatrix<T> cNew = add(cOther, &error);
	//if (error.getError() != NO_ERROR) throw Exception(error.sGetError());
	error.check();
	return move(cNew);
}

template<typename T>														//metoda odejmowania macierzy
CMatrix<T> CMatrix<T> :: sub(const CMatrix<T>& cOther, CError* pcError) {
	pcError->setError(NO_ERROR);
	if (rows != cOther.rows || columns != cOther.columns) {
		pcError->setError(INVALID_DIMENSION);
		CMatrix<T> cWrong(1, 1, pcError);
		return move(cWrong);
	}

	CMatrix<T> cNew(rows, columns, pcError);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cNew.pcMatrix[i][j] = pcMatrix[i][j] - cOther.pcMatrix[i][j];
		}
	}
	return move(cNew);
}


template <typename T>												//operator do odejmowania macierzy
CMatrix<T> CMatrix<T> :: operator-(const CMatrix<T>& cOther) {
	CError error;
	CMatrix<T> cNew = sub(cOther, &error);
	error.check();
	return move(cNew);
}

template <typename T>
void CMatrix<T> :: operator=(CMatrix<T>&& cOther) {				//operator przenosz¹cy
	if (this != &cOther) {
		rows = cOther.rows;
		columns = cOther.columns;
		pcMatrix = cOther.pcMatrix;
		cOther.rows = 0;
		cOther.columns = 0;
		cOther.pcMatrix = nullptr;
	}
}

template <typename T>
CMatrix<T> CMatrix<T> :: multNumber(T& number) {									//metoda mno¿enia przez liczbê
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			pcMatrix[i][j] = pcMatrix[i][j] * number;
		}
	}
	return move(*this);
}

template <typename T>
CMatrix<T> CMatrix<T> :: mult(const CMatrix<T>& cOther, CError* pcError) {									//metoda mno¿enia przez macierz
	pcError->setError(NO_ERROR);
	if (rows != cOther.rows || columns != cOther.columns) {
		pcError->setError(INVALID_DIMENSION);
		CMatrix<T> cWrong(1, 1);
		return move(cWrong);
	}

	CMatrix<T> cNew(rows, columns);
	
	for (int i = 0; i < rows; i++) {				
		for (int j = 0; j < cOther.columns; j++) {
			cNew.pcMatrix[i][j] = 0;
			for (int k = 0; k < columns; k++)
			{
				cNew.pcMatrix[i][j] += pcMatrix[i][k] * cOther.pcMatrix[k][j];
			}
		}
	}
	return move(cNew);
}

template <typename T>							//operator mnozenia przez liczbe
CMatrix<T> CMatrix<T> :: operator*(T& number) {
	return move(this->multNumber(number));
}

template <typename T>							//operator mnozenia przez macierz
CMatrix<T> CMatrix<T> :: operator*(const CMatrix<T>& cOther) {
	CError error;
	CMatrix<T> cNew = sub(cOther, &error);
	error.check();
	return move(cNew);
}

template <typename T>							//metoda zmiany wymiarów
bool CMatrix<T> ::resize(int newRows, int newColumns, CError* pcError) {
	pcError->setError(NO_ERROR);
	CMatrix<T> cNew(newRows, newColumns, pcError);
	if (newRows < 0) {
		pcError->setError(INVALID_NEW_X);
		return false;
	}
	if (newColumns < 0) {
		pcError->setError(INVALID_NEW_Y);
		return false;
	}


	
	T** newMatrix = new T * [newRows];
	for (int i = 0; i < newRows; i++) 
		newMatrix[i] = new T[newColumns];

	if (newRows > rows) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				newMatrix[i][j] = pcMatrix[i][j];
			}
		}

		for (int i = 0; i < rows; i++) {
			delete[] pcMatrix[i];
		}
		delete[] pcMatrix;
	}
	columns = newColumns;
	rows = newRows;
	pcMatrix = newMatrix;

	return true;
}

template <typename T>							//metoda transponowania
CMatrix<T> CMatrix<T> :: transp() {
	CError error;
	if (pcMatrix != nullptr) {
		CMatrix<T> cNew(columns, rows, &error);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++)
				cNew.pcMatrix[j][i] = this->pcMatrix[i][j];
		}

		return move(cNew);
	}
	//error.setError(INVALID_MATRIX);
	//error.check();
}

template <typename T>				//wektor z rzedu
CMatrix<T> CMatrix<T> :: vectorRow(int row, CError* pcError) {
	pcError->setError(NO_ERROR);
	CMatrix<T> cNew(1, columns);
	if (row < 1) pcError->setError(INVALID_VALUE_X);
	if (pcMatrix == nullptr) pcError->setError(INVALID_MATRIX);
	else {
		for (int i = 0; i < columns; i++) {
			cNew.pcMatrix[0][i] = pcMatrix[row - 1][i];
		}
	}
	
	return move(cNew);
}

template <typename T>				//wektor z kolumn
CMatrix<T> CMatrix<T> :: vectorCol(int column, CError* pcError) {
	pcError->setError(NO_ERROR);
	CMatrix<T> cNew(1, columns);
	if (column < 1) pcError->setError(INVALID_VALUE_X);
	if (pcMatrix == nullptr) pcError->setError(INVALID_MATRIX);
	else {
		for (int i = 0; i < rows; i++) {
			cNew.pcMatrix[0][i] = pcMatrix[i][column - 1];
		}
	}	

	return move(cNew);
}

template <typename T>				//wyliczenie iloczynu skalarnego
T CMatrix<T> :: scalar(CMatrix<T>& cOther, CError* pcError) {
	T value = (T) DEFAULT_MATRIX_VALUE;
	if (rows != 1 || cOther.rows != 1 || columns != cOther.columns) {
		pcError->setError(INVALID_DIMENSION);
		return NAN;
	}
	
	for (int i = 0; i < columns; i++) {
		value += pcMatrix[0][i] * cOther.pcMatrix[0][i];
	}
	
	return value;
}

template <typename T>				//pobranie wartosci z danego rzedu i kolumny
T CMatrix<T> :: getVal(int row, int column, CError* pcError) {
	pcError->setError(NO_ERROR);
	if (row < 1 || column < 1) {
		pcError->setError(INVALID_VALUE_X);
		return NAN;
	}
	//error.check();

	return pcMatrix[row - 1][column - 1];
}

template <typename T>						//tutaj moze sam bool
bool CMatrix<T> :: setVal(int row, int column, T value, CError* pcError) {
	pcError->setError(NO_ERROR);
	if (row < 1 || column < 1 || row > rows || column > columns) {
		pcError->setError(INVALID_DIMENSION);
		return false;
	}

	pcMatrix[row - 1][column - 1] = value;
	return true;
}

template <typename T>				//macierz 1 na przekatnej
bool CMatrix<T> ::identity() {
	if (rows != columns) return false;
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (i == j) pcMatrix[i][j] = 1;
			else pcMatrix[i][j] = 0;
		}
	}
	return true;
}

template <typename T>			//do sprawdzania czy w pliku wczytwywaniu z pliku jest cos innego niz liczba
bool CMatrix<T> ::bNotNumber(string value) {
	for (char i : value) {
		if (((int)i < 48 || (int)i > 57) && (int)i != 44 && (int)i != 46) return true;
	}
	return false;
}

template <typename T>			//do wczytywania z pliku
bool CMatrix<T> :: readFile (string fileName, CError* pcError) {
	pcError->setError(NO_ERROR);	
	vector<int> newRows;
	ifstream file;
	stringstream stringStream;
	string rowOfMatrix;
	T found;


	file.open(fileName);
	if (!file.good()) {
		pcError->setError(INVALID_FILE_NAME);
		return false;
	}

	string wiersz;
	while (getline(file, wiersz)) {				//do wyznaczenia wymiarow
		stringStream << wiersz;

		int numberRows = 0;					
		while (!stringStream.eof()) {
			stringStream >> rowOfMatrix;
			numberRows++;
			rowOfMatrix = "";
		}
		newRows.push_back(numberRows);
		stringStream.clear();

	}
	file.close();

	int newColumns = *max_element(newRows.begin(), newRows.end());

	for (int i = 0; i < rows; i++) {
		delete[] pcMatrix[i];
	}
	delete[] pcMatrix;

	rows = newRows.size();
	columns = newColumns;

	pcMatrix = new T * [rows];
	for (int i = 0; i < rows; i++)
		pcMatrix[i] = new T[columns];

	file.open(fileName);
	if (!file.good()) {
		pcError->setError(INVALID_FILE_NAME);
		return false;
	}

	int actualRow = 0;
	//if (pcError->getError == NO_ERROR) {
		while (getline(file, wiersz)) {
			stringStream << wiersz;

			for (int i = 0; i < columns; i++) {
				if (!stringStream.eof()) {
					stringStream >> rowOfMatrix;
					if (!bNotNumber(rowOfMatrix)) {
						stringstream(rowOfMatrix) >> found;
						pcMatrix[actualRow][i] = found;
					}
					else pcMatrix[actualRow][i] = (T) DEFAULT_MATRIX_VALUE;
					rowOfMatrix = " ";
				}
				else {
					pcMatrix[actualRow][i] = (T) DEFAULT_MATRIX_VALUE;
				}
			}
			actualRow++;
			stringStream.clear();
		}
	//}
	file.close();

	return true;
}

template <typename T>							//metoda zmiany wymiarów
bool CMatrix<T> ::resizeFill(int newRows, int newColumns, T val, CError* pcError) {
	pcError->setError(NO_ERROR);
	CMatrix<T> cNew(newRows, newColumns, pcError);
	if (newRows < 0) {
		pcError->setError(INVALID_NEW_X);
		return false;
	}
	if (newColumns < 0) {
		pcError->setError(INVALID_NEW_Y);
		return false;
	}

	T** newMatrix = new T * [newRows];
	for (int i = 0; i < newRows; i++)
		newMatrix[i] = new T[newColumns];


		for (int i = 0; i < newRows; i++) {
			for (int j = 0; j < newColumns; j++) {
				if (i < rows && j < columns) newMatrix[i][j] = pcMatrix[i][j];
				else newMatrix[i][j] = val;
			}
		}

		for (int i = 0; i < rows; i++) {
			delete[] pcMatrix[i];
		}
		delete[] pcMatrix;

		columns = newColumns;
		rows = newRows;
	pcMatrix = newMatrix;

	return true;
}

template <typename T>			//do wczytywania z pliku
bool CMatrix<T> :: appendFromFile(string fileName, CError* pcError, T number) {
	pcError->setError(NO_ERROR);
	CMatrix<T> cNew(rows, columns, pcError);
	cNew.readFile(fileName, pcError);
	int oldRows = rows;
	this->resizeFill(rows + cNew.rows, max(columns, cNew.columns), number, pcError);
	for (int i = oldRows; i < rows ; i++) {
		for (int j = 0; j < cNew.columns; j++) {
			 pcMatrix[i][j] = cNew.pcMatrix[i- oldRows][j];
		}
	}

	return true;

}