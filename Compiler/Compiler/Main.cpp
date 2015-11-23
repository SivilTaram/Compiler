#include "parser.h"


int main() {
	Parser* parser_var = new Parser("test/var.txt");
	parser_var->parser();
	//Parser* parser_const = new Parser("test/const.txt");
	//parser_const->parser();
	system("pause");
}