#include "parser.h"


int main() {
	Parser* parser_var = new Parser("test/for.txt");
	parser_var->parser();
	parser_var->translate();
	//Parser* parser_const = new Parser("test/const.txt");
	//parser_const->parser();
	//Parser* parser_compound = new Parser("test/compoundstate.txt");
	//parser_compound->parser();
	//Parser* parser_block = new Parser("test/block.txt");
	//parser_block ->parser();
	//Parser* parser_for = new Parser("test/for.txt");
	//parser_for->parser();
	//Parser* parser_if = new Parser("test/if.txt");
	//parser_if->parser();
	//Parser* parser_if = new Parser("test/if_2.txt");
	//parser_if->parser();
	//Parser* parser_write = new Parser("test/write.txt");
	//parser_write->parser();
	//Parser* parser_read = new Parser("test/read.txt");
	//parser_read->parser();
	//Parser* parser_while = new Parser("test/while.txt");
	//parser_while->parser();
	//* parser_func = new Parser("test/function.txt");
	//parser_func->parser();
	//Parser* parser_test= new Parser("test/13061193_test.txt");
	//parser_test->parser(); 
	//Parser* parser_proc = new Parser("test/procedure.txt");
	//parser_proc->parser();
	//Parser* parser_call = new Parser("test/call.txt");
	//parser_call->parser();
	system("pause");
}