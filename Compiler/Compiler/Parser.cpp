#include "parser.h"
#include "lexer.h"
#include "error.h"
#include <queue>
#include <sstream>

//#define DEBUG_NEXT

#define LineNo (current_token.getLineNo())
#define PRINT(x) cout << LineNo <<" :"; for(int i=0;i<level;i++){cout<< "- ";} cout << x << endl
//#define DEBUG_NEXT
using namespace std;

#ifdef DEBUG
int level = 0;
#endif // DEBUG

const symset constbegsys = { 
	Symbol::plus ,
	Symbol::minus,
	Symbol::number,
	Symbol::charconst
};
const symset typebegsys = { 
	Symbol::integersym,
	Symbol::charsym,
	Symbol::arraysym
};
const symset blockbegsys = { 
	Symbol::constsym, 
	Symbol::varsym , 
	Symbol::procsym ,
	Symbol::beginsym, 
	Symbol::funcsym 
};
const symset facbegsys = {
	Symbol::ident,
	Symbol::lparen,
	Symbol::number
};
const symset statbegsys = { 
	Symbol::endsym,
	Symbol::semicolon,
	Symbol::beginsym,
	Symbol::ifsym,
	Symbol::writesym,
	Symbol::readsym,
	Symbol::dosym,
	Symbol::forsym,
	Symbol::ident
};

//except is suitable for one expression
//or something shorter.
void Parser::expect(Symbol sym,string meesage) {
	if (match(sym)) {
#ifdef DEBUG_EXCEPT
		switch (sym)
		{
		case nullsym:
			PRINT("nullsym "); break;
		case ident:
			PRINT("ident " ); break;
		case number:
			PRINT("number " ); break;
		case charconst:
			PRINT("charconst " ); break;
		case strconst:
			PRINT("strconst " ); break;
		case Symbol::minus:
			PRINT("- " ); break;
		case times:
			PRINT("* " ); break;
		case Symbol::plus:
			PRINT("+ " ); break;
		case slash:
			PRINT("/ " ); break;
		case eql:
			PRINT("= " ); break;
		case neq:
			PRINT("!= " ); break;
		case lss:
			PRINT("< " ); break;
		case leq:
			PRINT("<= " ); break;
		case gtr:
			PRINT("> " ); break;
		case geq:
			PRINT(">= " ); break;
		case lparen:
			PRINT("( " ); break;
		case rparen:
			PRINT(") " ); break;
		case lsquare:
			PRINT("[ " ); break;
		case rsquare:
			PRINT("] " ); break;
		case comma:
			PRINT(", " ); break;
		case semicolon:
			PRINT("; " ); break;
		case period:
			PRINT(". " ); break;
		case becomes:
			PRINT(":= " ); break;
		case colon:
			PRINT(": " ); break;
		case beginsym:
			PRINT("begin " ); break;
		case endsym:
			PRINT("end " ); break;
		case ifsym:
			PRINT("if " ); break;
		case thensym:
			PRINT("then " ); break;
		case elsesym:
			PRINT("else " ); break;
		case dosym:
			PRINT("do " ); break;
		case whilesym:
			PRINT("while " ); break;
		case forsym:
			PRINT("for " ); break;
		case downtosym:
			PRINT("downto " ); break;
		case tosym:
			PRINT("to " ); break;
		case constsym:
			PRINT("const " ); break;
		case varsym:
			PRINT("var " ); break;
		case procsym:
			PRINT("procedure " ); break;
		case funcsym:
			PRINT("function " ); break;
		case charsym:
			PRINT("char " ); break;
		case integersym:
			PRINT("integer " ); break;
		case arraysym:
			PRINT("array " ); break;
		case ofsym:
			PRINT("of " ); break;
		case readsym:
			PRINT("read " ); break;
		case writesym:
			PRINT("write " ); break;
		case eofsym:
			PRINT("eof " ); break;
		default:
			break;
		}
#endif // DEBUG
		next();
	}
	else {
		//Except something but it doesn't appear.
		error_handle.errorMessage(9,LineNo,meesage);
	}
}

bool Parser::match(Symbol sym) {
	if (Parser::current_token.getType() == sym)
		return true;
	else
		return false;
};

//return false -> end.
//return true -> next.
void Parser::next() throw(eofexception){
	//Read s symbol.
	current_token = token_lexer.getsym();
	//if symbol is '.' or the eof of the file,then end.
	if (current_token.getType() == Symbol::eofsym)
		throw eofexception();
#ifdef DEBUG_NEXT
	if (current_token.getType()==Symbol::charconst)
		cout << "'" << current_token.getName() << "'" <<endl;
	else if(current_token.getType()==Symbol::strconst)
		cout << "\"" << current_token.getValue() << "\"" <<endl;
	else if (current_token.getType() == Symbol::number)
		cout << current_token.getValue() << " " <<endl;
	else if (current_token.getType() == Symbol::ident)
		cout << current_token.getType() << " name: " << current_token.getName() << " " <<endl;
	else
		cout << current_token.getType() << endl;
#endif
};

string Parser::getErrorString(Token token) {
	stringstream outstream;
	if (token.getType() == Symbol::ident)
		outstream << token.getName();
	else if (token.getType() == Symbol::charconst)
		outstream << (char)token.getValue();
	else if (token.getType() == Symbol::strconst)
		outstream << token.getName();
	else if (token.getType() == Symbol::number)
		outstream << token.getValue();
	return outstream.str();
}

//accoring the ident_name to get the symbol item.
//Important!!!:the ident_name include the function's name or the procedure's name.
SymbolItem* Parser::get(string _ident_name) {
	SymbolItem* item = symbol_set.search(_ident_name);
	if (item != NULL)
		return item;
	//The Undefined ident;
	error_handle.errorMessage(40, LineNo,_ident_name);
	item = symbol_set.insert(_ident_name, TokenKind::UNDEFINE, TokenType::notyp);
	return item;
}

//skip some words until a valid follow set.
void Parser::skip(symset fsys,int error_code) {
	//read while the next() into.
	if (fsys.find(current_token.getType()) != fsys.end())
		return;
	error_handle.errorMessage(error_code, LineNo);
	while (fsys.find(current_token.getType()) == fsys.end())
	{
		next();
	}
};
//test whether the current_token is valid.
void Parser::test(symset s1,symset s2,int error_code) {
	//not found
	if (s1.find(current_token.getType()) == s1.end()) {
		symset stop_set(s1.begin(), s1.end());
		stop_set.insert(s2.begin(), s2.end());
		skip(stop_set,error_code);
	}

};

void Parser::translate() {
	if (error_handle.IsSuccess()) {
		symbol_set.calcOffset();
		symbol_set.printSymbolSet();
		middle_code.printMiddleCode();
		MipsInstr* mips_instr = new MipsInstr(middle_code, symbol_set);
		mips_instr->translate();
	}
	else
		error_handle.print();
}
/*
Start parse.
*/void Parser::parser() {
	next();
	block(blockbegsys);
	if (!match(Symbol::period))
		//Number50: the program ended not normally
		error_handle.errorMessage(50, LineNo);
	else {
#ifdef DEBUG
		level++;
		PRINT("period");
		level--;
#endif // DEBUG
	}
}

/*
block's parser.
<分程序>        ::=   [<常量说明部分>][<变量说明部分>]{[<过程说明部分>]| [<函数说明部分>]}<复合语句>
<过程说明部分>  ::=  <过程首部><分程序>{; <过程首部><分程序>};
<函数说明部分>  ::=  <函数首部><分程序>{; <函数首部><分程序>};
<常量说明部分> first set = {const}
<变量说明部分> first set = {var}
<过程说明部分> first set = {procedure}
<函数说明部分> first set = {function}
*/
void Parser::block(symset _block) {
#ifdef DEBUG
	level++;
	PRINT("block");
#endif // DEBUG
	try {
		skip(blockbegsys, 62);
		if (match(Symbol::constsym)) {
			next();
			constDec();
		}
		//[FIXME]:there is a conflution:
		//const must be declared before var.
		//but how can we control it?
		if (match(Symbol::varsym))
		{
			next();
			variableDec();
		}
		while (match(Symbol::procsym) || match(Symbol::funcsym))
		{
			//the first symbol of proc.
			if (match(Symbol::procsym))
			{
				//for some methods which appeared without essential,
				//we must read the first word to confirm that they have
				//appeared,but for those methods which appeared with essential,
				//we must also read the first word for more accurate error message.
				next();
				//the loop is outside,and procDec can't have a loop
				procDec();
				block(blockbegsys);
				symbol_set.goback();
				expect(Symbol::semicolon, ";");
			}
			else {
				next();
				funcDec();
				block(blockbegsys);
				//SymbolTable go back.
				symbol_set.goback();
				expect(Symbol::semicolon, ";");
			}
		}
		//the begin of the block;
		middle_code.gen(Opcode::BEGIN, (SymbolItem*)symbol_set.getCurrentSet(),NULL,NULL);
		compoundStatement();
		//the end of the block;
		middle_code.gen(Opcode::END, (SymbolItem*)symbol_set.getCurrentSet(),NULL,NULL);
	}
	catch (eofexception) {
	}
#ifdef DEBUG
	level--;
#endif // DEBUG

}

//Handle const declaration
//<常量说明部分> :: = const<常量定义>{,<常量定义>};
void Parser::constDec() {
#ifdef DEBUG
	level++;
	PRINT("const Declaration");
#endif // DEBUG
	if (match(Symbol::ident))
	{
		while (1) {
			constDef();
			if (match(Symbol::comma))
				next();
			else
				break;
		}
		// except ";" here.
		expect(Symbol::semicolon, ";");
	}
	else
	{
		// I am confused by my index of errors. Oh my god....
		// error_handle.errorMessage(41, LineNo);
	}
#ifdef DEBUG
	level--;
#endif // DEBUG

};

//<常量定义> ::= <标识符>＝ <常量>
//<常量>     ::= [+| -] <无符号整数>|<字符>
void Parser::constDef() {
#ifdef DEBUG
	level++;
	PRINT("const Definition");
#endif // DEBUG

	// if there is a identiter.
	// const should be added into the symbolset.
	//read the ident twice.
	if (match(Symbol::ident)) {
		//const_name
		string ident_name = current_token.getName();
		next();
		// I have just estimated the PL0...
		if (match(Symbol::eql) || match(Symbol::becomes)) {
			if (match(Symbol::becomes))
				//[Error]:There should be a eql.
				error_handle.errorMessage(42, LineNo);
			next();
			bool minus = false;
			skip(constbegsys, 61);
			if (match(Symbol::plus) || match(Symbol::minus)) {
				if (match(Symbol::minus))
					minus = true;
				next();
			}
			if (match(Symbol::number))
			{
				//get the value of number
				int number_value = current_token.getValue();
				if (minus)
					number_value = - number_value;
				//[FIXME]:enter the symbol set.
				//There should be a number.
				if (!symbol_set.insert(ident_name, TokenKind::CONST, TokenType::inttyp, number_value))
				{
					error_handle.errorMessage(42, LineNo,ident_name);
				}
				next();
			}
			else if (match(Symbol::charconst)) {
				//[FXIMe]:enter the symbol set.
				// if this is a charconst, and the name means its char.
				char char_value = current_token.getValue();
				if (!symbol_set.insert(ident_name, TokenKind::CONST, TokenType::chartyp,char_value))
					error_handle.errorMessage(42, LineNo,ident_name);
				next();
			}
			else {
				//when we meet a "," we should do next;
				error_handle.errorMessage(16, LineNo,ident_name);
			}
		}
	}
#ifdef DEBUG
	level--;
#endif
}

//Handle var declaration
// <变量说明部分>  :: = var <变量说明>; {<变量说明>; }
void Parser::variableDec() {
#ifdef DEBUG
	level++;
	PRINT("variable Dec");
#endif // DEBUG
	while (1) {
		if (!match(Symbol::ident))
			error_handle.errorMessage(19, LineNo, getErrorString(current_token));
		skip({ Symbol::ident }, 65);
		variableDef();
		expect(Symbol::semicolon, ";");
		//identifier or others.
		if (match(Symbol::ident))
			continue;
		else
			break;
	}
#ifdef DEBUG
	level--;
#endif
}

//Hanle procedure and function declaration
//<变量说明> ::= <标识符>{, <标识符>} : <类型>
void Parser::variableDef() {
#ifdef DEBUG
	level++;
	PRINT("variableDefinition");
#endif // DEBUG
	queue<string>* var_name = new queue<string>();
	while (1) {
		if (match(Symbol::ident)) {
			string ident_name = current_token.getName();
			var_name->push(ident_name);
		}
		else {
			error_handle.errorMessage(19, LineNo, getErrorString(current_token));
		}
		next();
		// :
		if (match(Symbol::comma))
			next();
		else
			break;
	}
	if (match(Symbol::colon)) {
		next();
		skip(typebegsys, 66);
		varType(var_name);
	}
#ifdef DEBUG
	level--;
#endif
}

//将var的具体类型填入到符号表中
//<类型> :: = <基本类型> | <数组类型>
void Parser::varType(queue<string>* var_name) {
#ifdef DEBUG
	level++;
	PRINT("varType");
#endif // DEBUG

	if (match(Symbol::integersym) || match(Symbol::charsym))
	{
		//enter the table with the var's type.
		// first in first out.Then the ident_name should be in order.
		//And we should do util the queue is empty.
		if (match(Symbol::integersym)) {
			while (!(var_name->empty())) {
				// the front of var_name is the latest variables.
				//[Error] Var repeated.
				if (!symbol_set.insert(var_name->front(), TokenKind::VAR, TokenType::inttyp))
					error_handle.errorMessage(43, LineNo,var_name->front());
				var_name->pop();
			}
		}
		else if (match(Symbol::charsym)) {
			while (!(var_name->empty())) {
				// the front of var_name is the latest variables.
				//[Error] Var repeated.
				if (!symbol_set.insert(var_name->front(), TokenKind::VAR, TokenType::chartyp))
					error_handle.errorMessage(43, LineNo,var_name->front());
				var_name->pop();
			}
		}
		next();
	}
	//array[10] of integer;
	else if (match(Symbol::arraysym))
	{
		selector(var_name);
	}
#ifdef DEBUG
	level--;
#endif // DEBUG

}

/*
<过程首部>   ::=   procedure<标识符>[<形式参数表>];
<形式参数表> ::= '('<形式参数段>{; <形式参数段>}')'
*/
void Parser::procDec() {
#ifdef DEBUG
	level++;
	PRINT("procedure Declaration");
#endif // DEBUG

	if (match(Symbol::ident))
	{
		SymbolSet* current_table = symbol_set.getCurrentSet();
		string proc_name = current_table->getProcName() + current_token.getName()+"_";
		SymbolItem* proc = symbol_set.insert(proc_name, TokenKind::PROC, TokenType::voidtyp);
		if (proc == NULL)
			//the procedure is built.
			error_handle.errorMessage(41,LineNo,proc_name);
		next();
		// if parameter table's first is '(',then loop
		// to map the parameter list.
		if (match(Symbol::lparen))
		{
			//read one again;
			next();
			while (1) {
				parameterList();
				if (match(Symbol::rparen))
				{
					next();
					break;
				}
				// ';' and read next
				expect(Symbol::semicolon, ";");
			}
		}
		//except a ';'
		expect(Symbol::semicolon, ";");
	}
	else {
		//No.11 should define identity at the first of proc.After procedure.
		error_handle.errorMessage(11, LineNo, getErrorString(current_token));
	}
#ifdef DEBUG
	level--;
#endif
};

//Handle the parameterlist of function or procedure declaration.
//<形式参数段> ::= [var]<标识符>{, <标识符>}: <基本类型>
void Parser::parameterList() {
#ifdef DEBUG
	level++;
	PRINT("parameter List");
#endif // DEBUG
	TokenKind kind = TokenKind::PARA;
	if (match(Symbol::varsym))
	{
		kind = TokenKind::PARAVAR;
		next();
		//[FIXME] there should be a enter table's handle.
		//And the param is [var]!!!
	}
	queue<string> *args = new queue<string>();
	while (1) {
		if (match(Symbol::ident)) {
			args->push(current_token.getName());
			next();
		}
		if (match(Symbol::colon))
		{
			next();
			break;
		}
		expect(Symbol::comma,",");
	}
	//basic type.
	//kind means var para or normal para.
	basicType(args,kind);
#ifdef DEBUG
	level--;
#endif // DEBUG

}

//<基本类型>  ::=  integer | char
void Parser::basicType(queue<string> *args,TokenKind kind) {
#ifdef DEBUG
	level++;
	PRINT("basic Type");
#endif // DEBUG
	TokenType type = TokenType::inttyp;
	if (match(Symbol::integersym) || match(Symbol::charsym))
	{
		if (match(Symbol::charsym))
			type = TokenType::chartyp;
		next();
	}
	else
	{
		//No.12 except type after ':'
		error_handle.errorMessage(12,LineNo, getErrorString(current_token));
		next();
	}
	while (!(args->empty())) {
		if (!(symbol_set.insert(args->front(), kind, type)))
			error_handle.errorMessage(43, LineNo,args->front());
		args->pop();
	}
#ifdef DEBUG
	level--;
#endif // DEBUG
}

//Handle funcDec
//<函数首部> ::= function <标识符>[<形式参数表>]: <基本类型>;
void Parser::funcDec() {
	if (match(Symbol::ident))
	{
#ifdef DEBUG
		level++;
		PRINT("function Declaration "+ current_token.getName());
#endif // DEBUG
		SymbolSet* current_table = symbol_set.getCurrentSet();
		string func_name = current_table->getProcName() + current_token.getName() + "_";
		//void is the temporatory return type!!!
		SymbolItem* item = symbol_set.insert(func_name, TokenKind::FUNC, TokenType::voidtyp);
		if (item == NULL)
		{
			//[Error]:redefintion.
			error_handle.errorMessage(44, LineNo, getErrorString(current_token));
			//we should confrim the func is existing.
			item = symbol_set.search(func_name);
		}
		next();
		// if parameter table's first is '(',then loop
		// to map the parameter list.
		if (match(Symbol::lparen))
		{
			//read one again;
			next();
			while (1) {
				parameterList();
				if (match(Symbol::rparen))
				{
					next();
					break;
				}
				// ';' and read next
				expect(Symbol::semicolon, ";");
			}
		}
		//the begin of the basic type.
		//except : then basictype and then ;
		if (match(Symbol::colon))
		{
			next();
			TokenType type = TokenType::notyp;
			if (match(Symbol::charsym))
				type = TokenType::chartyp;
			else if (match(Symbol::integersym))
				type = TokenType::inttyp;
			else
			{
				type = TokenType::notyp;
				// notyp not a return type!
				error_handle.errorMessage(45, LineNo, getErrorString(current_token));
			}
			//set the function's return type.
			if(item!=NULL)
				item->setType(type);
			next();
		}
		else
		{
			//No.13 There 
			error_handle.errorMessage(13, LineNo,func_name);
		}
		expect(Symbol::semicolon, ";");
	}
	else {
		//No.11 should define identity at the first of proc.After procedure.
		error_handle.errorMessage(11, LineNo);
		//skip();
	}
#ifdef DEBUG
	level--;
#endif
};

//Handle statements
//<语句> ::= <赋值语句>|<条件语句>|<当循环语句>|<过程调用语句>|<复合语句>|<读语句>|<写语句>|<for循环语句>|<空>
void Parser::statement() {
#ifdef DEBUG
	level++;
	PRINT("statement");
#endif // DEBUG

	//assignment or proc or func
	if (match(Symbol::ident)) {
		string ident_name = current_token.getName();
		SymbolItem* ident_item = get(ident_name);
		next();
		//proc or func there should be a paren
		//<过程调用语句>  ::=  <标识符>[<实在参数表>]
		if(match(Symbol::becomes) || match(Symbol::lsquare))
		{
			assignment(ident_item);
		}
		else {
			if(ident_item!=NULL && ident_item->getKind()==TokenKind::PROC)
				callPro(ident_item,ident_name);
		}
	}
	else if (match(Symbol::beginsym))
		//Note!!! the compoundstatement read the begin as the first!
		compoundStatement();
	else if (match(Symbol::ifsym))
		ifStatement();
	else if (match(Symbol::dosym))
		whileStatement();
	else if (match(Symbol::forsym))
		forStatement();
	else if (match(Symbol::readsym))
		readStatement();
	else if (match(Symbol::writesym))
		writeStatement();
	// ;
	else if (match(Symbol::semicolon) || match(Symbol::endsym))
	{
	//this is a blank statement.
#ifdef DEBUG
		level++;
		PRINT("blank statement");
		level--;
#endif // DEBUG
		;
	}
	else
		//No.15 Unexpected word.
		error_handle.errorMessage(15, LineNo);
#ifdef DEBUG
	level--;
#endif // DEBUG

}

//Handle expressions
//<表达式> ::= [+|-]<项>{<加法运算符><项>}
SymbolItem* Parser::expression() {
#ifdef DEBUG
	level++;
	PRINT("expression");
#endif // DEBUG
	bool minus = false;
	if (match(Symbol::plus) || match(Symbol::minus)){
		//matching means there is minus number.
		if (match(Symbol::minus))
			minus = true;
		next();
	}
	SymbolItem* first_item = item();
	SymbolItem* temp = NULL;

	if (minus) {
		temp = symbol_set.genTemp(TokenKind::TEMP, first_item->getType());
		// temp = - first_item;
		middle_code.gen(Opcode::NEG, temp, first_item, NULL);
		first_item = temp;
	}

	while (match(Symbol::plus) || match(Symbol::minus)) {
		Opcode op = match(Symbol::plus) ? Opcode::ADD : Opcode::SUB;
		next();
		//the second item to calc
		SymbolItem* second_item = item();
		temp = symbol_set.genTemp(TokenKind::TEMP, first_item->getType());
		if (first_item->getType() != second_item->getType())
			temp->setType(TokenType::inttyp);
		middle_code.gen(op, temp, first_item, second_item);
		//for itearting.
		first_item = temp;
	}

	return first_item;
#ifdef DEBUG
	level--;
#endif // DEBUG

}

//Handle the index varaiable of array
//<因子> ::= <标识符>|<标识符>'['<表达式>']'|<无符号整数>| '('<表达式>')' | <函数调用语句>
SymbolItem* Parser::factor(){
#ifdef DEBUG
	level++;
	PRINT("factor");
#endif // DEBUG
	//if there is a array.should like this: array[(expr)];
	if (match(Symbol::ident)) {
		// array
		string ident_name = current_token.getName();
		// the name of the array.
		// the frist address of the array.
		SymbolItem* ident = get(ident_name);
		next();
		// [ 
		if (match(Symbol::lsquare))
		{
			if (ident->getKind() != TokenKind::ARRAY)
				error_handle.errorMessage(46, LineNo,ident_name);
			next();
			// 表达式
			SymbolItem* expr = expression();
			// ]
			if (expr != NULL && ident != NULL)
			{
				//[Error]:there should be a array out of bound.
				SymbolItem* temp = symbol_set.genTemp(TokenKind::TEMP_ADD, ident->getType());
				//////////////////////////////////////
				// should a gen to store the address of array.
				/////////////////////////////////////
				middle_code.gen(Opcode::ARRADD, temp, ident, expr);
				SymbolItem* value = symbol_set.genTemp(TokenKind::TEMP, ident->getType());
				middle_code.gen(Opcode::ARRASS, value, temp, NULL);
				expect(Symbol::rsquare, "]");
				return value;
			}
			return 0;
		}
		// func()
		else if (match(Symbol::lparen))
		{
			//callFunc and generate the function code.
			SymbolItem* func = NULL;
			if(ident!=NULL && ident->getKind()== TokenKind::FUNC)
				func = callFunc(ident,ident_name);
			return func;
		}
		else if (ident!=NULL && ident->getKind() == TokenKind::FUNC) {
			callFunc(ident,ident_name);
			return ident;
		}
		else {
			return ident;
		}
	}
	// 45
	else if (match(Symbol::number)) {
		//should be a temp const number;
		SymbolItem * temp = symbol_set.genTemp(TokenKind::TEMP_CON, TokenType::inttyp);
		temp->setValue(current_token.getValue());
		next();
		return temp;
	}
	// ( 3 + 4 )
	else if(match(Symbol::lparen)) {
		next();
		SymbolItem * expr = expression();
		expect(Symbol::rparen,")");
		return expr;
	}
	else {
		error_handle.errorMessage(19, LineNo, getErrorString(current_token));
	}
#ifdef DEBUG
	level--;
#endif // DEBUG
	return NULL;
}

//<数组类型> ::= array'['<无符号整数>']' of <基本类型>
void Parser::selector(queue<string>* var_name) {
#ifdef DEBUG
	level++;
	PRINT("selector");
#endif // DEBUG
	expect(Symbol::arraysym,"array");
	if (match(Symbol::lsquare)) {
		next();
		int array_size = current_token.getValue();
		expect(Symbol::number,"数字");
		//array_size;
		//enter the array_table;
		expect(Symbol::rsquare,"]");
		expect(Symbol::ofsym,"of");
		TokenType temp;
		if (match(Symbol::integersym) || match(Symbol::charsym))
		{
			if (match(Symbol::integersym))
				temp = TokenType::inttyp;
			else
				temp = TokenType::chartyp;
		}
		else {
			//NOT CORRET TYPE;
			error_handle.errorMessage(12, LineNo, getErrorString(current_token));
			temp = TokenType::notyp;
		}
		//enter the array_table;
		while (!var_name->empty()){
			SymbolItem *array_item = symbol_set.insert(var_name->front(), TokenKind::ARRAY, temp);
			if (array_item != NULL)
				array_item->setSize(array_size);
			var_name->pop();
		}
		next();
	}
	
#ifdef DEBUG
	level--;
#endif // DEBUG
}

//Handle the call of procedure and function
//<过程调用语句>  :: = <标识符>[<实在参数表>]
void Parser::callPro(SymbolItem* proc,string proc_name) {
	if (proc == NULL)
		return;
	else if (proc->getKind() != TokenKind::PROC) {
		error_handle.errorMessage(57, LineNo,proc->getName());
		return;
	}
	if (match(Symbol::lparen)) {
		realParameter(proc,proc_name);
	}
	else {
		SymbolSet* callee_func = symbol_set.serachTable(proc->getName());
		SymbolSet* caller_func = symbol_set.getCurrentSet();
		middle_code.gen(Opcode::CALL, (SymbolItem*)caller_func, (SymbolItem*)callee_func, NULL);
	}
}

//<函数调用语句>  :: = <标识符>[<实在参数表>]
SymbolItem* Parser::callFunc(SymbolItem* func,string func_name) {
	if (func == NULL)
		return symbol_set.genTemp(TokenKind::TEMP, TokenType::notyp);
	if (match(Symbol::lparen)) {
		SymbolItem* return_value = realParameter(func,func_name);
		//let the return_value to store the value of func.
		middle_code.gen(Opcode::ASS,return_value, func,NULL);
		return return_value;
	}
	else {
		SymbolSet* callee_func = symbol_set.serachTable(func->getName());
		SymbolSet* caller_func = symbol_set.getCurrentSet();
		middle_code.gen(Opcode::CALL, (SymbolItem*)caller_func, (SymbolItem*)callee_func, NULL);
	}
	return NULL;
}

//<实在参数表>    :: = '(' <实在参数> {, <实在参数>}')'
//<实在参数>      :: = <表达式>
SymbolItem* Parser::realParameter(SymbolItem* func,string func_name) {
#ifdef DEBUG
	level++;
	PRINT("real Parameter");
#endif // DEBUG
	vector<SymbolItem*> form_parameters = symbol_set.getArgList(func->getName());
	vector<SymbolItem*> real_parameters;
	if (match(Symbol::lparen)) {
		next();
		while (1) {
			//add the args to the realparameters.
			real_parameters.push_back(expression());
			if (!match(Symbol::comma))
				break;
			next();
		}
	}
	expect(Symbol::rparen,")");
	//from the begin to the end
	vector<SymbolItem*>::iterator form_iter = form_parameters.begin();
	vector<SymbolItem*>::iterator real_iter = real_parameters.begin();

	//we should check that the [var] should be a var 
	//not a expr ,not a const , even a func!
	while (real_iter!=real_parameters.end()) {
		if (form_iter == form_parameters.end()) {
			//FormParameters can't match the RealParameters.
			error_handle.errorMessage(55, LineNo,func_name);
			break;
		}
		else if (
			(
				(*form_iter)->getKind() == TokenKind::PARAVAR
				//if the expression just procedure one,then can't temp;
				&& 
				(
					   (*real_iter)->getKind() != TokenKind::VAR
					&& (*real_iter)->getKind() != TokenKind::PARA
					&& (*real_iter)->getKind() != TokenKind::PARAVAR)
				)
			||
			(
				(*form_iter)->getKind() == TokenKind::PARA
				&& 
				(
					   (*real_iter)->getKind() !=TokenKind::VAR
					&& (*real_iter)->getKind() !=TokenKind::PARA
					&& (*real_iter)->getKind() !=TokenKind::PARAVAR
					&& (*real_iter)->getKind() !=TokenKind::CONST
					&& (*real_iter)->getKind() !=TokenKind::TEMP
					&& (*real_iter)->getKind() !=TokenKind::TEMP_ADD
					&& (*real_iter)->getKind() !=TokenKind::TEMP_CON
					)
				)
			)
		{
			//the real parameter's type can't match
			error_handle.errorMessage(56, LineNo ,func_name);
		}
		real_iter++;
		form_iter++;
	}

	if (form_iter != form_parameters.end())
	{
		error_handle.errorMessage(55, LineNo,func_name);
	}

	real_iter = real_parameters.begin();
	form_iter = form_parameters.begin();
	while (real_iter != real_parameters.end()) {
		if (form_iter != form_parameters.end()) {
			if ((*form_iter)->getKind() == TokenKind::PARAVAR)
				middle_code.gen(Opcode::PUSHVAR, (*real_iter),NULL,NULL);
			else
				middle_code.gen(Opcode::PUSH, (*real_iter),NULL,NULL);
			form_iter++;
		}
		else {
			break;
		}
		real_iter++;
	}

	//gen a call func quater.
	//I want to generate a quater with push and var.
	SymbolSet* callee_func = symbol_set.serachTable(func->getName());
	SymbolSet* caller_func = symbol_set.getCurrentSet();
	middle_code.gen(Opcode::CALL,(SymbolItem*)caller_func,(SymbolItem*)callee_func,NULL);

	//if this is a function and we should return its value
	//for transfering it to others.
	if (func->getKind() == TokenKind::FUNC) {
		SymbolItem* temp = symbol_set.genTemp(TokenKind::TEMP, func->getType());
		return temp;
	}
	else
		return NULL;

#ifdef DEBUG
	level--;
#endif // DEBUG
	return NULL;
}
//Handle the for statement
//<for循环语句>:: = for <标识符>  : = <表达式> （downto | to） <表达式> do <语句> //步长为1
void Parser::forStatement() {
#ifdef DEBUG
	level++;
	PRINT("for statement");
#endif // DEBUG
	SymbolItem* first_ident = NULL;
	expect(Symbol::forsym,"for");
	if (match(Symbol::ident)) {
		string ident_name = current_token.getName();
		first_ident = get(ident_name);
		if (first_ident !=NULL && first_ident->getKind()!= TokenKind::VAR)
			error_handle.errorMessage(57, LineNo);
		next();
	}
	else {
		error_handle.errorMessage(19,LineNo);
		next();
	}
	// :=
	expect(Symbol::becomes,":=");
	SymbolItem* initial_value = expression();
	// first_ident = initial_value;
	middle_code.gen(Opcode::ASS, first_ident, initial_value,NULL);

	bool downto = false;

	if (match(Symbol::downtosym) || match(Symbol::tosym)) {
		downto = match(Symbol::downtosym) ? true : false;
		next();
	}
	else {
		error_handle.errorMessage(24, LineNo);
		next();
	}

	SymbolItem* loop_label = symbol_set.genLabel();
	SymbolItem* state_label = symbol_set.genLabel();
	SymbolItem* end_loop_label = symbol_set.genLabel();
	//JUMP like the BR in P-code
	middle_code.gen(Opcode::SETL, loop_label,NULL,NULL);

	//expression
	SymbolItem* last_value = expression();

	middle_code.gen(Opcode::JUMP, end_loop_label, NULL, NULL);

	expect(Symbol::dosym,"do");

	//set the label of statement.
	middle_code.gen(Opcode::SETL, state_label, NULL, NULL);

	// statement
	statement();

	if (downto == true && first_ident != NULL)
		middle_code.gen(Opcode::DEC, first_ident,NULL,NULL);
	else if (downto == false && first_ident != NULL)
		middle_code.gen(Opcode::INC, first_ident,NULL,NULL);

	middle_code.gen(Opcode::JUMP, loop_label, NULL, NULL);

	middle_code.gen(Opcode::SETL, end_loop_label, NULL, NULL);

	if (downto == true)
		middle_code.gen(Opcode::BGE, state_label, first_ident, last_value);
	else
		middle_code.gen(Opcode::BLE, state_label, first_ident, last_value);


	/*
		init;(ASS)
loop:	(SETL)
		expression();
		J end_loop_label;(J)
state:  (SETL)
		statement;
		decrement/incerement;(DEC/INC)
		j loop_label;(J)
endloop:(SETL)
		BLE state_label;(BLE)

	
	*/


#ifdef DEBUG
	level--;
#endif // DEBUG

}

//Handle the while statement
//<当循环语句> :: = do<语句> while<条件>
void Parser::whileStatement() {
#ifdef DEBUG
	level++;
	PRINT("while statement");
#endif // DEBUG
	// do

	SymbolItem* loop = symbol_set.genLabel();
	expect(Symbol::dosym,"do");
	middle_code.gen(Opcode::SETL, loop,NULL,NULL);
	statement();
	expect(Symbol::whilesym,"while");
	condition(loop,true);
#ifdef DEBUG
	level--;
#endif // DEBUG
	/*

	(do once and just)
loop:
	<statement>

	if meet the requirements,jump loop.

endloop :
	*/
}

//Handle the if statement;
//<条件语句> :: = if<条件>then<语句> | if<条件>then<语句>else<语句>
void Parser::ifStatement() {
#ifdef DEBUG
	level++;
	PRINT("if statement");
#endif // DEBUG
	expect(Symbol::ifsym,"if");
	SymbolItem* else_label = symbol_set.genLabel();
	SymbolItem* endif_label = symbol_set.genLabel();
	//the branch is set in condition.
	//IMPORTANT:if false
	condition(else_label,false);
	if (match(Symbol::thensym)) {
		next();
		statement();
		middle_code.gen(Opcode::JUMP, endif_label,NULL,NULL);
	}
	middle_code.gen(Opcode::SETL, else_label, NULL, NULL);
	if (match(Symbol::elsesym))
	{
#ifdef DEBUG
		PRINT("else");
#endif // DEBUG
		next();
		statement();
	}
#ifdef DEBUG
	level--;
#endif // DEBUG
	middle_code.gen(Opcode::SETL, endif_label,NULL,NULL);
	/*

			condition;
			Bxx then_label(if false,then false);
			true statement;
			JUMP endif_label;
else_label: 
			false_statement;
endif_label:

	
	*/

}

//<条件>       ::= <表达式><关系运算符><表达式>
//<关系运算符> ::= <|<=|>|>= |=|<>
//if success,then we should generate
void Parser::condition(SymbolItem* else_label,bool _ifsuccess=true) {
	//表达式
	SymbolItem* src1 = expression();
	Symbol op = Symbol::nullsym;
	if (match(Symbol::leq) || match(Symbol::lss) || match(Symbol::gtr) || match(Symbol::geq) || match(Symbol::eql) || match(Symbol::neq)) {
		op = current_token.getType();
		next();
		//表达式
		SymbolItem* src2 = expression();
		Opcode op_code;
			if(_ifsuccess) {
				op_code = 
					(op == Symbol::leq) ? Opcode::BLE :
					(op == Symbol::lss) ? Opcode::BLS :
					(op == Symbol::gtr) ? Opcode::BGR :
					(op == Symbol::geq) ? Opcode::BGE :
					(op == Symbol::eql) ? Opcode::BEQ :
					(op == Symbol::neq) ? Opcode::BNE : Opcode::JUMP;
			}
			else {
				op_code =
					//<= false >
					(op == Symbol::leq) ? Opcode::BGR :
					//< false >=
					(op == Symbol::lss) ? Opcode::BGE :
					//> false <=
					(op == Symbol::gtr) ? Opcode::BLE :
					//>= flase <
					(op == Symbol::geq) ? Opcode::BLS :
					//== false <>
					(op == Symbol::eql) ? Opcode::BNE :
					//<> false ==
					(op == Symbol::neq) ? Opcode::BEQ : Opcode::JUMP;
			}
		middle_code.gen(op_code, else_label,src1,src2);
	}
	else {
		error_handle.errorMessage(26, LineNo);
	}
}

//Handle compound statement
// <复合语句>       ::=  begin<语句>{; <语句>}end
void Parser::compoundStatement() {
#ifdef DEBUG
	level++;
	PRINT("compound Statement");
#endif // DEBUG
	skip(statbegsys, 67);
	expect(Symbol::beginsym, "begin");
	statement();
	while (1) {
		// except ';'
		if (!match(Symbol::semicolon))
			break;
		next();
		statement();
	}
	if (match(Symbol::endsym)) {
#ifdef DEBUG
		PRINT("end");
#endif // DEBUG
		next();
	}
	if (match(Symbol::eofsym)) {
		//No.14 Unexcepted end of the file
		error_handle.errorMessage(14, LineNo);
		throw eofexception();
	}
#ifdef DEBUG
	level--;
#endif // DEBUG

}

//Handle the assign
//<赋值语句>      ::=  <标识符> := <表达式>| <函数标识符> := <表达式> | <标识符>'['<表达式>']':= <表达式>
void Parser::assignment(SymbolItem* ident) {
#ifdef DEBUG
	level++;
	PRINT("assignment");
#endif // DEBUG

	SymbolItem* dst = ident;
	SymbolItem* src1 = NULL;
	SymbolItem* src2 = NULL;

	//must be ":=" or "[" 
	// ident[
	if (match(Symbol::lsquare)) {
		next();
		//the index of the array.
		src1 = expression();
		// ]
		if (dst != NULL && dst->getKind() != TokenKind::ARRAY)
			error_handle.errorMessage(46, LineNo , dst->getName());

		//There we can give out a error which belong to the type of out of bound.
		expect(Symbol::rsquare,"]");
		expect(Symbol::becomes,":=");
		// dst[src1] := src2;
		src2 = expression();
		//the type of the src2 is different from the type of the dst.
		if (src2!=NULL && dst!=NULL && src2->getType() != dst->getType())
			error_handle.errorMessage(47, LineNo,src2->getName(),dst->getName());
		SymbolItem *addr = symbol_set.genTemp(TokenKind::TEMP_ADD, dst->getType());
		// fill the addr with the base address of array and the offset.
		// addr = ADDR(dst) + src1;
		middle_code.gen(Opcode::ARRADD, addr, dst, src1);
		// [addr] = src2;
		middle_code.gen(Opcode::ASSADD, addr, src2,NULL);
	}
	// ident :=
	else {
		expect(Symbol::becomes,":=");
		// dst := src1;
		src1 = expression();
		if (dst != NULL && 
			src1 != NULL &&
			(dst->getKind() == TokenKind::VAR
			|| dst->getKind() == TokenKind::PARA
			|| dst->getKind() == TokenKind::PARAVAR)) {
			if (dst->getType() == src1->getType()
				||(dst->getType() == TokenType::inttyp
					&& src1->getType()==TokenType::chartyp))
				middle_code.gen(Opcode::ASS, dst, src1,NULL);
			else
				error_handle.errorMessage(47, LineNo,src1->getName(),dst->getName());
		}
		// (a func)dst := src1;
		else if (dst != NULL && (dst->getKind() == TokenKind::FUNC)) {
			if (!symbol_set.findProc(dst->getName())) {
				// no func!
				error_handle.errorMessage(40, LineNo,dst->getName());
			}
			else {
				if (dst->getType() != src1->getType())
					error_handle.errorMessage(47, LineNo);
				middle_code.gen(Opcode::ASS, dst, src1,NULL);
			}

		}
	}
#ifdef DEBUG
	level--;
#endif // DEBUG
}

//<项>::= <因子>{<乘法运算符><因子>}
//We should put every temporary variables into one trying our best.
SymbolItem* Parser::item() {
#ifdef DEBUG
	level++;
	PRINT("item");
#endif // DEBUG
	//the first factor.
	skip(facbegsys, 63);
	SymbolItem * first_factor = factor();
	SymbolItem * temp = NULL;
	
	//times means "*";
	//slash means "/";
	Opcode op;
	while (match(Symbol::times) || match(Symbol::slash)) {
		if (match(Symbol::times))
			op = Opcode::MUL;
		else
			op = Opcode::DIV;
		next();
		skip(facbegsys, 63);
		SymbolItem *second_factor = factor();
		//there should be a temp varaiable.
		temp = symbol_set.genTemp(TokenKind::TEMP, first_factor->getType());
		//there should be a resulttype.
		//for example,should a char to int or others.
		//But I don't have time to do this now
		middle_code.gen(op, temp, first_factor, second_factor);
		//let the first_factor store the result.
		first_factor = temp;
	}
#ifdef DEBUG
	level--;
#endif // DEBUG
	return first_factor;
}

//<读语句> ::=  read'('<标识符>{,<标识符>}')'
void Parser::readStatement() {
#ifdef DEBUG
	level++;
	PRINT("read statement");
#endif // DEBUG
	expect(Symbol::readsym,"read");
	expect(Symbol::lparen,"(");
	vector<SymbolItem*> args;
	while (1) {
		if (match(Symbol::ident)) {
			string ident_name = current_token.getName();
			SymbolItem* ident = get(ident_name);
			//ident
			args.push_back(ident);
			next();
		}
		else
			error_handle.errorMessage(19, LineNo);
		if (match(Symbol::comma))
			next();
		else
			break;
	}
	vector<SymbolItem*>::iterator it = args.begin();
	while (it != args.end()) {
		if ((*it) != NULL) {
			middle_code.gen(Opcode::READ, (*it),NULL,NULL);
		}
		it++;
	}
	//[FIXME] enter the symbol set.
	//while (it != args.end()) {

	//}
	expect(Symbol::rparen,")");
#ifdef DEBUG
	level--;
#endif // DEBUG

}

//<写语句> :: = write'('<字符串>, <表达式>')' | write'('<字符串>')' | write'('<表达式>')'
void Parser::writeStatement() {
#ifdef DEBUG
	level++;
	PRINT("write statement");
#endif // DEBUG

	if (match(Symbol::writesym)) {
		next();
	}
	expect(Symbol::lparen,"(");
	// "
	if (match(Symbol::strconst)) {
#ifdef DEBUG
		level++;
		PRINT("str const");
		level--;
#endif // DEBUG
		SymbolItem* temp = symbol_set.genTemp(TokenKind::TEMP_CON,TokenType::stringtyp);
		temp->setString(current_token.getName());
		middle_code.gen(Opcode::WRITE, temp,NULL,NULL);

		next();
		if (match(Symbol::comma))
		{
			next();
			// the write_arg can be const,or var or others.
			SymbolItem* write_arg = expression();
			middle_code.gen(Opcode::WRITE, write_arg,NULL,NULL);
		}
		expect(Symbol::rparen,")");
	}
	else{
		SymbolItem* write_arg = expression();
		middle_code.gen(Opcode::WRITE, write_arg,NULL,NULL);
		expect(Symbol::rparen,")");
	}

#ifdef DEBUG
	level--;
#endif // DEBUG

}



/********************************************/
// Have done for QuaterInstr                //
// factor
// item
// assignment
// forstatement
// ifstatement
// readstatement
// writestatement
// condition
// realparameters
// assignment
/********************************************/