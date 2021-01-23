#include <iostream>
#include <fstream>
#include "CError.h"
#include "CMatrix.h"

using namespace std;
int main()
{
   /* CError error;
    CMatrix<double> x(2, 2, &error);
    CMatrix<double> y(2, 2, &error);
    x.wypelnienie();
    y.wypelnienie();
    //x = x.transp();
   // CError error;

    CMatrix<double>nowy = x + y;
   //nowy.wyswietl();
    nowy.resize(2, 3, &error);
    nowy.wypelnienie();
    nowy.wyswietl();
    nowy.transp();
    nowy.wyswietl();

    /*CMatrix<int> test = nowy.vectorCol(2);
    test.wyswietl();
    CMatrix<int> test2 = nowy.vectorCol(1);
    test2.wyswietl();

    int wynik = test.scalar(test2);
    cout << "Wynik: " << wynik;
    nowy.bSetColumns(4);
    nowy.wyswietl();
    cout << nowy.iGetColumns();
    
    nowy.readFile("tep.txt", &error);
    nowy.wyswietl();
    nowy = nowy - y;*/
    CError error;
    CMatrix<int> x(2, 2, &error);
    x.wypelnienie();
   // CMatrix<int> nowy("tep.txt", &error);

    x.appendFromFile("tep.txt", &error, 0);
    x.wyswietl();
}
