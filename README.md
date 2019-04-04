# JsonParser
Header only json parser in C++

## Installation

Include appropiate header
```cpp
#include "jsonparser.hpp"
```

## Usage

1. Instantiate a JsonObject with a string
```cpp
const std::string data("Imagine I am a json...");
jsp::JsonObject root(data);
```

2. Access json data with []
```cpp
std::cout << root["ok"]["yes"].asString() << std::endl;
```

## Supported operators
- asString()
- asFloat()
- asInt()
- []

If an operator is called on an object which is not supporting it, a __default value__ is returned

## Warning
This lib is not strictly Jsonic

 - For example array of heterogeneous are pressurelessly supported.
 - Coma are optionnal
 - No parsing errors
 - etc...

