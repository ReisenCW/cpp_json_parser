#pragma once

#include "JsonElement.h"
#include "Scanner.h"


namespace cpp_json_parser {
	class Parser {
	public:
		Parser(Scanner& scanner) : scanner(scanner) {}
		JsonElement* Parse();
	private:
		JsonObject* ParseObject();
		JsonArray* ParseArray();
	private:
		Scanner scanner;
	};
}