#pragma once

#include <map>
#include <string>
#include <vector>
#include <sstream>

#include "Error.h"

namespace cpp_json_parser {
	class JsonElement;
	using JsonObject = std::map<std::string, JsonElement* >;
	using JsonArray = std::vector<JsonElement* >;

	class JsonElement {
		friend std::ostream& operator<<(std::ostream& os, JsonObject& obj);
		friend std::ostream& operator<<(std::ostream& os, JsonArray& arr);
	public:
		enum class JsonElementType {
			JSON_OBJECT,
			JSON_ARRAY,
			JSON_STRING,
			JSON_NUMBER,
			JSON_BOOL,
			JSON_NULL
		};
		union Value {
			JsonObject* v_object;
			JsonArray* v_array;
			std::string* v_string;
			double v_number;
			bool v_bool;
		};


		JsonElement();
		JsonElement(JsonObject* v_object);
		JsonElement(JsonArray* v_array);
		JsonElement(std::string* v_string);
		JsonElement(double v_number);
		JsonElement(bool v_bool);

		void SetElement(JsonObject* object);
		void SetElement(JsonArray* array);
		void SetElement(std::string* string);
		void SetElement(double number);
		void SetElement(bool boolean);

		~JsonElement();
	
	public:
		JsonObject* AsObject();
		JsonArray* AsArray();
		std::string* AsString();
		double AsNumber();
		bool AsBool();

		std::string Dumps();
	
	private:
		JsonElementType type;
		Value value;
	};


}