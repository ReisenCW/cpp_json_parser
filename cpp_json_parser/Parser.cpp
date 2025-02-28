#include "Parser.h"


namespace cpp_json_parser {
	using JSONTokenType = Scanner::JSONTokenType;
	using Type = JsonElement::JsonElementType;

	JsonElement* Parser::Parse()
	{
		JsonElement* element = new JsonElement();
		//扫描下一个类别
		JSONTokenType token = scanner.Scan();
		//根据类别给element赋类型和值
		switch (token) {
		case JSONTokenType::END_OF_FILE:
			break;
		case JSONTokenType::BEGIN_OBJECT: {
			JsonObject* object = ParseObject();
			element->SetElement(object);
			break;
		}
		case JSONTokenType::BEGIN_ARRAY: {
			JsonArray* array = ParseArray();
			element->SetElement(array);
			break;
		}
		case JSONTokenType::STRING_VALUE: {
			std::string* str = new std::string(scanner.GetStringValue());
			element->SetElement(str);
			break;
		}
		case JSONTokenType::NUMBER_VALUE: {
			double number = scanner.GetNumberValue();
			element->SetElement(number);
			break;
		}
		case JSONTokenType::LITERAL_TRUE: {
			element->SetElement(true);
			break;
		}
		case JSONTokenType::LITERAL_FALSE: {
			element->SetElement(false);
			break;
		}
		case JSONTokenType::NULL_VALUE:
			break;
		}
		return element;
	}

	JsonObject* Parser::ParseObject()
	{
		JsonObject* res_object = new JsonObject();
		JSONTokenType next_type = scanner.Scan();
		//判断是否是空对象
		if (next_type == JSONTokenType::END_OBJECT)
			return res_object;
		scanner.RollBack();
		while (1) {
			//判断key是否是字符串
			next_type = scanner.Scan();
			if (next_type != JSONTokenType::STRING_VALUE) {
				Error("key must be string! ");
			}
			std::string key = scanner.GetStringValue();

			//判断是否有:
			next_type = scanner.Scan();
			if (next_type != JSONTokenType::NAME_SEPARATOR) {
				Error("Expected ':' !");
			}

			//解析value
			(*res_object)[key] = Parse();

			//判断对象是否结束或是有,
			next_type = scanner.Scan();
			if (next_type == JSONTokenType::END_OBJECT) {
				break;
			}
			if (next_type != JSONTokenType::VALUE_SEPARATOR) {
				Error("Expected ',' !");
			}
		}
		return res_object;
	}

	JsonArray* Parser::ParseArray()
	{
		JsonArray* res_array = new JsonArray();
		JSONTokenType next_type = scanner.Scan();
		//判断是否是空数组
		if (next_type == JSONTokenType::END_ARRAY)
			return res_array;
		scanner.RollBack();
		while (1) {
			//解析value
			res_array->push_back(Parse());
			//判断数组是否结束或是有,
			next_type = scanner.Scan();
			if (next_type == JSONTokenType::END_ARRAY) {
				break;
			}
			if (next_type != JSONTokenType::VALUE_SEPARATOR) {
				Error("Expected ',' !");
			}
		}
		return res_array;
	}
}


