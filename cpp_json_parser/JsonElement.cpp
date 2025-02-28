#include "JsonElement.h"

namespace cpp_json_parser {
	JsonElement::JsonElement()	
		: type(JsonElementType::JSON_NULL) {}

	JsonElement::JsonElement(JsonObject* v_object)
	{
		SetElement(v_object);
	}
	JsonElement::JsonElement(JsonArray* v_array)
	{
		SetElement(v_array);
	}
	JsonElement::JsonElement(std::string* v_string)
	{
		SetElement(v_string);
	}
	JsonElement::JsonElement(double v_number)
	{
		SetElement(v_number);
	}
	JsonElement::JsonElement(bool v_bool)
	{
		SetElement(v_bool);
	}


	void JsonElement::SetElement(JsonObject* object)
	{
		type = JsonElementType::JSON_OBJECT;
		value.v_object = object;
	}
	void JsonElement::SetElement(JsonArray* array)
	{
		type = JsonElementType::JSON_ARRAY;
		value.v_array = array;
	}
	void JsonElement::SetElement(std::string* string)
	{
		type = JsonElementType::JSON_STRING;
		value.v_string = string;
	}
	void JsonElement::SetElement(double number)
	{
		type = JsonElementType::JSON_NUMBER;
		value.v_number = number;
	}
	void JsonElement::SetElement(bool boolean)
	{
		type = JsonElementType::JSON_BOOL;
		value.v_bool = boolean;
	}

	JsonElement::~JsonElement()
	{
		if (type == JsonElementType::JSON_OBJECT) {
			for (auto it = value.v_object->begin(); it != value.v_object->end(); ++it) {
				delete it->second;
			}
			delete value.v_object;
		}
		else if (type == JsonElementType::JSON_ARRAY) {
			for (auto it = value.v_array->begin(); it != value.v_array->end(); ++it) {
				delete* it;
			}
			delete value.v_array;
		}
		else if (type == JsonElementType::JSON_STRING)
			delete value.v_string;
	}


	JsonObject* JsonElement::AsObject()
	{
		if(type == JsonElementType::JSON_OBJECT)
			return value.v_object;
		Error("Type of JsonElement isn't JsonObject");
	}
	JsonArray* JsonElement::AsArray()
	{
		if (type == JsonElementType::JSON_ARRAY)
			return value.v_array;
		Error("Type of JsonElement isn't JsonArray");
	}
	std::string* JsonElement::AsString()
	{
		if (type == JsonElementType::JSON_STRING)
			return value.v_string;
		Error("Type of JsonElement isn't String");
	}
	double JsonElement::AsNumber()
	{
		if (type == JsonElementType::JSON_NUMBER)
			return value.v_number;
		Error("Type of JsonElement isn't Number");
	}
	bool JsonElement::AsBool()
	{
		if (type == JsonElementType::JSON_BOOL)
			return value.v_bool;
		Error("Type of JsonElement isn't Bool");
	}


	std::string JsonElement::Dumps()
	{
		std::stringstream ss;
		switch (type) {
		case JsonElementType::JSON_OBJECT:
			ss << *(value.v_object);
			break;
		case JsonElementType::JSON_ARRAY:
			ss << *(value.v_array);
			break;
		case JsonElementType::JSON_STRING:
			ss << "\"" << *(value.v_string) << "\"";
			break;
		case JsonElementType::JSON_NUMBER:
			ss << value.v_number;
			break;
		case JsonElementType::JSON_BOOL:
			ss << (value.v_bool ? "true" : "false");
			break;
		case JsonElementType::JSON_NULL:
			ss << "null";
			break;
		}
		return ss.str();
	}

	std::ostream& operator<<(std::ostream & os, JsonObject & obj) {
		os << "{";
		for (auto it = obj.begin(); it != obj.end(); it++) {
			os << "\"" << it->first << "\":";
			os << it->second->Dumps();
			// --obj.end()是最后一个元素,不需要加逗号
			// 不用obj.end()-1是因为map的迭代器不支持随机访问
			if (it != --obj.end()) {
				os << ",";
			}
		}
		os << "}";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, JsonArray& arr) {
		os << "[";
		for (size_t i = 0; i < arr.size(); i++) {
			os << arr[i]->Dumps();
			if (i != arr.size()-1) {
				os << ",";
			}
		}
		os << "]";
		return os;
	}
}


