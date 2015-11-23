#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Error {
public:
	Error();
	int getErrorCount();
	static void errorMessage(int errortype, int line);
private:
	bool is_success;
	int error_count;
	static vector<string> error_messages;
};


#endif