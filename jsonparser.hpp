#pragma once

#include "json_objects.hpp"

namespace jsp
{
	enum JsonToken
	{
		JsonOpenBracket = '{',
		JsonClosedBracket = '}',
		JsonComa = ',',
		JsonTwoPoints = ':',
		JsonNewLine = '\n',
		JsonSpace = ' ',
		JsonTab = '\t'
	};

	std::string extract(const std::string& str, size_t start, size_t end)
	{
		size_t length(end - start);
		return str.substr(start, length);
	}

	JsonBase* getChild(const std::string& raw_data, size_t start, size_t& end);

	size_t skipSpace(const std::string& raw_data, size_t start)
	{
		const size_t size(raw_data.size());
		for (size_t i(start); i < size; ++i)
		{
			const char c(raw_data[i]);
			if (c != JsonToken::JsonSpace && c != JsonToken::JsonTab)
			{
				return i;
			}
		}

		return size;
	}

	bool getIdentifier(const std::string& raw_data, size_t start, size_t& id_start, size_t& id_length)
	{
		id_start = 0;
		size_t size(raw_data.size());
		size_t i(skipSpace(raw_data, start));

		for (i; i < size; ++i)
		{
			const char c(raw_data[i]);
			if (!id_start)
			{
				if (c == JsonToken::JsonClosedBracket)
				{
					return false;
				}
				else if (c == JsonToken::JsonTwoPoints)
				{
					return false;
				}
				else if (c != JsonToken::JsonNewLine)
				{
					id_start = i;
				}
			}
			else if (c == JsonToken::JsonTwoPoints)
			{
				id_length = i - id_start;
				return true;
			}
		}

		return false;
	}

	// Get next object
	JsonObject* getObject(const std::string& raw_data, size_t start, size_t& end)
	{
		JsonObject* object = new JsonObject();

		size_t size(raw_data.size());
		if (size == 0)
			return object;

		size_t i(start);
		while (i < size)
		{
			size_t id_start, id_length;
			if (raw_data[i] == JsonToken::JsonClosedBracket)
			{
				break;
			}
			else if (getIdentifier(raw_data, i + 1, id_start, id_length))
			{
				const std::string identifier = raw_data.substr(id_start, id_length);
				i = id_start + id_length + 1;
				object->add(identifier, getChild(raw_data, i, i));
			} 
			else
			{
				++i;
			}
		}

		end = i + 1;
		return object;
	}

	JsonData* getValue(const std::string& raw_data, size_t start, size_t& end)
	{
		const size_t size(raw_data.size());

		size_t val_start(0);
		for (size_t i(start); i < size; ++i)
		{
			const char c(raw_data[i]);
			if (c == JsonToken::JsonClosedBracket || c == JsonToken::JsonComa)
			{
				end = i;
				JsonData* data = new JsonData(extract(raw_data, val_start, i));
				return data;
			}
			else if (c != JsonToken::JsonSpace && !val_start)
			{
				val_start = i;
			}
		}

		end = size;
		return new JsonData();
	}

	JsonBase* getChild(const std::string& raw_data, size_t start, size_t& end)
	{
		size_t size(raw_data.size());

		for (size_t j(skipSpace(raw_data, start)); j < size; ++j)
		{
			end = j;
			const char c(raw_data[j]);
			if (c == JsonToken::JsonOpenBracket)
			{
				return getObject(raw_data, j, end);
			}
			else
			{
				return getValue(raw_data, j, end);
			}
		}

		return new JsonData();
	}
}