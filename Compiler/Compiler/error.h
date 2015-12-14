#pragma once

#include <string>
#include <iostream>
#include <vector>

using namespace std;

static class Error {
public:
	Error();
	static int getErrorCount();
	static void errorMessage(int errortype, int line);
private:
	static bool is_success;
	static int error_count;
	static vector<string> error_messages;
};

