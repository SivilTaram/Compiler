#include "mipsinstr.h"
#include "quaterinstr.h"
#include <sstream>

#define DEBUG
const string $sp = "$sp";
const string $fp = "$fp";
const string $s0 = "$s0";
const string $s1 = "$s1";
const string $s2 = "$s2";
const string $ra = "$ra";
const string $v0 = "$v0";
const string $t0 = "$t0";
const string $t1 = "$t1";
const string $t2 = "$t2";
const string $a0 = "$a0";
const string $0 = "$0";
const string $t7 = "$t7";

void MipsInstr::Handle(QuaterInstr* _middle) {
	Opcode choose = _middle->getOpType();
	switch (choose)
	{
	case ADD:
	case SUB:
	case MUL:
	case DIV:
		HandleCalc(choose, _middle->getDes(), _middle->getSrc1(), _middle->getSrc2()); break;
	case NEG:
		HandleNeg(_middle->getDes(), _middle->getSrc1()); break;
	case ASS:
		HandleAssign(_middle->getDes(), _middle->getSrc1()); break;
	case ASSADD:
		HandleAssignAddr(_middle->getDes(), _middle->getSrc1()); break;
	case ARRADD:
		HandleArrayAddr(_middle->getDes(), _middle->getSrc1(), _middle->getSrc2()); break;
	case ARRASS:
		HandleArrayAssign(_middle->getDes(), _middle->getSrc1()); break;
	case SETL:
		HandleSetLabel(_middle->getDes());
	case READ:break;
	case WRITE:
		HandleWrite(_middle->getDes()); break;
	case BGR:
	case BGE:
	case BLS:
	case BLE:
	case BEQ:
	case BNE:
		HandleBranch(choose,_middle->getDes(),_middle->getSrc1(),_middle->getSrc2()); break;
	case JUMP:
		HandleJump(_middle->getDes()); break;
	case BEGIN:
		HandleBegin(_middle->getDes());break;
	case END:
		HandleEnd(_middle->getDes()); break;
	case CALL:
		HandleCall(_middle->getDes(), _middle->getSrc1());break;
	case PUSH:
		HandlePush(_middle->getDes()); break;
	case PUSHVAR:break;
	case INC:
		HandleInc(_middle->getDes()); break;
	case DEC:
		HandleDec(_middle->getDes()); break;
	case RETURN:break;
	default:break;
	}
}


MipsInstr::MipsInstr(MiddleCode& _code, RootSymbolSet& _root):middle_code(_code), root_table(_root) {
	current_table = _root.getRootSet();
	add(MipsCode::j, "root_");
}

void MipsInstr::HandleJump(SymbolItem* des) {
	add(MipsCode::j, des->getName());
}

void MipsInstr::HandleBegin(SymbolItem* des) {
	SymbolSet* current_table = (SymbolSet*)des;
	/*
	if there is the begin of a function or procedure.
	we should do these things.
	save the pre abp.
	according the level's sub to save display.
	then move the $sp to the $fp.
	so the stack should be like this:
	args1
	args2
	...
	argsn
	pre $fp
	abp(n)
	abp(n-1)...
	abp(0)
	----------- $fp
	return address
	saved regs $s0-$s7:
	
	sw $s0,-8($fp)
	sw $s1,-12($fp)
	...

	(return value)
	local variables and structures
	----------- $sp
	*/
	// subu $sp,$sp,(display_size+pre_abp_size)
	// sw $fp,0($sp)
	// 
	add(MipsCode::label, current_table->getProcName() + ":");
	// move $fp,$sp to update the $fp.
	// $fp = $sp
	add(MipsCode::move, $fp, $sp);
	/*
	$ra --------- $fp
	...
	$s7
	local_1
	local_2
	local_3  ---- $sp
	*/
	add(MipsCode::sw, $ra, "0", $fp);
	for (int i = 0; i < 8; i++) {
		add(MipsCode::sw, "$s" + to_string(i), to_string(-4 - i * 4), $fp);
	}
	int size = current_table->getStackSize();
	add(MipsCode::subi, $sp, $sp, to_string(size));
}

void MipsInstr::HandleEnd(SymbolItem* des) {
	SymbolSet* current_table = (SymbolSet*)des;
	/*
	lw $ra,-4($fp)
	lw $s0,-8($fp)
	...
	lw $s7,-36($fp)
	lw $v0,-40($fp)
	lw $t0,displaysize($fp)
	add $t1,$fp,args_size+displaysize
	move $sp,$t1
	move $fp,$t0

	update the $fp and $sp
	jr $ra
	*/
	if (current_table->getLevel() != 0) {
		add(MipsCode::lw, $ra, "0", $fp);
		for (int i = 0; i < 8; i++) {
			add(MipsCode::lw, "$s" + to_string(i), to_string(-4 - i * 4), $fp);
		}
		if (current_table->getProcItem()->getKind() == TokenKind::FUNC)
			add(MipsCode::lw, $v0, "-36", $fp);
		// $ra ----- 0
		add(MipsCode::lw, $t0, to_string(current_table->getDisplaySize()+4), $fp);
		add(MipsCode::addi, $t1, $fp, to_string(current_table->getArgsSize() + current_table->getDisplaySize()+4));
		add(MipsCode::move, $sp, $t1);
		add(MipsCode::move, $fp, $t0);
		add(MipsCode::jr, $ra);
	}
}

void MipsInstr::HandleBranch(Opcode _branch,SymbolItem* des, SymbolItem* src1, SymbolItem* src2){
	loadReg(src1, $t0);
	loadReg(src2, $t1);
	if (_branch == Opcode::BEQ)
		add(MipsCode::beq, $t0, $t1, des->getName());
	else if (_branch == Opcode::BGE)
	{
		add(MipsCode::sub, $t0, $t0, $t1);
		add(MipsCode::bgez, $t0, des->getName());
	}
	else if (_branch == Opcode::BGR)
	{
		add(MipsCode::sub, $t0, $t0, $t1);
		add(MipsCode::bgtz, $t0, des->getName());
	}
	else if (_branch == Opcode::BLE)
	{
		add(MipsCode::sub, $t0, $t0, $t1);
		add(MipsCode::blez, $t0, des->getName());
	}
	else if (_branch == Opcode::BLS)
	{
		add(MipsCode::sub, $t0, $t0, $t1);
		add(MipsCode::bltz, $t0, des->getName());
	}
	else if (_branch == Opcode::BNE)
		add(MipsCode::bne, $t0, $t1, des->getName());
}

//write something
//char , int , and string
void MipsInstr::HandleWrite(SymbolItem* item) {
	if (item->getKind() == TokenKind::CONST || item->getKind() == TokenKind::TEMP_CON) {
		if (item->getType() == TokenType::chartyp) {
			add(MipsCode::addi, $a0, $0, to_string(item->getValue()));
			add(MipsCode::li, $v0, "11");
			add(MipsCode::syscall);
		}
		else if (item->getType() == TokenType::inttyp) {
			add(MipsCode::addi, $a0, $0, to_string(item->getValue()));
			add(MipsCode::li, $v0, "1");
			add(MipsCode::syscall);
		}
		else if (item->getType() == TokenType::stringtyp) {
			string label_string = genString();
			addData(label_string, item->getString());
			add(MipsCode::la, $a0,label_string);
			add(MipsCode::li, $v0, "4");
			add(MipsCode::syscall);
		}
	}
	else if (item->getKind() == TokenKind::VAR ||
		item->getKind() == TokenKind::PARA ||
		item->getKind() == TokenKind::TEMP ||
		item->getKind() == TokenKind::TEMP_ADD ||
		item->getKind() == TokenKind::ARRAY)
	{
		if (item->getType() == TokenType::chartyp) {
			loadReg(item, $t0);
			add(MipsCode::add, $a0, $0, $t0);
			add(MipsCode::li, $v0, "11");
			add(MipsCode::syscall);
		}
		else if (item->getType() == TokenType::inttyp) {
			loadReg(item, $t0);
			add(MipsCode::add, $a0, $0, $t0);
			add(MipsCode::li, $v0, "1");
			add(MipsCode::syscall);
		}
	}
}

void MipsInstr::HandleCall(SymbolItem* _caller,SymbolItem* _callee) {
	/*
	...
	abp(1)
	abp(0)	---------- $fp
	...
	pre $fp
	abp(n)
	abp(n-1)...
	abp(0)	----------- $sp
	*/
#ifdef DEBUG
	cout << "debug call" << endl;
#endif // DEBUG

	SymbolSet* caller_table = (SymbolSet*)_caller;
	SymbolSet* callee_table = (SymbolSet*)_callee;
	// subi $sp,$sp,4
	// sw $fp,0($sp)
	add(MipsCode::sw, $fp, "0", $sp);
	add(MipsCode::subi, $sp, $sp, "4");
	// subi $sp,$sp,display_size
	// if the level is less or equal,then drag calller's SL
	int size = callee_table->getDisplaySize();
	if (callee_table->getLevel() <= caller_table->getLevel())
	{
		for (int i = 0; i < size / 4; i++) {
			add(MipsCode::lw, $s0, to_string(i * 4), $fp);
			add(MipsCode::sw, $s0, to_string(i * 4), $sp);
		}
	}
	// if the level is more than caller.
	else {
		int  i = 0;
		if (caller_table->getDisplaySize() != 0)
		{
			for (i = 0; i < caller_table->getDisplaySize() / 4; i++) {
				add(MipsCode::lw, $s0, to_string(i * 4), $fp);
				add(MipsCode::sw, $s0, to_string(i * 4), $sp);
			}
		}
		add(MipsCode::sw, $fp, to_string(i * 4), $sp);
	}
	if (size != 0)
		add(MipsCode::subi, $sp, $sp, to_string(size));
	// jal callee_table;
	add(MipsCode::jal,callee_table->getProcName());
}

//load the item -> $i
void MipsInstr::loadReg(SymbolItem* item,const string _$i) {
	if (item->getKind() == TokenKind::TEMP_CON || item->getKind() == TokenKind::CONST)
	{
		add(MipsCode::li, _$i, to_string(item->getValue()));
		return;
	}
	else if (item->getKind() == TokenKind::FUNC) {
		add(MipsCode::addi, _$i, $v0, "0");
		return;
	}
	if (current_table->getItem(item->getName())!=NULL) {
		add(MipsCode::lw, _$i, to_string(item->getOffset()), $fp);
	}
	else {
		int display_base = item->getLevel() * 4 + 4;
		if (_$i == "$t0")
			cout << "this is a bug" << endl;
		add(MipsCode::lw, $t7, to_string(display_base), $fp);
		add(MipsCode::lw, _$i, to_string(item->getOffset()), $t7);
	}
}

//$i -> memory of item.
void MipsInstr::storeMemory(const string _$i, SymbolItem* item) {
	if (item->getKind() == FUNC) {
		// -36 is the return value
		add(MipsCode::sw, _$i, "-36", $fp);
		return;
	}
	if (current_table->getItem(item->getName()) != NULL) {
		add(MipsCode::sw, _$i, to_string(item->getOffset()), $fp);
	}
	else {
		int display_base = item->getLevel()* 4 + 4;
		add(MipsCode::lw, $t7, to_string(display_base), $fp);
		add(MipsCode::sw, _$i, to_string(item->getOffset()), $t7);
	}
}

//[des] = src
void MipsInstr::HandleAssignAddr(SymbolItem* addr,SymbolItem* value) {
		loadReg(value, $t0);
		loadReg(addr, $t1);
		add(MipsCode::sw, $t0,"0", $t1);
}

//temp = abstract add or array + offset.
void MipsInstr::HandleArrayAddr(SymbolItem* addr, SymbolItem* base, SymbolItem* offset) {
	getRef(base);
	loadReg(offset, $t1);
	add(MipsCode::li, $t2, "4");
	add(MipsCode::mult, $t1, $t1, $t2);
	add(MipsCode::sub, $t0, $t0, $t1);
	storeMemory($t0, addr);
}

// value = [addr];
void MipsInstr::HandleArrayAssign(SymbolItem* value , SymbolItem* addr) {
	//addr getValue;
	// $t0 = address of addr
	getRef(addr);
	// $t2 = lw 0($t0) = addr;
	add(MipsCode::lw, $t2, "0", $t0);
	// $t2 = lw 0($t2) = value;
	add(MipsCode::lw, $t2, "0", $t2);
	storeMemory($t2, value);
}

// des = src;
void MipsInstr::HandleAssign(SymbolItem* des, SymbolItem* src) {
	loadReg(src,$t0);
	storeMemory($t0, des);
}

//Handle the calc instruction
void MipsInstr::HandleCalc(Opcode choose,SymbolItem* des,SymbolItem* src1,SymbolItem* src2){
	loadReg(src1, $t0);
	loadReg(src2, $t1);
	if (choose == Opcode::MUL) {
		add(MipsCode::mult, $s0, $t0, $t1);
	}
	else if (choose == Opcode::ADD) {
		add(MipsCode::add, $s0, $t0, $t1);
	}
	else if (choose == Opcode::SUB) {
		add(MipsCode::sub, $s0, $t0, $t1);
	}
	else if (choose == Opcode::DIV) {
		add(MipsCode::ddiv,$t0, $t1);
		add(MipsCode::mflo, $s0);
	}
	storeMemory($s0, des);
}

// src = src + 1
void MipsInstr::HandleInc(SymbolItem* src) {
	loadReg(src, $t0);
	add(MipsCode::addi, $t0, $t0, "1");
	storeMemory($t0, src);
}

// src = src - 1
void MipsInstr::HandleDec(SymbolItem* src) {
	loadReg(src, $t0);
	add(MipsCode::subi, $t0, $t0, "1");
	storeMemory($t0, src);
}

void MipsInstr::HandleSetLabel(SymbolItem* des) {
	add(MipsCode::label,des->getName()+":");
}

// $s0 = - $t0;
void MipsInstr::HandleNeg(SymbolItem* des, SymbolItem* src) {
	loadReg(src, $t0);
	add(MipsCode::sub, $s0, $0, $t0);
	storeMemory($s0, des);
}

void MipsInstr::HandlePush(SymbolItem* des) {
	loadReg(des, $t0);
	add(MipsCode::sw, $t0, "0", $sp);
	add(MipsCode::subi, $sp, $sp, "4");
}

string MipsInstr::genString() {
	static int count = 1;
	stringstream s;
	s << "_string" << count ;
	count++;
	return s.str();
}
//void MipsInstr::HandleCalc(Opcode choose,)
//if the item is a call func or call procedure.
//then we should point the current table to the procedure or the func's table.
void MipsInstr::translate() {
	vector<QuaterInstr*>::iterator iter = middle_code.getMiddleCode()->begin();
	while (iter != middle_code.getMiddleCode()->end()) {
		QuaterInstr* single_middle = (*iter);
		if (single_middle->getOpType() == Opcode::BEGIN)
			current_table = (SymbolSet*)single_middle->getDes();
		Handle(single_middle);
		iter++;
	}
	ofstream fout("F://Compiler//output.txt");
	fout << ".data" << endl;
	for (int i = 0; i < object_datas.size(); i++) {
		fout << object_datas[i].setData() << endl;
	}
	fout << ".text" << endl;
	for (int i = 0; i < object_insrtuctions.size(); i++) {
		fout << object_insrtuctions[i].printInstr() << endl;
	}
	fout.close();
}

//get the address of the symbol item.
//the address of the symbol item strore in the $t0
void MipsInstr::getRef(SymbolItem* item) {
	if (current_table->getItem(item->getName()) != NULL) {
		add(MipsCode::addi, $t0, $fp, to_string(item->getOffset()));
	}
	else {
		int display_base = item->getLevel() * 4;
		add(MipsCode::lw, $t0, to_string(display_base), $fp);
		add(MipsCode::addi, $t0, $t0, to_string(item->getOffset()));
	}
}
