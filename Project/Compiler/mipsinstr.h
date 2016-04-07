#pragma once
#include <string>
#include "symbolset.h"
#include "middlecode.h"
#include "quaterinstr.h"
#include "rootsymbolset.h"

using namespace std;


enum MipsCode {
	//load to the register
	lw,
	//store to the memory
	sw,
	//load the immdiate number.
	li,
	//load address
	la,
	//jump and link
	jal,
	//
	jr,
	//jumpd
	j,
	//add the ident
	add,
	addi,
	//sub
	sub,
	subi,
	//mult
	mult,
	//dive
	ddiv,
	//assign
	move,
	//branch
	bgez,
	blez,
	bgtz,
	bltz,
	beq,
	bne,
	syscall,
	label,
	mflo,
	note
};


class Instruction {
private:
	string op = "";
	string des = "";
	string src1 = "";
	string src2 = "";
public:
	Instruction(MipsCode _op) {
		op = getOpName(_op);
	}
	Instruction(MipsCode _op, string _des) {
		op = getOpName(_op);
		des = _des;
	}
	Instruction(MipsCode _op, string _des, string _src1) {
		op = getOpName(_op);
		des = _des;
		src1 = _src1;
	}
	Instruction(MipsCode _op, string _des, string _src1, string _src2) {
		op = getOpName(_op);
		des = _des;
		src1 = _src1;
		src2 = _src2;
	}

	string getOpName(MipsCode _opcode) {
		switch (_opcode)
		{
		case lw:
			return "lw"; break;
		case sw:
			return "sw"; break;
		case li:
			return "li"; break;
		case la:
			return "la"; break;
		case jal:
			return "jal"; break;
		case jr:
			return "jr"; break;
		case add:
			return "add"; break;
		case addi:
			return "addi"; break;
		case sub:
			return "sub"; break;
		case subi:
			return "subi"; break;
		case mult:
			return "mul"; break;
		case MipsCode::ddiv:
			return "div"; break;
		case MipsCode::move:
			return "move"; break;
		case mflo:
			return "mflo"; break;
		case bgez:
			return "bgez"; break;
		case blez:
			return "blez"; break;
		case bgtz:
			return "bgtz"; break;
		case bltz:
			return "bltz"; break;
		case beq:
			return "beq"; break;
		case bne:
			return "bne"; break;
		case syscall:
			return "syscall"; break;
		case label:
			return ""; break;
		case j:
			return "j"; break;
		case note:
			return "#"; break;
		default:
			break;
		}
	}
	string printInstr() {
		return op + des + src1 + src2;
	}
};

class Dataseg {
private:
	string label;
	string conststring;
public:
	string setData() {
		return label + ":" + ".asciiz " + conststring;
	}
	Dataseg(string _label, string _conststring) {
		label = _label;
		conststring = _conststring;
	}
};

class MipsInstr {
public:
	//add the instruction to the vector.
	MiddleCode middle_code;
	RootSymbolSet root_table;
	SymbolSet*  current_table = NULL;
	MipsInstr(MiddleCode& _code, RootSymbolSet& _root);
	void add(MipsCode _op) {
		object_insrtuctions.push_back(Instruction(_op));
	}
	void add(MipsCode _op, string _des) {
		object_insrtuctions.push_back(Instruction(_op," "+_des));
	}
	void add(MipsCode _op, string _des, string _src1) {
		object_insrtuctions.push_back(Instruction(_op, " "+_des, ","+_src1));
	}
	void add(MipsCode _op, string _des, string _src1, string _src2) {
		if (_op == MipsCode::sw || _op == MipsCode::lw)
			object_insrtuctions.push_back(Instruction(_op, " " + _des, "," + _src1, "(" + _src2 + ")"));
		else
			object_insrtuctions.push_back(Instruction(_op, " "+_des, ","+_src1, ","+_src2));
	}
	void addData(string _name, string _conststring) {
		object_datas.push_back(Dataseg(_name, _conststring));
	}
	void Handle(QuaterInstr* middle);
	void translate();
private:
	vector<Instruction> object_insrtuctions;
	vector<Dataseg> object_datas;
	void HandleWrite(SymbolItem* des);
	void HandleBegin(SymbolItem* des);
	void HandleEnd(SymbolItem* des);
	void HandleCalc(Opcode choose, SymbolItem* des, SymbolItem* src1, SymbolItem* src2);
	void HandleNeg(SymbolItem* des,SymbolItem* src);
	void HandleAssignAddr(SymbolItem* addr, SymbolItem* value);
	void HandleArrayAddr(SymbolItem* addr, SymbolItem* base, SymbolItem* offset);
	void HandleArrayAssign(SymbolItem* value,SymbolItem* addr);
	void HandleAssign(SymbolItem* des,SymbolItem* src);
	void HandleBranch(Opcode branch,SymbolItem* des,SymbolItem* src1,SymbolItem* src2);
	void HandleCall(SymbolItem* caller,SymbolItem* callee);
	void HandleInc(SymbolItem* src);
	void HandleDec(SymbolItem* src);
	void HandleSetLabel(SymbolItem* des);
	void HandleJump(SymbolItem* des);
	void HandlePush(SymbolItem* des);
	void HandlePushVar(SymbolItem* des);
	void HandleRead(SymbolItem* des);
	void loadReg(SymbolItem* item,const string _$i,bool _loadaddrvalue=false);
	void storeMemory(const string _$i, SymbolItem* item);
	string genString();
	void getRef(SymbolItem* item);
};

