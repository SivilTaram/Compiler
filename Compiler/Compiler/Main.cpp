#include "parser.h"


int main() {
	//Parser* parser_var = new Parser("test/var.txt");
	//parser_var->parser();
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
	Parser* parser_write = new Parser("test/write.txt");
	parser_write->parser();
	system("pause");
}