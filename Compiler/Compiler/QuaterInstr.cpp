#include "quaterinstr.h"
#include "sstream"

void QuaterInstr::printQuater() {
	string opcode_name = printOpcode();
	stringstream output;

	if(op!=Opcode::SETL)
		output << "\t" << opcode_name << "\t";	

	if (op == Opcode::BEGIN || op == Opcode::END) {
		output << ((SymbolSet *)des)->getProcName() << "\t";
	}
	else if (des == NULL && op != Opcode::SETL)
		output << "-\t";
	else if (des->getKind() == TokenKind::TEMP_CON && des->getType() == TokenType::inttyp)
		output << des->getValue() << "\t";
	else if (des->getKind() == TokenKind::TEMP_CON && des->getType() == TokenType::stringtyp)
		output << des->getString() << "\t";
	else if (des->getKind() == TokenKind::TEMP_CON && des->getType() == TokenType::chartyp)
		output << (char)des->getValue() << "\t";
	else if (des->getKind() == TokenKind::TEMP_ADD)
		output << "[" << des->getValue() << "]\t";
	else if (des->getKind() == TokenKind::VAR)
		output << des->getName() << "\t";
	else if (des->getKind() == TokenKind::TEMP)
		output << des->getName() << "\t";
	else if (des->getKind() == TokenKind::LABEL)
		output << des->getName() << "\t";
	else if (des->getKind() == TokenKind::FUNC)
		output << des->getName() << "\t";
	else if (des->getKind() == TokenKind::PROC)
		output << des->getName() << "\t";

	if (op == Opcode::SETL)
		output << "";
	else if (src1 == NULL)
		output << "-\t";
	else if (src1->getKind() == TokenKind::TEMP_CON && src1->getType() == TokenType::inttyp)
		output << src1->getValue() << "\t";
	else if (src1->getKind() == TokenKind::TEMP_CON && src1->getType() == TokenType::stringtyp)
		output << src1->getString() << "\t";
	else if (src1->getKind() == TokenKind::TEMP_CON && src1->getType() == TokenType::chartyp)
		output << (char)src1->getValue() << "\t";
	else if (src1->getKind() == TokenKind::TEMP_ADD)
		output << "[" << src1->getValue() << "]\t";
	else if (src1->getKind() == TokenKind::TEMP)
		output << src1->getName() << "\t";
	else if (src1->getKind() == TokenKind::VAR)
		output << src1->getName() << "\t";
	else if (src1->getKind() == TokenKind::FUNC)
		output << src1->getName() << "\t";
	else if (src1->getKind() == TokenKind::PROC)
		output << src1->getName() << "\t";

	if (op == Opcode::SETL)
		output << "";
	else if (src2 == NULL)
		output << "-\t";
	else if (src2->getKind() == TokenKind::TEMP_CON && src2->getType() == TokenType::inttyp)
		output << src2->getValue() << "\t";
	else if (src2->getKind() == TokenKind::TEMP_CON && src2->getType() == TokenType::stringtyp)
		output << src2->getString() << "\t";
	else if (src2->getKind() == TokenKind::TEMP_CON && src2->getType() == TokenType::chartyp)
		output << (char)src2->getValue() << "\t";
	else if (src2->getKind() == TokenKind::TEMP_ADD)
		output << "[" << src2->getValue() << "]\t";
	else if (src2->getKind() == TokenKind::VAR)
		output << src2->getName() << "\t";
	else if (src2->getKind() == TokenKind::TEMP)
		output << src2->getName() << "\t";
	else if (src2->getKind() == TokenKind::LABEL)
		output << src2->getName() << "\t"; 
	else if (src2->getKind() == TokenKind::FUNC)
		output << src2->getName() << "\t";
	else if (src2->getKind() == TokenKind::PROC)
		output << src2->getName() << "\t";


	cout << output.str();
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
	case ARRADD:return "ARRADD\t";
	case ASSADD:return "ASSDD\t";
	case JUMP:return "JUMP\t";
	case SETL:return "";
	case READ:return "READ\t";
	case WRITE:return "WRITE\t";
	case BEGIN:return "BEGIN\t";
	case END:return "END\t";
	case PUSH:return "PUSH\t";
	case RETURN:return "RET\t";
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