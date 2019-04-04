#include <iostream>
#include "jsonparser.hpp"

int main()
{
	const std::string data("{lol: 7,\
        k: 5,\
        mam: {\
            j: \"78 {}\"     \
        },\
		lil : { \
		ar : [5, 6, 7],\
		l: 3\
		}\
     }");

	size_t end(0);
	jsp::JsonObject root(jsp::getObject(data, end));

	std::cout << root["mam"]["j"].asString() << std::endl;

	return 0;
}