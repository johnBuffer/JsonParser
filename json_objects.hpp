#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

namespace jsp
{
/*
	Base class for all kind of JSON objects
*/

enum ObjectType
{
	JSON_NONE,
	JSON_DATA,
	JSON_ARRAY,
	JSON_OBJECT
};

struct JsonBase
{
	JsonBase() = default;

	virtual void print(const std::string& indent) const = 0;
};

struct JsonData : public JsonBase
{
	JsonData(const std::string& str = "") :
		m_data(str)
	{}

	int32_t asInt() const
	{
		int32_t r(0);
		try
		{
			r = std::stoi(m_data);
		}
		catch (std::invalid_argument&) {}
		catch (std::out_of_range&) {}

		return r;
	}

	float asFloat() const
	{
		float r(0.0f);
		try
		{
			r = std::stof(m_data);
		}
		catch (std::invalid_argument&) {}
		catch (std::out_of_range&) {}

		return r;
	}

	const std::string& asString() const
	{
		return m_data;
	}

	void print(const std::string&) const
	{
		std::cout << m_data << std::endl;
	}

	std::string m_data;
};

struct JsonObject : public JsonBase
{
	JsonObject() :
		JsonBase()
	{}

	void add(const std::string& key, JsonBase* child)
	{
		m_childs[key] = child;
	}

	void print(const std::string& indent) const
	{
		const std::string next_indent(indent + "    ");
		std::cout << "{" << std::endl;
		for (auto jsb : m_childs)
		{
			std::cout << next_indent << jsb.first << ": ";
			jsb.second->print(next_indent);
		}
		std::cout << indent << "}" << std::endl;
	}

	std::string m_value;
	std::unordered_map<std::string, JsonBase*> m_childs;
};

struct JsonArray : public JsonBase
{
	JsonArray();

	void add(const JsonObject& item);

	std::vector<JsonObject> m_items;
};

} // Namespace's end

