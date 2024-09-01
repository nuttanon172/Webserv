#include "Request.hpp"

void Request::writeStream(char *str, int size)
{
	if (size < 0)
		return ;
	inputStream.write(str, size);
}
