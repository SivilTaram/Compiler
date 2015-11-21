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
	ident, /* ab */
	number,/* '123' */

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
	colon,/*':'*/
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

	/* declaretion */
	constsym, 
	varsym, 
	procsym, 
	funcsym,
	charsym, 
	integersym, 
	arraysym, 
	ofsym,

	/* read & write syscall */
	readsym, 
	writesym
};

enum TokenObj {
	constobj,
	varobj,
	procobj,
	funcobj
};

enum TokenType {
	inttyp,
	chartyp,
	arraytyp,
	notyp
};

class Token {
public:
	Symbol type;

};


#endif