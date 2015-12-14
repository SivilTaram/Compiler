#pragma once

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Error {
public:
	Error();
	int getErrorCount();
	bool IsSuccess();
	void print();
	void errorMessage(int errortype, int line); 
	void errorMessage(int errortype, int line, string message1);
private:
	bool is_success;
	int error_count;
	vector<string> error_messages;
};

