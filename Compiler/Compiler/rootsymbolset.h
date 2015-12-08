#pragma once

#include "symbolset.h"

class RootSymbolSet {
public:
	RootSymbolSet();

	SymbolItem* search(string _name); 
	SymbolItem* insert(string _name, TokenKind _kind, TokenType _type,int _value=0);
	SymbolItem* genTemp(TokenKind _kind,TokenType _type);
	SymbolItem* genLabel();
	vector<SymbolItem*> getArgList(string _name);
	void printSymbolSet();

	SymbolSet* getRootSet();
	SymbolSet* getCurrentSet();
	SymbolSet* serachTable(string _name);
	bool RootSymbolSet::goback();

	bool findProc(string _name);
	void calcOffset();
	string getCurrentName();
	

private:
	//the pointer to the current table;
	SymbolSet* current_table;
	//the pointer to the root table;
	SymbolSet* root_table;

};