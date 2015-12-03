
#include "rootsymbolset.h"
#include <sstream>
RootSymbolSet::RootSymbolSet() {
	SymbolItem* root_item = new SymbolItem("root",TokenKind::PROC,TokenType::voidtyp);
	//allocate a new table for root function.
	root_table = new SymbolSet(root_item, NULL, 0);
	current_table = root_table;
}
//look up to search the variable _name 
//util the root.
SymbolItem* RootSymbolSet::search(string _name) {
	SymbolSet* temp = current_table;
	SymbolItem* result = NULL;
	while (temp != NULL) {
		result = temp->getItem(_name);
		//if found,then exit;
		if (result != NULL)
			return result;
		//else look up and search.
		temp = temp->father_table;
	}
}

//Insert new Symbol to SymbolSet if no item with same name already exists.
SymbolItem* RootSymbolSet::insert(string _name, TokenKind _kind, TokenType _type, int _value) {
	SymbolItem* item = new SymbolItem(_name, _kind, _type, _value);
	string ident_name = item->getName();
	//if item already exist,return NULL;
	if (!(current_table->enterItem(item)))
		return NULL;
	if (_kind == TokenKind::FUNC
		|| _kind == TokenKind::PROC)
		current_table = current_table->enterProc(item);
	return item;
}

//go back to the father table.
// true means back success. false means back failure.
bool RootSymbolSet::goback() {
	if (current_table->father_table == NULL)
		return false;
	current_table = current_table->father_table;
	return true;
}

//generate temporatory variable to store the value of 
// middle for QuaterInstr.
SymbolItem* RootSymbolSet::genTemp(TokenKind _kind,TokenType _type) {
	SymbolItem *temp = NULL;
	//the temp's count for not repeating.
	static int temp_count = 1;
	//string name = "!t" + temp_count;
	stringstream name;
	name << "!TEMP" << temp_count;
	temp = new SymbolItem(name.str(), _kind, _type);
	current_table->enterItem(temp);
	temp_count++;
	return temp;
}

//look up to search table;
//[FIXME]
//IMPORTANT!!!
//the proc can call a proc as a result of what reason?!
//We can just conclude that the procedure can call the procedure
//which is the direct son of it and its brother proc,
//and its ancestors.
SymbolSet* RootSymbolSet::serachTable(string _name) {
	//look up from this table.
	SymbolSet *temp = current_table;
	SymbolSet *table = NULL;
	while (temp != NULL) {
		table = temp->getProcTable(_name);
		if (table == NULL)
			break;
		temp = temp->father_table;
	}
	//if table is null,there is no reference.
	return table;
}

//find the procedure or function.
bool RootSymbolSet::findProc(string _name) {
	SymbolSet *temp = current_table;
	while (temp != NULL) {
		if (temp->getProcName()==_name)
			return true;
		temp = temp->father_table;
	}
	//if table is null,there is no reference.
	return false;
}

//we should use the name to 
string RootSymbolSet::getCurrentName() {
	return current_table->getProcName();
}

//get the root of the all symbols tables;
SymbolSet* RootSymbolSet::getRootSet() {
	return root_table;
}

//get the current symbol table.
SymbolSet* RootSymbolSet::getCurrentSet() {
	return current_table;
}

//get a label for those for or if statement
SymbolItem* RootSymbolSet::genLabel() {
	static int count = 1;
	SymbolItem* label = NULL;
	stringstream name;
	name<< "-L-" << count;
	//string name = "_L" + count;
	label = new SymbolItem(name.str(), TokenKind::LABEL, TokenType::notyp);
	current_table->enterItem(label);
	//the size of label is zero.
	label->setSize(0);
	count++;
	return label;
}

vector<SymbolItem*> RootSymbolSet::getArgList(string _name)
{
	SymbolSet* table = serachTable(_name);
	return table->getArgList();
}
