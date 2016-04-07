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

string SymbolSet::getProcLabelName() {
	return proc->getString();
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
	cout << "Name:" << name << " | Kind:" << getKindName() << " | Type:" << getTypeName() << " | Value:" << value << " | size:" << size << " | level:"<< level << "| Offset:"<< offset << endl;
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
pre abp
abp(n)
abp(n-1)...
abp(0)
saved return addresss
local variables and structures
saved Regs $s0-$s7
----------------------------- $sp

*/

void SymbolSet::calcOffset() {
	//calculate the offset of the arguments.

	int current_offset = 0;

	current_offset += level * 4 + 8;

	//as a reserve order to set the offset.
	for (int i = local_list.size() - 1; i >= 0; i--) {
		SymbolItem* iter = local_list[i];
		if (iter->getKind() == TokenKind::PARAVAR || iter->getKind() == TokenKind::PARA)
		{
			args_size += iter->getSize();
			iter->setOffset(current_offset);
			current_offset += iter->getSize() * 4;
		}
	}

	//the return address and s0~s7
	stack_size += 9;

 	if (this->getProcItem()->getKind() == TokenKind::FUNC)
	{
		current_offset = -40;
		stack_size += 1;
	}
	else
		current_offset = -36;

	vector<SymbolItem*>::iterator iter = local_list.begin();

	while (iter != local_list.end()) {
		SymbolItem* item = *iter;
		if (item->getKind() == TokenKind::TEMP
			|| item->getKind() == TokenKind::VAR
			|| item->getKind() == TokenKind::TEMP_ADD
			)
		{
			stack_size += 1;
			item->setOffset(current_offset);
			current_offset -= 4;
		}
		else if (item->getKind() == TokenKind::ARRAY)
		{
			stack_size += item->getSize();
			item->setOffset(current_offset);
			current_offset -= item->getSize() * 4;
		}
		else if (item->getKind() == TokenKind::PROC
			|| item->getKind() == TokenKind::FUNC)
		{
			//Recursively;
			item->setOffset(0);
			SymbolSet* next_table = table_map[item->getName()];
			next_table->calcOffset();
		}
		iter++;
	}
	//if the level = 1,then the SL should be like this:
	// abp(0)
	// abp(1)
	// ...
	// abp(n)
	display_size = level;

}


int SymbolSet::getDisplaySize() {
	if (level >= 1)
		return getLevel() * 4;
	return 0;
}

int SymbolSet::getArgsSize() {
	return args_size * 4;
}

int SymbolSet::getStackSize() {
	return stack_size * 4;
}