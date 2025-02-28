#pragma once

#include <string>
#include <iostream>
#include "Error.h"

namespace cpp_json_parser {
	class Scanner {
	public:
		Scanner(std::string& src) 
			: source(src), current_pos(0), previous_pos(0) {}
		enum class JSONTokenType {
			BEGIN_OBJECT,	// {
			END_OBJECT,		// }

			BEGIN_ARRAY,	// [
			END_ARRAY,		// ]

			STRING_VALUE,
			NUMBER_VALUE,
			NULL_VALUE,

			LITERAL_TRUE,
			LITERAL_FALSE,

			VALUE_SEPARATOR,	// ,
			NAME_SEPARATOR,		// :

			END_OF_FILE
		};

		friend std::ostream& operator<<(std::ostream& os, JSONTokenType type);

		JSONTokenType Scan();
		void RollBack();

		double GetNumberValue() { return number_value; }
		std::string GetStringValue() { return string_value; }
	
	private:
		bool IsEnd();
		char Advance();
		void ScanNull();
		void ScanTrue();
		void ScanFalse();
		void ScanNumber();
		void ScanString();

		char Peek();
		char PeekNext();
		bool IsDigit(char c);
	private:
		std::string source;

		size_t current_pos;
		size_t previous_pos;

		double number_value;
		std::string string_value;
	};
}