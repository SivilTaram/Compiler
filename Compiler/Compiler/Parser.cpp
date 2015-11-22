#include "parser.h"
#include "lexer.h"
#include "error.h"
#define DEBUG
#define LineNo (current_token.getLineNo())
#define PRINT(x) (cout << "Debug::" << x << endl)
//#define DEBUG_NEXT
using namespace std;

void Parser::except(Symbol sym) {
	if (match(sym)) {
		next();
	}
	else {
		//Except something but it doesn't appear.
		Error::errorMessage(9, sym);
		next();
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
bool Parser::next() {
	//Read s symbol.
	current_token = token_lexer.getsym();
	//if symbol is '.' or the eof of the file,then end.
	if (current_token.getType() == Symbol::period || current_token.getType() == Symbol::eofsym)
		return false;
	else
		return true;
#ifdef DEBUG_NEXT
	if (current_token.type==Symbol::charconst)
		cout << "'" << current_token.ident_name << "'";
	else if(current_token.type==Symbol::strconst)
		cout << "\"" << current_token.ident_name << "\"";
	else if (current_token.type == Symbol::number)
		cout << current_token.num_value << " ";
	else if (current_token.type == Symbol::ident)
		cout << current_token.type << " name: " << current_token.ident_name << " ";
	else
		cout << current_token.type << endl;
#endif
};

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
void Parser::block() {
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
			//the loop is outside,and procDec can't have a loop.
			procDec();
			block();
			except(Symbol::semicolon);
		}
		else {
			next();
			funcDec();
			block();
			except(Symbol::semicolon);
		}
	}
	compoundStatement();
}

/*
Start parse.
*/
void Parser::parser() {
	/*
	get one sym
	*/
	next();
	block();
	if (!match(Symbol::period))
		//Number50: the program ended not normally
		Error::errorMessage(50,LineNo);
}



//skip some words until a valid follow set.
void Parser::skip() {};
//test whether the current_token is valid.
void Parser::test() {};

/*
<常量说明部分> :: = const<常量定义>{,<常量定义>};
*/
//Handle const declaration
void Parser::constDec() {
#ifdef DEBUG
	PRINT("const_Dec");
#endif // DEBUG
	if (match(Symbol::ident))
	{
		while (1) {
			// because there is just one way.
			constDef();
			if (match(Symbol::comma))
			{
				next();
			}
			else {
				break;
			}
		}
		// except ";" here.
		except(Symbol::semicolon);
	}
};

void Parser::constDef() {

}

//Handle var declaration
void Parser::variableDec() {};
//Hanle procedure and function declaration

/*
<过程首部>   ::=   procedure<标识符>[<形式参数表>];
<形式参数表> ::= '('<形式参数段>{; <形式参数段>}')'
*/
void Parser::procDec() {
#ifdef DEBUG
	PRINT("proc_Dec");
#endif // DEBUG

	if (match(Symbol::ident))
	{
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
				except(Symbol::semicolon);
			}
		}
		//except a ';'
		except(Symbol::semicolon);
	}
	else {
		//No.11 should define identity at the first of proc.After procedure.
		Error::errorMessage(11, LineNo);
	}
};

//Handle the parameterlist of function or procedure declaration.
//<形式参数段> ::= [var]<标识符>{, <标识符>}: <基本类型>
void Parser::parameterList() {
#ifdef DEBUG
	PRINT("parameter_List");
#endif // DEBUG

	if (match(Symbol::varsym))
	{
		next();
		//[FIXME] there should be a enter table's handle.
		//And the param is [var]!!!
	}
	while (1) {
		except(Symbol::ident);
		if (match(Symbol::colon))
		{
			next();
			break;
		}
		except(Symbol::comma);
	}
	//basic type.
	basicType();
}

//<基本类型>  ::=  integer | char
void Parser::basicType() {
#ifdef DEBUG
	PRINT("basic_Type");
#endif // DEBUG

	if (match(Symbol::integersym) || match(Symbol::charsym))
		next();
	else
	{
		//No.12 except type after ':'
		Error::errorMessage(12,LineNo);
		next();
	}
}

//Handle funcDec
//<函数首部> ::= function <标识符>[<形式参数表>]: <基本类型>;
void Parser::funcDec() {
#ifdef DEBUG
	PRINT("func_Dec");
#endif // DEBUG


	if (match(Symbol::ident))
	{
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
				except(Symbol::semicolon);
			}
		}
		//except : then basictype and then ;
		if(match(Symbol::colon))
			basicType();
		else
		{
			//No.13 There 
			Error::errorMessage(13, LineNo);
			next();
		}
		except(Symbol::semicolon);
	}
	else {
		//No.11 should define identity at the first of proc.After procedure.
		Error::errorMessage(11, LineNo);
	}
};

//Handle statements
//<语句> ::= <赋值语句>|<条件语句>|<当循环语句>|<过程调用语句>|<复合语句>|<读语句>|<写语句>|<for循环语句>|<空>
void Parser::statement() {
#ifdef DEBUG
	PRINT("state_ment");
#endif // DEBUG

	//assignment or proc or func
	if (match(Symbol::ident)) {
		string ident = current_token.getName();
		next();
		//proc or func there should be a paren
		if (match(Symbol::lparen))
			profuncCall(ident);
		//or there should be a assign.
		else
			assignment(ident);
	}
}
//Handle expressions
void Parser::expression() {}
//Handle the index varaiable of array
void Parser::factor() {}
void Parser::term() {}
void Parser::selector() {}
//Handle the call of procedure and function
void Parser::profuncCall(string ident) {}
//Handle the for statement 
void Parser::forStatement() {}
//Handle the while statement
void Parser::whileStatement() {}
//Handle the if statement;
void Parser::ifStatement() {

}

//Handle compound statement
// <复合语句>       ::=  begin<语句>{; <语句>}end
void Parser::compoundStatement() {
#ifdef DEBUG
	PRINT("compound_Statement");
#endif // DEBUG
	except(Symbol::beginsym);
	statement();
	if (match(Symbol::endsym))
	{
		next();
		return;
	}
	while (1) {
		// except ';'
		except(Symbol::semicolon);
		statement();
		if (match(Symbol::endsym)) {
			next();
			return;
		}
		if (match(Symbol::eofsym)) {
			//No.14 Unexcepted end of the file
			Error::errorMessage(14, LineNo);
			return;
		}
	}
}
//Handle the assign
void Parser::assignment(string ident) {

};
