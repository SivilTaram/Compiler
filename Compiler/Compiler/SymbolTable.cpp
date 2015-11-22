#include "symbolset.h"
#include <vector>

SymbolSet::SymbolSet() {
	level = 0;
}

int SymbolSet::enter(SymbolItem item) {
	table.push_back(item);
	return (table.size()-1);
}

int SymbolSet::locIdent(string name) {
	int i = level;
	int j = 0;
	do {
		j = display[i]->getLast();
		while (table[j].getName() != name)
			j = table[j].getLink();
		i--;
	} while (i >= 0 && j == 0);
	return j;
}

void SymbolSet::enterproc(SymbolItem& _item, SymbolPro& _pro) {
	level++;
	table.push_back(_item);
	protable.push_back(_pro);
	display[level] = &_pro;
}

void SymbolSet::exitproc() {
	level--;

}