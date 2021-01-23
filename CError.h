#pragma once
#include <string>
#include <stdexcept>     
using namespace std;
static const int NO_ERROR = 0;
static const int INVALID_X_SIZE = -100;
static const int INVALID_VALUE_X = -102;
static const int INVALID_DIMENSION = -103;
static const int INVALID_NEW_X = -104;
static const int INVALID_NEW_Y = -105;
static const int INVALID_FILE_NAME = -106;
static const int INVALID_MATRIX = -107;

class CError {
public: 
	CError() { iCode = NO_ERROR; }
	string sGetError() { return "Error code is: " + iCode; }
	int getError() { return iCode; }
	void setError(int newCode) { iCode = newCode; }
	void check() {
		if (iCode == NO_ERROR) return;
		throw invalid_argument(sGetError());
	}
private:
	int iCode;

};

