#pragma once
#include "quaterinstr.h"

class MiddleCode {
private:
	vector<QuaterInstr*> middle_codes;
	vector<string> data_const_string;
public:
	QuaterInstr* gen(Opcode _op, SymbolItem* _des  , SymbolItem* _src1  , SymbolItem* _src2 );
	void printMiddleCode();
	void addDataSeg(string _stringconst);
	vector<QuaterInstr*>* getMiddleCode();
};