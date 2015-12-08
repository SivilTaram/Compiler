#include "mipsinstr.h"
#include "quaterinstr.h"
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
const string $a0 = "$a0";
const string $0 = "$0";

void MipsInstr::Handle(QuaterInstr* _middle) {
	Opcode choose = _middle->getOpType();
	switch (choose)
	{
	case ADD:
		break;
	case SUB:
		break;
	case MUL:
		break;
	case DIV:
		break;
	case NEG:
		break;
	case ASS:break;
	case ASSADD:break;
	case ARRADD:break;
	case ARRASS:break;
	case SETL:break;
	case READ:break;
	case WRITE:break;
	case BGR:
	case BGE:
	case BLS:
	case BLE:
	case BEQ:
	case BNE:HandleBranch(choose,_middle->getDes(),_middle->getSrc1(),_middle->getSrc2()); break;
	case JUMP:break;
	case BEGIN:HandleBegin(_middle->getDes());break;
	case END:HandleEnd(_middle->getDes()); break;
	case CALL:HandleCall(_middle->getDes(), _middle->getSrc1());break;
	case PUSH:break;
	case PUSHVAR:break;
	case INC:break;
	case DEC:break;
	case RETURN:break;
	default:break;
	}
}

MipsInstr::MipsInstr(MiddleCode& _code, RootSymbolSet& _root):middle_code(_code), root_table(_root) {
	current_table = _root.getRootSet();
}

void MipsInstr::HandleAssign() {

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
	add(MipsCode::move, $fp, $sp);
	add(MipsCode::sw, $ra, "-4", $fp);
	for (int i = 0; i < 8; i++) {
		add(MipsCode::sw, "$s" + to_string(i), to_string(-4 - (i+1) * 4), $fp);
	}

	int current_stack_size = current_table->getStackSize();
	add(MipsCode::subi, $sp, $sp, to_string(current_stack_size));
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
	add(MipsCode::lw, $ra, "-4", $fp);
	for (int i = 0; i < 8; i++) {
		add(MipsCode::lw, "$s" + to_string(i), to_string(-4 - (i + 1) * 4), $fp);
	}
	if(current_table->getProcItem()->getKind()==TokenKind::FUNC)
		add(MipsCode::lw, $v0, "-40", $fp);
	add(MipsCode::lw, $t0, to_string(current_table->getDisplaySize()), $fp);
	add(MipsCode::addi, $t1, $fp, to_string(current_table->getArgsSize() + current_table->getDisplaySize()));
	add(MipsCode::move, $sp, $t1);
	add(MipsCode::move, $fp, $t0);
	add(MipsCode::jr, $ra);
}

void MipsInstr::HandleBranch(Opcode _branch,SymbolItem* des, SymbolItem* src1, SymbolItem* src2){

}

void MipsInstr::HandleWrite(SymbolItem* item) {
	if (item->getType()==TokenType::chartyp){
		add(MipsCode::addi, $a0, $0, to_string(item->getValue()));
		add(MipsCode::li, $v0, "11");
		add(MipsCode::syscall);
	}
}

void MipsInstr::HandleCall(SymbolItem* _caller,SymbolItem* _callee) {
	/*
	...
	abp(1)
	abp(0)
	---------- $fp
	...
	pre $fp
	abp(n)
	abp(n-1)...
	abp(0)
	----------- $sp
	*/
#ifdef DEBUG
	cout << "debug call" << endl;
#endif // DEBUG

	SymbolSet* caller_table = (SymbolSet*)_caller;
	SymbolSet* callee_table = (SymbolSet*)_callee;
	// subi $sp,$sp,4
	// sw $fp,0($sp)
	add(MipsCode::subi, $sp, $sp, "4");
	add(MipsCode::sw, $fp, "0", $sp);
	// subi $sp,$sp,display_size
	int size = caller_table->getDisplaySize();
	add(MipsCode::subi, $sp, $sp, to_string(size));
	// if the level is less or equal,then drag calller's SL
	if (callee_table->getLevel() <= caller_table->getLevel())
	{
		for (int i = 0; i < size / 4; i++) {
			add(MipsCode::lw, $s0, to_string(i * 4), $fp);
			add(MipsCode::sw, $s0, to_string(i * 4), $sp);
		}
	}
	// if the level is more than caller.
	else {
		int  i;
		for (i = 0; i < caller_table->getDisplaySize() / 4; i++) {
			add(MipsCode::lw, $s0, to_string(i * 4), $fp);
			add(MipsCode::sw, $s0, to_string(i * 4), $sp);
		}
		add(MipsCode::sw, $fp, to_string(i * 4), $sp);
	}
	// jal callee_table;
	add(MipsCode::jal,callee_table->getProcName());
}

void MipsInstr::loadReg(SymbolItem* item,const string _$i) {
	if (current_table->getItem(item->getName())!=NULL) {
		add(MipsCode::lw, _$i, to_string(item->getOffset()), $fp);
	}
	else {
		int display_base = (item->getLevel() - 1) * 4;
		add(MipsCode::lw, $t0, to_string(display_base), $fp);
		add(MipsCode::lw, _$i, to_string(item->getOffset()), $t0);
	}
}

//if the item is a call func or call procedure.
//then we should point the current table to the procedure or the func's table.
void MipsInstr::translate() {
	vector<QuaterInstr*>::iterator iter = middle_code.getMiddleCode()->begin();
	while (iter != middle_code.getMiddleCode()->end()) {
		QuaterInstr* single_middle = (*iter);
		if (single_middle->getOpType() == Opcode::BEGIN)
			current_table = (SymbolSet*)single_middle->getSrc1();
		Handle(single_middle);
		iter++;
	}
	for (int i = 0; i < object_insrtuctions.size(); i++) {
		object_insrtuctions[i].printInstr();
	}
}