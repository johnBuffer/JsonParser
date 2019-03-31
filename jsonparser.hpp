#pragma once
#include <vector>
#include <map>
#include <string>

namespace jsp
{
	/*
		Base class for all kind of JSON objects
	*/

	struct JsonBase
	{
		JsonBase() = default;
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

		std::string m_data;
	};

	struct JsonObject : public JsonBase
	{
		JsonObject() :
			JsonBase()
		{}

		void add(const std::string& key, const JsonObject& child)
		{
			m_childs[key] = child;
		}

		std::string m_value;
		std::map<std::string, JsonObject> m_childs;
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

	JsonObject parse(const std::string& raw_data, uint32_t start, uint32_t& end)
	{
		JsonObject root;
		uint32_t size(raw_data.size());
		if (!size)
			return root;
		
		for (uint32_t i(start); i < size; ++i)
		{
			const char c(raw_data[i]);
			if (c == '{')
			{
				uint32_t id_start, id_length;
				if (getIdentifier(raw_data, i + 1, id_start, id_length))
				{
					root.add(raw_data.substr(id_start, id_length), parse(raw_data, id_start + id_length, i));
				}
			}
			else if (c == '}')
			{
				end = i + 1;
				break;
			}
		}

		return root;
	}
}