#include "Error.h"

void Error(const char* msg)
{
	throw std::logic_error(msg);
}
