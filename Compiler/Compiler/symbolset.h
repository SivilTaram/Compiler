#ifndef SYMBOLSET_H
#define SYMBOLSET_H

#include <string>
#include <ctype.h>
#include "token.h"
#include <assert.h>
#include <array>     


#define MAX_LEVEL (3+1)

using namespace std;

class SymbolItem {
private:
	string name;
	TokenObj kind;
	TokenType type;
	int value;
	int size;
	int level;
	int link;
	//for coude generation.Offset of base label.
	int offset;
public:
	// get methods.
	string getName() { return name; }
	TokenObj getKind() { return kind; }
	TokenType getType() { return type; }
	int getValue() { return value; }
	int getSize() { return size; }
	int getLevel() { return level; }
	int getOffset() { return offset; }
	int getLink() { return link; }
	SymbolItem(string _name,TokenObj _kind,TokenType _type,int _value):kind(_kind),value(_value),name(_name),type(_type){}
	
	string getKindName() {
		switch (kind)
		{
		case TokenObj::constobj:return "const";
		case TokenObj::arrayobj:return "array";
		case TokenObj::funcobj:return "function";
		case TokenObj::paraobj:return "parameter";
		case TokenObj::paravarobj:return "var parameter";
		case TokenObj::procobj:return "procedure";
		case TokenObj::varobj:return "var";
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
	void setKind(TokenObj _kind) { kind = _kind; }
	void setType(TokenType _type) { type = _type; }
	void setValue(int _value) { value = _value; }
	void setSize(int _size) { size = _size; }
	void setLevel(int _level) { level = _level; }
	void setOffset(int _offset) { offset = _offset; }
	void setLink(int _link) { link = _link; }
};


enum TokenObj {
	constobj,
	varobj,
	procobj,
	funcobj,
	paraobj,
	paravarobj,
	arrayobj
};

enum TokenType {
	voidtyp,
	inttyp,
	chartyp,
	notyp,
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

class SymbolSet {
private:
	vector<SymbolItem> table;
	vector<SymbolPro> protable;
	vector<SymbolArray> arrtable;
	array<SymbolPro*, MAX_LEVEL> display;
	//if root,level = 0
	int level;
public:
	SymbolSet(){}
	int enter(SymbolItem item);
	int locIdent(string name) {

	}
	TokenType findIdent(string name);
	//enter a procedure or function.
	void enterproc(SymbolItem& _item,SymbolPro& _pro);
	//exit a procedure or function.
	void exitproc();
};
#endif