#include "error.h"
#include <sstream>

Error::Error() {
	is_success = true;
};

int Error::getErrorCount() {
	return error_count;
};

bool Error::IsSuccess() {
	return is_success;
}

void Error::errorMessage(int errortype,int line) {
	stringstream Message;
	Message << "Error in " << line << ":";
	error_count++;
	switch (errortype)
	{
		//the function have defined before.
	case 41: Message << "常量定义缺少标识符" << endl; break;
	case 50: Message << "程序没有以.结尾" << endl; break;
	case 42: Message << "把 = 当 := 使用" << endl; break;
	case 60: Message << "分程序没有正常结束" << endl; break;
	case 61: Message << "常量定义格式不正确" << endl; break;
	default:
		break;
	}
	error_messages.push_back(Message.str());
}

void Error::errorMessage(int errortype, int line,string message1) {
	stringstream Message;
	Message << "错误在第" << line << "行:";
	error_count++;
	switch (errortype)
	{
		//the function have defined before.
	case 13:
	case 16: Message << message1 << "不是一个合法的常量类型！" << endl; break;
	case 9: Message << "缺少字符" << message1 << endl; break;
	case 19: Message << message1 << "不应该在此处出现！" << endl; break;
	case 40: Message << "标识符" << message1 << "未定义！" << endl; break;
	case 42: Message << "常量" << message1 << "已经定义过了!" << endl; break;
	case 44: Message << "函数" << message1 << "重复定义！" << endl; break;
	case 45: Message << "函数的返回类型" << message1 << "不是基本类型！" << endl; break;

	default:
		break;
	}
	error_messages.push_back(Message.str());
}

void Error::print() {
	vector<string>::iterator it = error_messages.begin();
	while (it != error_messages.end()) {
		cout << (*it);
		it++;
	}
}