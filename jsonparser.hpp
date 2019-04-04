#pragma once

#include "json_objects.hpp"

namespace jsp
{
	enum JsonToken
	{
		JsonTab           = '\t',
		JsonComa          = ',',
		JsonSpace         = ' ',
		JsonQuote         = '"',
		JsonColumn        = ':',
		JsonNewLine       = '\n',
		JsonOpenBrace     = '{',
		JsonOpenBraket    = '[',
		JsonClosedBrace   = '}',
		JsonClosedBracket = ']'
	};

	const std::string extract(const std::string& str, size_t start, size_t end)
	{
		return str.substr(start, end - start);
	}

	const std::string getQuotedValue(const std::string& str, size_t& index)
	{
		const size_t size(str.size());
		const size_t start(index);
		for (; index < size; ++index) {
			if (str[index] == JsonToken::JsonQuote) {
				return extract(str, start, index);
			}
		}

		return std::string();
	}

	void skipSpace(const std::string& str, size_t& index)
	{
		const size_t size(str.size());
		for (;index < size; ++index)
		{
			const char c(str[index]);
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

	JsonBase* getChild(const std::string& raw_data, size_t& index);
	
	JsonData* getValue(const std::string& raw_data, size_t& index)
	{
		const size_t size(raw_data.size());
		size_t val_start(0);
		for (; index < size; ++index)
		{
			const char c(raw_data[index]);
			if (c == JsonToken::JsonClosedBrace || c == JsonToken::JsonClosedBracket || c == JsonToken::JsonComa)
			{
				return new JsonData(extract(raw_data, val_start, index));
			}
			else if (c == JsonToken::JsonQuote)
			{
				return new JsonData(getQuotedValue(raw_data, ++index));
			}
			else if (c != JsonToken::JsonSpace && !val_start) {
				val_start = index;
			}
		}
		return new JsonData();
	}

	JsonDict* getObject(const std::string& raw_data, size_t& index)
	{
		JsonDict* object(new JsonDict());
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