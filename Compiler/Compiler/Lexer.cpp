#include <iostream>
#include <map>
#include <string>
#include <ctype.h>
#include <vector>
#include <fstream>
#include <iomanip>
#include "token.h"
#include "error.h"
#define LINEMAX 1024
#define NMAX 20

using namespace std;
//Overview:Lexer for reading ch and recongnise them as words.
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
	//for parser's watch;
	static Symbol sym;
	static string ident;
	Lexer(string filename) {
		//Read from source code.
		infile.open(filename);
		initialReserved();
		getch();
	};
	//Initial reserved_map;
	void initialReserved() {
		reserved["+"] = Symbol::plus;
		reserved["-"] = Symbol::minus;
		reserved["*"] = Symbol::times;
		reserved["/"] = Symbol::slash;
		//??¡Á¡Â¡¤?

		reserved["("] = Symbol::lparen;
		reserved["["] = Symbol::lsquare;
		reserved["]"] = Symbol::rsquare;
		reserved[")"] = Symbol::rparen;

		reserved[","] = Symbol::comma;
		reserved["."] = Symbol::period;
		reserved["="] = Symbol::eql;
		reserved["<"] = Symbol::lss;
		reserved[">"] = Symbol::gtr;
		reserved[";"] = Symbol::semicolon;
		reserved["'"] = Symbol::quote;
		reserved[":"] = Symbol::colon;

		reserved["integer"]		= Symbol::integersym;
		reserved["char"	  ]		= Symbol::charsym;
		reserved["array"  ]		= Symbol::arraysym;
		reserved["of"     ] 	= Symbol::ofsym;

		reserved["begin"  ]		= Symbol::beginsym;
		reserved["end"    ]		= Symbol::endsym;
		reserved["if"]			= Symbol::ifsym;
		reserved["then"]		= Symbol::thensym;
		reserved["else"]		= Symbol::elsesym;
		reserved["while"]		= Symbol::whilesym;
		reserved["do"]			= Symbol::dosym;
		reserved["for"]			= Symbol::forsym;

		reserved["const"]		= Symbol::constsym;
		reserved["procedure"]	= Symbol::procsym;
		reserved["function"]	= Symbol::funcsym;
		reserved["var"]			= Symbol::varsym;

		reserved["downto"]		= Symbol::downtosym;
		reserved["to"]			= Symbol::tosym;
		reserved["read"]		= Symbol::readsym;
		reserved["write"]		= Symbol::writesym;
	}

	void getch() {
		if (cc == ll) {
			if (infile.eof())
			{
				cout << "The source code ended." << endl;
			}
			infile.getline(buf, 1024);
			ll = strlen(buf);
			cc = 0;
			linenum++;
			cout << endl;
			peek = ' ';
		}
		else {
			cc++;
			peek = buf[cc - 1];
		}
	}

	void getsym() {
		// skip space and '\t' '\n'
		while (peek == ' ' || peek == '\t')
			getch();
		// 'a' <= peek <= 'Z'
		if (isalpha(peek)) {
			// do a while for read more chars.
			do {
				token.push_back(peek);
				getch();
			} while (isalnum(peek));
			string str(token.begin(), token.end());
			map<string,Symbol>::iterator it = reserved.find(str);
			if (it != reserved.end())
			{
				sym = reserved[str];
			}
			else
			{
				sym = Symbol::ident;
				string name = str;
			}
			token.clear();
		}
		else if (isdigit(peek)) {
			do {
				token.push_back(peek);
				getch();
			} while (isdigit(peek));
			if (size(token) > NMAX)
			{
				Error::errorMessage(2, linenum);
			}
			else {
				int val = 0;
				//?¨®??????val
				for (vector<char>::iterator it = token.begin(); it != token.end(); it++)
					val = 10 * val + (*it - '0');
				string s(token.begin(), token.end());
			}
			token.clear();
		}
		else if (peek == ':') {
			getch();
			if (peek == '=') {
				sym = Symbol::becomes;
				getch();
			}
			else
				sym = Symbol::colon;
		}
		else if (peek == '<') {
			getch();
			if (peek == '=') {
				sym = Symbol::eql;
				getch();
			}
			else if (peek == '>') {
				sym = Symbol::neq;
				getch();
			}
			else
				sym = reserved["<"];
		}
		else if (peek == '>') {
			getch();
			if (peek == '=') {
				sym = Symbol::gtr;
				getch();
			}
			else
				sym = reserved[">"];
		}
		else if (peek == '"') {
			int tmplinenum = linenum;
			getch();
			while (peek != '"' && (tmplinenum == linenum)) {
				token.push_back(peek);
				getch();
			}
			if (tmplinenum == linenum) {
				string ident(token.begin(), token.end());
				sym = Symbol::constsym;
				getch();
			}
			else
				Error::errorMessage(3, linenum);
			token.clear();
		}
		else if (peek == '\'') {
			getch();
			char temp = peek;
			getch();
			if (peek == '\'')
			{
				sym = Symbol::constsym;
				string ident(1, temp);
				getch();
			}
			else
				Error::errorMessage(4, linenum);
			token.clear();
		}
		else
		{
			string s(1, peek);
			sym = reserved[s];
			getch();
		}
	}

};