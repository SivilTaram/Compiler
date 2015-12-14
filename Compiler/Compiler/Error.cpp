#include "error.h"
#include <sstream>

Error::Error() {
	is_success = true;
};

int Error::getErrorCount() {
	return error_count;
};

void Error::errorMessage(int errortype,int line) {
	stringstream Message;
	Message << "Error in " << line << ":";
	error_count++;
	switch (errortype)
	{
	case 1:Message << " " << endl;
	default:
		break;
	}
	error_messages.push_back(Message.str());
}