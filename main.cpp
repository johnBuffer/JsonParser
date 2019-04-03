#include <iostream>
#include "jsonparser.hpp"

int main()
{
	uint32_t end;
	jsp::JsonObject* root = jsp::getObject("{lol:7,k:5}", 0, end);
	
	root->print("");

	return 0;
}