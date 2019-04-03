#pragma once
#include <vector>
#include <map>
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
		JsonData(const std::string& str = ""):
			m_data(str)
		{}

		int32_t asInt() const
		{
			int32_t r(0);
			try
			{
				r = std::stoi(m_data);
			}
			catch (std::invalid_argument&)
			{
				// No an int
			}
			catch (std::out_of_range&)
			{
				// Too big
			}

			return r;
		}

		float asFloat() const
		{
			float r(0.0f);
			try
			{
				r = std::stof(m_data);
			}
			catch (std::invalid_argument&)
			{
				// No a float
			}
			catch (std::out_of_range&)
			{
				// Too big
			}

			return r;
		}

		const std::string& asString() const
		{
			return m_data;
		}

		void print(const std::string& indent) const
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
		std::map<std::string, JsonBase*> m_childs;
	};

	struct JsonArray : public JsonBase
	{
		JsonArray();

		void add(const JsonObject& item);

		std::vector<JsonObject> m_items;
	};

	bool getIdentifier(const std::string& raw_data, uint32_t start, uint32_t& id_start, uint32_t& id_length)
	{
		bool found(false);
		uint32_t size(raw_data.size());
		for (int i(start); i < size; ++i)
		{
			const char c(raw_data[i]);
			if (!found)
			{
				if (c == '}')
				{
					return false;
				}
				else if (c != ':' && c != '\n')
				{
					found = true;
					id_start = i;
				}
				else if(!found && c == ':')
				{
					return false;
				}
			}
			else if (found && c == ':')
			{
				id_length = i - id_start;
				return true;
			}
		}

		return false;
	}

	// Get next object
	JsonObject* getObject(const std::string& raw_data, uint32_t start, uint32_t& end)
	{
		JsonObject* object = new JsonObject();

		uint32_t size(raw_data.size());
		if (size == 0)
			return object;

		uint32_t i(start);
		while (i < size)
		{
			uint32_t id_start, id_length;
			if (getIdentifier(raw_data, i + 1, id_start, id_length))
			{
				const std::string identifier = raw_data.substr(id_start, id_length);
				std::cout << "Found identtifier " << identifier << std::endl;
				i = id_start + id_length + 1;

				uint32_t val_start(0);
				for (uint32_t j(i); j < size; ++j)
				{
					const char c(raw_data[j]);
					if (c == '{')
					{
						object->add(identifier, getObject(raw_data, j, i));
						break;
					}
					else if (c == '}')
					{
						uint32_t data_length(j - val_start);
						const std::string value(raw_data.substr(val_start, data_length));
						std::cout << "Add data child with value:" << value << std::endl;
						JsonData* data = new JsonData(value);
						object->add(identifier, data);

						return object;
					}
					else if (c == ',')
					{
						uint32_t data_length(j - val_start);
						JsonData* data = new JsonData(raw_data.substr(val_start, data_length));
						object->add(identifier, data);
						break;
					}
					else if (c != ' ' && !val_start)
					{
						std::cout << "Found value " << c << " at " << j << std::endl;
						val_start = j;
					}
				}
			}
			else
			{
				break;
			}
		}

		return object;
	}
}