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
		JsonBase();
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

		std::string m_data;
	};

	struct JsonObject : public JsonBase
	{
		JsonObject();

		void add(const std::string& key, const JsonObject& child);

		std::string m_value;
		std::map<std::string, JsonObject> m_childs;
	};

	struct JsonArray : public JsonBase
	{
		JsonArray();

		void add(const JsonObject& item);

		std::vector<JsonObject> m_items;
	};
}