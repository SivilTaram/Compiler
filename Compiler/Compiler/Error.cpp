#include "error.h"

Error::Error() {
	is_success = true;
};

int Error::getErrorCount() {
	return Error::error_count;
};

void Error::errorMessage(int errortype,int line,char invalid=' ') {
	string Message = "Line " + (line);
	switch (errortype)
	{
	case 1:Message += "";
	case 6:Message += "Invalid char"+invalid;
	default:
		break;
	}
};