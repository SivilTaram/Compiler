#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Error {
public:
	Error();
	static void errorMessage(int errortype,int line);
	static int getErrorCount();
private:
	static bool is_success;
	static int error_count;
	static vector<string> error_messages;
};


#endif