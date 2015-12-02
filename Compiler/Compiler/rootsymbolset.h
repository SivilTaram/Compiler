#pragma once

#include "symbolset.h"

class RootSymbolSet {
public:
	RootSymbolSet();

	SymbolItem* search(string _name); 
	SymbolItem* insert(string _name, TokenKind _kind, TokenType _type,int _value=0);
	SymbolItem* genTemp(TokenKind _kind,TokenType _type);

	SymbolSet* getRootSet();
	SymbolSet* getCurrentSet();
	SymbolSet* serachTable(string _name);
	bool RootSymbolSet::goback();

	bool findProc(string _name);
	vector<SymbolItem*> getArgList(string _name);
	string getCurrentName();
	

private:
	//the pointer to the current table;
	SymbolSet* current_table;
	//the pointer to the root table;
	SymbolSet* root_table;

};