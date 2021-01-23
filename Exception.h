#pragma once
#include <exception>
#include <string>
using namespace std;
class Exception : public exception {
private:
	string sMessege;
public:
	Exception(const string& messege) { sMessege = messege; }
	~Exception() {}
	virtual const char* what() const noexcept override {
		return sMessege.c_str();
	}
	string sGetMessege() const { return (sMessege); }
};



