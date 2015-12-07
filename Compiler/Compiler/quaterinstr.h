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
	// [des] = src
	ASSADD,
	// temp = add of array + offset
	ARRADD,
	// des = [src]
	ARRASS,


	JUMP,
	SETL,
	READ,
	WRITE,
	BEGIN,
	END,

	CALL,
	/*
	as a result of not knowing offset in generating 
	quaters.So we can't get the address of the vairable
	or the parameters.So we must generate a push var
	to do this.

	*/
	PUSH,
	PUSHVAR,

	//increment +1
	INC,
	//decrement -1
	DEC,

	RETURN

};

//单条四元式类
class QuaterInstr {
	//op des src1 src2
public:
	QuaterInstr(Opcode _op,SymbolItem* _des ,SymbolItem* _src1 ,SymbolItem* _src2);
	void printQuater();
	string printOpcode();
private:
	Opcode op;
	SymbolItem* des;
	SymbolItem* src1;
	SymbolItem* src2;
};