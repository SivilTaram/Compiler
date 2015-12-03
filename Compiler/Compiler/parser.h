#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "symbolset.h"
#include "lexer.h"
#include "rootsymbolset.h"
#include "middlecode.h"
#include <queue>
#include <set>

#define DEBUG

typedef set<Symbol> symset;

class  Parser {
private:
	Token current_token;
	Lexer token_lexer;
	RootSymbolSet symbol_set;
	MiddleCode middle_code;

public:
	Parser(string file_path) :token_lexer(file_path){};
	bool match(Symbol sym);
	void next();
	void parser();
	//If the next symbol is equal to sym,then next;
	//Else skip some words.
	void expect(Symbol sym);

	SymbolItem* get(string _ident_name);
	//skip some words until a valid follow set.
	void skip(symset s1,int error_code);
	//test whether the current_token is valid.
	void test(symset s1, symset s2, int error_code);

	void block();

	//Handle const declaration
	void constDec();
	//Handle const define
	void constDef();
	//Handle var declaration
	void variableDec();
	void variableDef();
	//Fill the symbol set with the type of var.
	void varType(queue<string>* var_name);
	//Hanle procedure declaration
	void procDec();
	//Handle the list of params of procdure or function.
	void parameterList();
	//Handle the basic type.
	void basicType(queue<string>* args_name,TokenKind kind);
	//Hanld function delaration
	void funcDec();
	//Handle statements
	void statement();
	void selector(queue<string>* var_name);
	//Handle the call of procedure and function
	//void profuncCall(string name);
	//Handle the real parameters
	//Handle the for statement 
	void forStatement();
	//Handle the while statement
	void whileStatement();
	//Handle the if statement;
	void ifStatement();
	void condition(SymbolItem* label);
	//Handle compound statement
	void compoundStatement();
	//Handle the assign
	void assignment(SymbolItem* ident);
	//Item
	void callPro(SymbolItem* proc);

	//the func should have a return value.
	SymbolItem* callFunc(SymbolItem* func);
	SymbolItem* realParameter(SymbolItem* func);

	/* They should have return value.*/
	//Item
	SymbolItem* item();
	//Handle expressions
	SymbolItem* expression();
	//Handle the index varaiable of array
	SymbolItem* factor();


	//read and write statement.
	void readStatement();
	void writeStatement();
};

class eofexception : public exception {
};


/*
<程序>          ::=   <分程序>.
<分程序>        ::=   [<常量说明部分>][<变量说明部分>]{[<过程说明部分>]| [<函数说明部分>]}<复合语句>
<常量说明部分>  ::=   const<常量定义>{,<常量定义>};
<常量定义>      ::=    <标识符>＝ <常量>
<常量>          ::=    [+| -] <无符号整数>|<字符>
<字符>          ::=  '<字母>' | '<数字>'
<字符串>        ::= "{十进制编码为32,33,35-126的ASCII字符}"
<无符号整数>    ::=   <数字>{<数字>}
<标识符>        ::=   <字母>{<字母>|<数字>}
<变量说明部分>  ::=   var <变量说明> ; {<变量说明>;}
<变量说明>      ::=   <标识符>{, <标识符>} : <类型>
<类型>          ::=   <基本类型>|array'['<无符号整数>']' of <基本类型>
<基本类型>      ::=   integer | char
<过程说明部分>  ::=  <过程首部><分程序>{; <过程首部><分程序>};
<函数说明部分>  ::=  <函数首部><分程序>{; <函数首部><分程序>};
<过程首部>      ::=   procedure<标识符>[<形式参数表>];
<函数首部>      ::=   function <标识符>[<形式参数表>]: <基本类型>;
<形式参数表>    ::= '('<形式参数段>{; <形式参数段>}')'
<形式参数段>    ::=   [var]<标识符>{, <标识符>}: <基本类型>
<语句>          ::=   <赋值语句>|<条件语句>|<当循环语句>|<过程调用语句>|<复合语句>|<读语句>|<写语句>|<for循环语句>|<空>
<赋值语句>      ::=  <标识符> := <表达式>| <函数标识符> := <表达式> | <标识符>'['<表达式>']':= <表达式>
<函数标识符>    ::=  <标识符>
<表达式>        ::=   [+|-]<项>{<加法运算符><项>}
<项>            ::=   <因子>{<乘法运算符><因子>}
<因子>          ::=   <标识符>|<标识符>'['<表达式>']'|<无符号整数>| '('<表达式>')' | <函数调用语句>
<函数调用语句>  ::=   <标识符>[<实在参数表>]
<实在参数表>    ::= '(' <实在参数> {, <实在参数>}')'
<实在参数>      ::=   <表达式>
<加法运算符>    ::=   +|-
<乘法运算符>    ::=   *|/
<条件>          ::=   <表达式><关系运算符><表达式>
<关系运算符>    ::=   <|<=|>|>= |=|<>
<条件语句>      ::=   if<条件>then<语句> | if<条件>then<语句>else<语句>
<当循环语句>    ::=   do<语句> while<条件>
<for循环语句>   ::=   for <标识符>  := <表达式> （downto | to） <表达式> do <语句> //步长为1
<过程调用语句>  ::=  <标识符>[<实在参数表>]
<复合语句>      ::=   begin<语句>{; <语句>}end
<读语句>        ::=   read'('<标识符>{,<标识符>}')'
<写语句>        ::=   write'('<字符串>,<表达式>')'|write'('<字符串>')'|write'('<表达式>')'
<字母>          ::=   a|b|c|d…x|y|z |A|B…|Z
<数字>          ::=   0|1|2|3…8|9
*/

#endif