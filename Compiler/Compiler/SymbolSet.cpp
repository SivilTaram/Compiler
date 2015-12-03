#include "symbolset.h"
#include "error.h"
#include <vector>


SymbolSet::SymbolSet(SymbolItem* _proc, SymbolSet* _father, int _level) {
	proc = _proc;
	father_table = _father;
	level = _level;
}

//get the item from the symboltable using _name;
SymbolItem* SymbolSet::getItem(string _name) {
	map<string, SymbolItem*>::iterator symbol_it = symbol_map.find(_name);
	//if found,we can assert that the name existed.
	if (symbol_it != symbol_map.end())
		return symbol_map[_name];
	else
		//if return NULL,we can assert the symbolitem is null;
		return NULL;
}

bool SymbolSet::enterItem(SymbolItem * _item) {
	string item_name = _item->getName();
	//not found,can insert.
	//change the local_list and symbol_map.
	if (getItem(item_name) == NULL)
	{
		_item->setLevel(level);
		local_list.push_back(_item);
		symbol_map[item_name] = _item;
		return true;
	}
	else
		return false;
}

//get the proc's item.
SymbolItem* SymbolSet::getProcItem() {
	return proc;
}

//get the procedure or function's name mapping SymbolSet.
SymbolSet* SymbolSet::getProcTable(string _name) {
	map<string, SymbolSet*>::iterator it_table = table_map.find(_name);
	if (it_table != table_map.end())
		// the it_table's second of <string,SYmbolSet*> 
		return it_table->second;
	else
		return NULL;
}

string SymbolSet::getProcName() {
	return proc->getName();
}

//get arguments of the procedure or function.
//[FIXME]:IMPORTANT!!!
//The arglist should be in order !!!
vector<SymbolItem*> SymbolSet::getArgList() {
	vector<SymbolItem*> args;
	vector<SymbolItem*>::iterator it_args = local_list.begin();
	// This is a confused problem
	// when I wrote vector<SymbolItem*>
	// I can do it.
	// but when I replaced it with ARGLIST.It didn't work!
	// Why?????
	while (it_args != local_list.end()) {
		if ((*it_args)->getKind() == TokenKind::PARA
			|| (*it_args)->getKind() == TokenKind::PARAVAR)
			args.push_back(*it_args);
		it_args++;
	}
	return args;
}

int SymbolSet::getLevel() {
	return level;
}

//enter a Proc into the symbolset,and return a symbolset of the procedure.
SymbolSet* SymbolSet::enterProc(SymbolItem* _proc) {
	SymbolSet* son_proc = new SymbolSet(_proc, this, level + 1);
	//update the table_map of this table's sons.
	table_map[_proc->getName()] = son_proc;
	return son_proc;
}


