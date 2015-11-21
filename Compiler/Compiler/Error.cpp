#include "error.h"

Error::Error() {
	is_success = true;
};

int Error::getErrorCount() {
	return Error::error_count;
};

void Error::errorMessage(int errortype,int line) {
	string Message = "Line " + (line);
	switch (errortype)
	{
	case 1:Message += "";
	default:
		break;
	}
};