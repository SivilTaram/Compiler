
#include"middlecode.h"

QuaterInstr* MiddleCode::gen(Opcode _op, SymbolItem* _des, SymbolItem* _src1, SymbolItem* _src2) {
	QuaterInstr* new_quater_instr = new QuaterInstr(_op, _des, _src1, _src2);
	//add the quater code to the middle code.
	middle_codes.push_back(new_quater_instr);
	return new_quater_instr;
}

//print all of the middle codes.
void MiddleCode::printMiddleCode() {
	vector<QuaterInstr*>::iterator it = middle_codes.begin();
	while (it != middle_codes.end()) {
		(*it)->printQuater();
		cout << endl;
	}
}