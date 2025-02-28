#include <iostream>
#include <string>

#include "Scanner.h"
#include "Parser.h"

int main() {
	using namespace cpp_json_parser;
	std::string json_test = R"(

			{
				"name" : "CwReisen",
				"age" : 20,
				"grade" : [4.7,4.5,4.3],
				"isMale" : true,
				"null" : null
			}	
			
	)";

	Scanner scanner(json_test);
	Parser parser(scanner);

	////sannerµÄ²âÊÔ
	//Scanner::JSONTokenType type;
	//while ((type = scanner.Scan()) != Scanner::JSONTokenType::END_OF_FILE) {
	//	std::cout << "Token Type : " << type;
	//	if (type == Scanner::JSONTokenType::STRING_VALUE)
	//		std::cout << ", Stinrg Value : " << scanner.GetStringValue();
	//	else if (type == Scanner::JSONTokenType::NUMBER_VALUE)
	//		std::cout << ", Number Value : " << scanner.GetNumberValue();
	//	std::cout << "\n";
	//}
	//std::cout << "\n";

	JsonElement* element = parser.Parse();
	std::cout << element->Dumps() << std::endl;

	delete element;
	
	return 0;
}