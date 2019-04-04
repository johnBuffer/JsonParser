#include <iostream>
#include "jsonparser.hpp"

int main()
{
	size_t end;

	const std::string data("{ \
		lol: 7,\
        k: 5,\
        mam: {\
            j: 78\
        },\
		lil : { \
		ar : [5, 6, 7],\
		l: 3\
		}\
     }");

	jsp::JsonObject* root = jsp::getObject(data, end);

	root->print("");

	return 0;
}