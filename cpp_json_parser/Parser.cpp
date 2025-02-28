#include "Parser.h"


namespace cpp_json_parser {
	using JSONTokenType = Scanner::JSONTokenType;
	using Type = JsonElement::JsonElementType;

	JsonElement* Parser::Parse()
	{
		JsonElement* element = new JsonElement();
		//ɨ����һ�����
		JSONTokenType token = scanner.Scan();
		//��������element�����ͺ�ֵ
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
		//�ж��Ƿ��ǿն���
		if (next_type == JSONTokenType::END_OBJECT)
			return res_object;
		scanner.RollBack();
		while (1) {
			//�ж�key�Ƿ����ַ���
			next_type = scanner.Scan();
			if (next_type != JSONTokenType::STRING_VALUE) {
				Error("key must be string! ");
			}
			std::string key = scanner.GetStringValue();

			//�ж��Ƿ���:
			next_type = scanner.Scan();
			if (next_type != JSONTokenType::NAME_SEPARATOR) {
				Error("Expected ':' !");
			}

			//����value
			(*res_object)[key] = Parse();

			//�ж϶����Ƿ����������,
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
		//�ж��Ƿ��ǿ�����
		if (next_type == JSONTokenType::END_ARRAY)
			return res_array;
		scanner.RollBack();
		while (1) {
			//����value
			res_array->push_back(Parse());
			//�ж������Ƿ����������,
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


