#ifndef SYMBOLSET_H
#define SYMBOLSET_H

#include<string>
#include<ctype.h>
#include "token.h"

using namespace std;

class SymbolItem {
public:
	string name;
	TokenObj object;
	TokenType type;
	int adr;
	int ref;
	int lev;

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

class SymbolSet {
public:
	vector<SymbolItem> table;
};
#endif