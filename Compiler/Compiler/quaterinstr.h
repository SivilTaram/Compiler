#pragma once

#include "symbolset.h"
enum Opcode {
	ADD,
	SUB,
	MUL,
	DIV,
	NEG,
	//calculate instructions.
	//>
	BGR,
	//>=
	BGE,
	//<
	BLS,
	//<=
	BLE,
	//==
	BEQ,
	//<>
	BNE,
	//branch if meeting the conditions.

	// des = src
	ASS,
	// 
	ASSADD,

	JUMP,
	LABEL,
	READ,
	WRITE,
	BEGIN,
	END,
	PUSH,
	RETURN

};

//单条四元式类
class QuaterInstr {
	//op des src1 src2
public:
	QuaterInstr(Opcode _op,SymbolItem* _des=NULL,SymbolItem* _src1=NULL,SymbolItem* _src2=NULL);
	void printQuater();
	string printOpcode();
private:
	Opcode op;
	SymbolItem* des;
	SymbolItem* src1;
	SymbolItem* src2;
};