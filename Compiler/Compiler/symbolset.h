#pragma once

#include <string>
#include <ctype.h>
#include "token.h"
#include <assert.h>
#include <array>     

#define MAX_LEVEL (3+1)

using namespace std;

enum TokenKind {
	CONST,// .data
	VAR,
	PARA, // we should put it on the stack.
	PARAVAR, // stack.
	ARRAY, // array must be integer or char.
	PROC,
	FUNC,
	LABEL,// we should use it in three address code.
	TEMP, // Temporary varibale.
	TEMP_ADD,// Temporary variable address(in array).
	TEMP_CON// immediate number. Temporary constant.
};

enum TokenType {
	voidtyp,
	inttyp,
	chartyp,
	notyp,
	stringtyp
};

class SymbolItem {
private:
	string name;
	TokenKind kind;
	TokenType type;
	int value;
	int size;
	int level;

	//for coude generation.Offset of base label.
	int offset;
	string conststring;
public:
	// get methods.
	SymbolItem(string _name, TokenKind _kind, TokenType _type, int _value=0) {
		name = _name;
		kind = _kind;
		type = _type;
		value = _value;
		size = 1;
		//the char here will be regarded as the int.
		//This may be changed.
	}
	string getName() { return name; }
	TokenKind getKind() { return kind; }
 	TokenType getType() { return type; }
	int getValue() { return value; }
	int getSize() { return size; }
	int getLevel() { return level; }
	int getOffset() { return offset; }
	string getString() { return conststring; }

	string getKindName() {
		switch (kind)
		{
		case TokenKind::CONST:return "const";
		case TokenKind::ARRAY:return "array";
		case TokenKind::FUNC:return "function";
		case TokenKind::PARA:return "parameter";
		case TokenKind::PARAVAR:return "var parameter";
		case TokenKind::PROC:return "procedure";
		case TokenKind::VAR:return "var";
		case TokenKind::TEMP:return "temp";
		case TokenKind::LABEL:return "label";
		case TokenKind::TEMP_ADD:return "temp_add";
		case TokenKind::TEMP_CON:return "temp_const";
		default: return "unknown";
		}
	}

	string getTypeName() {
		switch (type)
		{
		case TokenType::chartyp:return "char";
		case TokenType::inttyp:return "integer";
		case TokenType::notyp:return "notype";
		case TokenType::voidtyp:return "void";
		default: return "unknowntype";
		}
	}

	// set methods.
	void setName(string _name) { name = _name; }
	void setKind(TokenKind _kind) { kind = _kind; }
	void setType(TokenType _type) { type = _type; }
	void setValue(int _value) { value = _value; }
	void setSize(int _size) { size = _size; }
	void setLevel(int _level) { level = _level; }
	void setString(string _conststring) { conststring = _conststring; }
	void setOffset(int _offset) { offset = _offset; }


	void printData();
};

class SymbolPro {
private:
	int lastpar;
	int last;
public:
	SymbolPro(int _lastpar, int _last) :lastpar(_lastpar), last(_last) { assert(last - lastpar); }//lastpar < last
	int getLast() { return last; }
	int getLastPar() { return lastpar; }
};

enum ArrayType {
	arrayint,
	arraychar
};
class SymbolArray {
private:
	ArrayType ele_type;
	int low = 0;
	int high;
	int ele_size;
	int array_size;
public:
	SymbolArray(ArrayType _ele_type, int _high, int _ele_size):ele_type(_ele_type),high(_high),ele_size(_ele_size) {
		// for size indent.
		array_size = (1 + high)*ele_size;
		array_size = 4 - array_size % 4 + array_size;
		assert(array_size % 4 == 0);
	}
};

class SymbolSet{
public:
	// the pointer to the father of a procedure or a function.
	SymbolSet* father_table;

	//create and the delete method.
	SymbolSet(SymbolItem* _proc, SymbolSet* _father, int _level);

	//get the item from the table according to the name.
	SymbolItem* getItem(string _name);
	bool enterItem(SymbolItem* _item);

	//get the table the proc mapping to.

	SymbolItem* getProcItem();
	SymbolSet* getProcTable(string _procname);
	string		getProcName();
	SymbolSet* enterProc(SymbolItem* _proc);

	vector<SymbolItem*> getArgList();
	
	void printData();
	void calcOffset();
	//get the table's mapping item.

#ifdef DEBUG_SYMBOL
	void debug_printer();
#endif // DEBUG

	int getLevel();
	int getArgsSize();
	int getStackSize();
	int getDisplaySize();
	//calculate offset of var,para,var para,array based on base pointer($fp)

private:

	SymbolItem* proc;
	vector<SymbolItem*> local_list;
	map<string, SymbolItem*> symbol_map;
	//store the names of current level function or procedure.
	map<string, SymbolSet*> table_map;
	int level;
	int stack_size;
	int args_size;
	int display_size;
};