#pragma once

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
	char buf[LINEMAX] = "";

public:
	//values for token 
	Symbol sym;
	Lexer(string filename);
	void getch();
	Token getsym();
	void initialReserved();
};