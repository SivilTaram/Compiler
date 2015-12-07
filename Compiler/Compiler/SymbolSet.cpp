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


void SymbolItem::printData() {
	cout << "Name:" << name << " | Kind:" << getKindName() << " | Type:" << getTypeName() << " | Value:" << value << " | size:" << size << " | level:"<< level << endl;
}

void SymbolSet::printData() {
	vector<SymbolItem*>::iterator local_iter = local_list.begin();
	while (local_iter != local_list.end()) {
		if ((*local_iter)->getKind() == TokenKind::FUNC || (*local_iter)->getKind() == TokenKind::PROC) {
			(*local_iter)->printData();
			string func_name = (*local_iter)->getName();
			SymbolSet* temp_table = table_map[func_name];
			temp_table->printData();
		}
		else {
			(*local_iter)->printData();
		}
		local_iter++;
	}
}


/* 
STACK STRUCTURE
----------------------------- $fp
arguments:
args_1
args_2
...
args_n
saved return addresss
saved old $fp
local variables and structures
saved Regs $s0-$s7
----------------------------- $sp

*/

void SymbolSet::calcOffset() {
	std::vector<SymbolItem*>::iterator iter = local_list.begin();
	int args_size = 0;
	//calculate the offset of the arguments.
	while (iter != local_list.end()) {
		if (((*iter)->getKind() == TokenKind::PARAVAR) || ((*iter)->getKind() == TokenKind::PARA)) {
			args_size += (*iter)->getSize();
		}
		else if ((*iter)->getKind() == TokenKind::FUNC) {
			(*iter)->setOffset(-(*iter)->getSize());
		}
	}

	int current_offset;
	int display_size = 0;
	
	//if the level = 1,then the SL should be like this:
	// abp(0)
	// abp(1)
	// ...
	// abp(n)
	display_size = level;





}