#pragma once

#include "json_objects.hpp"

namespace jsp
{
	enum JsonToken
	{
		JsonOpenBrace = '{',
		JsonClosedBrace = '}',
		JsonOpenBraket = '[',
		JsonClosedBracket = ']',
		JsonComa = ',',
		JsonColumn = ':',
		JsonNewLine = '\n',
		JsonSpace = ' ',
		JsonTab = '\t'
	};

	std::string extract(const std::string& str, size_t start, size_t end)
	{
		return str.substr(start, end - start);
	}

	JsonBase* getChild(const std::string& raw_data, size_t& index);

	void skipSpace(const std::string& raw_data, size_t& index)
	{
		const size_t size(raw_data.size());
		for (;index < size; ++index)
		{
			const char c(raw_data[index]);
			if (c != JsonToken::JsonSpace && c != JsonToken::JsonTab) {
				return;
			}
		}
	}

	bool getIdentifier(const std::string& raw_data, size_t& index, size_t& id_start)
	{
		id_start = 0;
		size_t size(raw_data.size());
		for (skipSpace(raw_data, index); index < size; ++index)
		{
			const char c(raw_data[index]);
			if (!id_start)
			{
				if (c == JsonToken::JsonClosedBrace) {
					return false;
				} else if (c == JsonToken::JsonColumn) {
					return false;
				} else if (c != JsonToken::JsonNewLine) {
					id_start = index;
				}
			} else if (c == JsonToken::JsonColumn) {
				return true;
			}
		}

		return false;
	}

	JsonData* getValue(const std::string& raw_data, size_t& index)
	{
		const size_t size(raw_data.size());
		size_t val_start(0);
		for (; index < size; ++index)
		{
			const char c(raw_data[index]);
			if (c == JsonToken::JsonClosedBrace || 
				c == JsonToken::JsonClosedBracket || 
				c == JsonToken::JsonComa)
			{
				std::cout << "getValue " << extract(raw_data, val_start, index) << std::endl;
				JsonData* data = new JsonData(extract(raw_data, val_start, index));
				return data;
			}
			else if (c != JsonToken::JsonSpace && !val_start) {
				val_start = index;
			}
		}
		return new JsonData();
	}

	JsonObject* getObject(const std::string& raw_data, size_t& index)
	{
		JsonObject* object(new JsonObject());
		size_t size(raw_data.size());
		if (!size) {
			return object;
		}

		while (index < size)
		{
			size_t id_start;
			if (raw_data[index] == JsonToken::JsonClosedBrace) {
				++index;
				break;
			} else if (getIdentifier(raw_data, ++index, id_start)) {
				const std::string identifier(extract(raw_data, id_start, index++));
				object->add(identifier, getChild(raw_data, index));
			} else {
				++index;
			}
		}

		return object;
	}

	JsonArray* getArray(const std::string& raw_data, size_t& index)
	{
		JsonArray* j_array(new JsonArray());
		const size_t size(raw_data.size());

		while (index < size)
		{
			const char c(raw_data[index]);
			std::cout << "Array on " << c << std::endl;
			if (c == JsonToken::JsonClosedBracket) {
				++index;
				break;
			} else if (c == JsonToken::JsonComa || c == JsonSpace) {
				++index;
			} else if (c == JsonToken::JsonOpenBrace) {
				j_array->add(getObject(raw_data, index));
			} else {
				j_array->add(getValue(raw_data, index));
			}
		}

		return j_array;
	}

	JsonBase* getChild(const std::string& raw_data, size_t& index)
	{
		//std::cout << "getChild " << raw_data.substr(index) << std::endl;
		size_t size(raw_data.size());
		for (skipSpace(raw_data, index); index < size; ++index)
		{
			const char c(raw_data[index]);
			if (c == JsonToken::JsonOpenBrace) {
				return getObject(raw_data, index);
			} else if (c == JsonToken::JsonOpenBraket) {
				return getArray(raw_data, ++index);
			} else {
				return getValue(raw_data, index);
			}
		}
		return new JsonData();
	}
}