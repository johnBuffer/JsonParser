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

	// Print the json object on the standard output
	virtual void print(const std::string& indent) const = 0;
	
	// Objects operators and their default fallbacks
	virtual JsonBase* operator[](const std::string& name) { return nullptr; }
	virtual float     asFloat() const { return 0.0f; }
	virtual int       asInt() const { return 0; }
	virtual const     std::string& asString() const { return std::string(); }
};

struct JsonData : public JsonBase
{
	JsonData(const std::string& str = "") :
		m_data(str)
	{}

	int32_t asInt() const
	{
		int32_t r(0);
		try {
			r = std::stoi(m_data);
		} catch (std::invalid_argument&) {
		} catch (std::out_of_range&) {
		}

		return r;
	}

	float asFloat() const
	{
		float r(0.0f);
		try {
			r = std::stof(m_data);
		} catch (std::invalid_argument&) {
		} catch (std::out_of_range&) {
		}

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

struct JsonDict : public JsonBase
{
	JsonDict() :
		JsonBase()
	{}

	void add(const std::string& key, JsonBase* child)
	{
		m_childs[key] = child;
	}

	JsonBase* operator[](const std::string& name)
	{
		auto it(m_childs.find(name));
		if (it != m_childs.end()) {
			return it->second;
		}
		return nullptr;
	}

	void print(const std::string& indent) const
	{
		const std::string next_indent(indent + "    ");
		std::cout << "{" << std::endl;
		for (auto jsb : m_childs) {
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
	JsonArray() :
		JsonBase()
	{}

	void add(JsonBase* item)
	{
		m_items.push_back(item);
	}

	void print(const std::string& indent) const
	{
		const std::string next_indent(indent + "    ");
		std::cout << "[" << std::endl;
		for (JsonBase* jsb : m_items) {
			jsb->print(next_indent);
		}
		std::cout << indent << "]" << std::endl;
	}

	std::vector<JsonBase*> m_items;
};


class JsonObject
{
public:
	JsonObject() :
		m_object(new JsonDict())
	{}

	JsonObject(JsonBase* object) :
		m_object(object)
	{}

	JsonObject operator[](const std::string& name)
	{
		return (*m_object)[name];
	}

	float asFloat() const 
	{
		return m_object->asFloat();
	}

	int asInt() const
	{ 
		return m_object->asInt();
	}

	const std::string& asString() const 
	{ 
		return m_object->asString(); 
	}

private:
	JsonBase* m_object;
};

} // Namespace's end

