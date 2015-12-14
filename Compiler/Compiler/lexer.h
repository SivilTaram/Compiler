#pragma once
#include"error.h"
#define LINEMAX 1024
#define NMAX 20
using namespace std;

class Lexer {
	//reserved
private:
	reserved_map reserved;
	//one token.
	vector<char> token;
	char peek = ' ';
	//infile if the source code file;
	ifstream infile;
	int ll = 0;
	int cc = 0;
	int linenum = 0;
	char buf[LINEMAX];
	Error error_handle;

public:
	//values for token 
	Symbol sym;
	Lexer(string filename, Error& _error_handle);
	void getch();
	Token getsym();
	void initialReserved();
};