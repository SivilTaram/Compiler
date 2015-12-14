#include <iostream>
#include <map>
#include <string>
#include <ctype.h>
#include <vector>
#include <fstream>
#include <iomanip>
#include "token.h"
#include "error.h"
#include "lexer.h"

using namespace std;

Lexer::Lexer(string filename,Error& _error_handle) :error_handle(_error_handle) {
	//Read from source code.
	infile.open(filename);
	initialReserved();
	infile.getline(buf, 1024);
	ll = strlen(buf);
	cc = 0;
	getch();
};
//Initial reserved_map;
void Lexer::initialReserved() {
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

	reserved["integer"] = Symbol::integersym;
	reserved["char"] = Symbol::charsym;
	reserved["array"] = Symbol::arraysym;
	reserved["of"] = Symbol::ofsym;

	reserved["begin"] = Symbol::beginsym;
	reserved["end"] = Symbol::endsym;
	reserved["if"] = Symbol::ifsym;
	reserved["then"] = Symbol::thensym;
	reserved["else"] = Symbol::elsesym;
	reserved["while"] = Symbol::whilesym;
	reserved["do"] = Symbol::dosym;
	reserved["for"] = Symbol::forsym;

	reserved["const"] = Symbol::constsym;
	reserved["procedure"] = Symbol::procsym;
	reserved["function"] = Symbol::funcsym;
	reserved["var"] = Symbol::varsym;

	reserved["downto"] = Symbol::downtosym;
	reserved["to"] = Symbol::tosym;
	reserved["read"] = Symbol::readsym;
	reserved["write"] = Symbol::writesym;
}

void Lexer::getch() {
	if (cc == ll) {
		if (infile.eof())
		{
			//cout << "The source code ended." << endl;
			peek = EOF;
			return;
		}
		infile.getline(buf, 1024);
		ll = strlen(buf);
		cc = 0;
		linenum++;
		peek = ' ';
	}
	else {
		cc++;
		peek = buf[cc - 1];
	}
}

Token Lexer::getsym() {
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
		map<string, Symbol>::iterator it = reserved.find(str);
		if (it != reserved.end())
		{
			sym = reserved[str];
			token.clear();
			return Token(sym,linenum);
		}
		else
		{
			sym = Symbol::ident;
			string ident_name = str;
			token.clear();
			return Token(sym,linenum,ident_name);
		}
	}
	//if 0 <= peek <= 9
	else if (isdigit(peek)) {
		do {
			token.push_back(peek);
			getch();
		} while (isdigit(peek));
		if (size(token) > NMAX)
		{
			error_handle.errorMessage(2, linenum);
			token.clear();
		}
		else {
			int val = 0;
			//calculate the value of number
			for (vector<char>::iterator it = token.begin(); it != token.end(); it++)
				val = 10 * val + (*it - '0');
			token.clear();
			return Token(Symbol::number,linenum,"", val);
		}
	}
	//colon
	else if (peek == ':') {
		getch();
		if (peek == '=') {
			sym = Symbol::becomes;
			getch();
			return Token(sym,linenum);
		}
		else
		{
			sym = Symbol::colon;
			return Token(sym,linenum);
		}
	}
	// < 
	else if (peek == '<') {
		getch();
		if (peek == '=') {
			//<=
			sym = Symbol::leq;
			getch();
		}
		else if (peek == '>') {
			//<>
			sym = Symbol::neq;
			getch();
		}
		else
			sym = reserved["<"];
		return Token(sym,linenum);
	}
	else if (peek == '>') {
		getch();
		if (peek == '=') {
			// >=
			sym = Symbol::geq;
			getch();
		}
		else
			sym = reserved[">"];
		return Token(sym,linenum);
	}
	//const string sym.
	else if (peek == '"') {
		int tmplinenum = linenum;
		getch();
		while (peek != '"' && (tmplinenum == linenum)) {
			token.push_back(peek);
			getch();
		}
		if (tmplinenum == linenum) {
			string _ident_name(token.begin(), token.end());
			token.clear();
			sym = Symbol::strconst;
			getch();
			return Token(sym,linenum,"\""+_ident_name+"\"");
		}
		else
		{
			error_handle.errorMessage(3, linenum);
		}
	}
	// ' ' const char.
	else if (peek == '\'') {
		getch();
		char value = peek;
		string temp ="\'"+peek;
		getch();
		if (peek == '\'')
		{
			sym = Symbol::charconst;
			getch();
			temp += "\'";
			token.clear();
			return Token(sym,linenum,"",value);
		}
		else
		{
			token.clear();
			error_handle.errorMessage(4, linenum);
			return Token(Symbol::nullsym, linenum);
		}
	}
	else if (peek == EOF)
	{
		return Token(Symbol::eofsym,linenum);
	}
	else
	{
		string str(1, peek);
		map<string, Symbol>::iterator it = reserved.find(str);
		if (it == reserved.end())
		{
			error_handle.errorMessage(6, linenum);
			sym = Symbol::nullsym;
			getch();
			return Token(sym,linenum);
		}
		else
		{
			sym = reserved[str];
			getch();
			return Token(sym,linenum);
		}
	}
}
