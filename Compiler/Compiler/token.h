#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <map>
#include <string>
#include <ctype.h>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

#define reserved_map map<string,Symbol>

enum Symbol {
	nullsym,/* not any one */

	ident, /* ab */
	number,/* 123 */
	charconst/* 'a' */,
	strconst/* "absdas" */,

	/* operator */
	minus,/* '-' */
	times,/* '*' */
	plus, /* '+' */
	slash,/* '/' */
	eql/* = */, 
	neq/*'<>'*/, 
	lss/*'<'*/, 
	leq/*'<='*/, 
	gtr/*'>'*/, 
	geq/*'>='*/,
	lparen/*'('*/, 
	rparen/*')'*/,
	lsquare/*'['*/,
	rsquare/*']'*/,
	comma/*','*/, 
	semicolon/*';'*/, 
	period/*'.'*/, 
	becomes/*':='*/, 
	colon,//:
	quote,/* ' */

	/* statment */
	beginsym, 
	endsym, 
	ifsym, 
	thensym, 
	elsesym, 
	dosym, 
	whilesym, 
	forsym, 
	downtosym, 
	tosym,

	// 'a'
	constsym,
	// var
	varsym, 
	procsym, 
	funcsym,
	charsym, 
	integersym, 
	arraysym, 
	ofsym,

	/* read & write syscall */
	readsym, 
	writesym,

	/* The end of the file */
	eofsym
};

class Token {
private:
	Symbol type;
	string ident_name;
	int num_value;
	//line_num for error message
	int line_num;
public:
	Token(Symbol _type,int _line_num,string _ident_name = "", int _num_value = 0);
	Token() {}
	Symbol getType();
	int getLineNo();
	string getName();
	int getValue();
};


#endif