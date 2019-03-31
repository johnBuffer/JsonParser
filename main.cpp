#include <iostream>
#include "jsonparser.hpp"

int main()
{
	uint32_t end;
	jsp::JsonObject root = jsp::parse("{}", 0, end);
	return 0;
}