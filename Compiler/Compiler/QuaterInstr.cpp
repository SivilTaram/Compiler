#include "quaterinstr.h"

void QuaterInstr::printQuater() {
	string output = printOpcode();
	string des_name = "";
	string src1_name = "";
	string src2_name = "";
	if (des != NULL)
		des_name = des->getName() + "\t,";
	else
		des_name = "-\t,";
	if (src1 != NULL)
		src1_name = src1->getName() + "\t,";
	else
		src1_name = "-\t,";
	if (src2 != NULL)
		src2_name = src2->getName() + "\t;";
	else
		src2_name = "-\t;";

}

string QuaterInstr::printOpcode() {
	switch (op)
	{
	case ADD:return "ADD\t";
	case SUB:return "SUB\t";
	case MUL:return "MUL\t";
	case DIV:return "DIV\t";
	case NEG:return "NEG\t";
	case BGR:return "BGR\t";
	case BGE:return "BGE\t";
	case BLS:return "BLS\t";
	case BLE:return "BLE\t";
	case BEQ:return "BEQ\t";
	case BNE:return "BNE\t";
	case ASS:return "ASS\t";
	case ASSADD:return "ASSADD\t";
	case JUMP:return "JUMP\t";
	case LABEL:return "LABEL\t";
	case READ:return "READ\t";
	case WRITE:return "WRITE\t";
	case BEGIN:return "BEGIN\t";
	case END:return "END\t";
	case PUSH:return "PUSH\t";
	case RETURN:return "RETURN\t";
	default:
		break;
	}
}

QuaterInstr::QuaterInstr(Opcode _op, SymbolItem* _des, SymbolItem* _src1, SymbolItem* _src2) {
	op = _op;
	des = _des;
	src1 = _src1;
	src2 = _src2;
}