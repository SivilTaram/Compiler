#include "quaterinstr.h"

void QuaterInstr::printQuater() {
	string output = printOpcode();
	string des_name = "";
	string src1_name = "";
	string src2_name = "";
	if (op != Opcode::BEGIN && op != Opcode::END)
	{
		if (des != NULL)
			des_name = des->getKind() + ",";
		else
			des_name = "";
		if (src1 != NULL)
			src1_name = src1->getKind() + ",";
		else
			src1_name = "";
		if (src2 != NULL)
			src2_name = src2->getKind() + ";";
		else
			src2_name = "";
	}
	cout << output << des_name << src1_name << src2_name;
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
	case SETLABEL:return "SETLABEL\t";
	case READ:return "READ\t";
	case WRITE:return "WRITE\t";
	case BEGIN:return "BEGIN\t";
	case END:return "END\t";
	case PUSH:return "PUSH\t";
	case RETURN:return "RETURN\t";
	case INC:return "INC\t";
	case DEC:return "DEC\t";
	default:
		break;
	}
	return "NULL\t";
}

QuaterInstr::QuaterInstr(Opcode _op, SymbolItem* _des, SymbolItem* _src1, SymbolItem* _src2) {
	op = _op;
	des = _des;
	src1 = _src1;
	src2 = _src2;
}