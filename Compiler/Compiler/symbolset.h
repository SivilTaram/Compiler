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

class SymbolSet {
public:
	vector<SymbolItem> table;
};
#endif