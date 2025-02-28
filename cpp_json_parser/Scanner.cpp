#include "Scanner.h"


namespace cpp_json_parser {
	Scanner::JSONTokenType Scanner::Scan()
	{
		//判断是否扫描到文件末尾
		if (IsEnd())
			return JSONTokenType::END_OF_FILE;
		//更新previous_pos
		previous_pos = current_pos;
		//获取当前字符,并更新current_pos
		char c = Advance();
		//根据当前字符判断JSONTokenType类别
		switch (c) {
		case '{':
			return JSONTokenType::BEGIN_OBJECT;
		case '}':
			return JSONTokenType::END_OBJECT;
		case '[':
			return JSONTokenType::BEGIN_ARRAY;
		case ']':
			return JSONTokenType::END_ARRAY;
		case ',':
			return JSONTokenType::VALUE_SEPARATOR;
		case ':':
			return JSONTokenType::NAME_SEPARATOR;
		case 'n':
			ScanNull();
			return JSONTokenType::NULL_VALUE;
		case 't':
			ScanTrue();
			return JSONTokenType::LITERAL_TRUE;
		case 'f':
			ScanFalse();
			return JSONTokenType::LITERAL_FALSE;
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			return Scan();
		case '\"':
			ScanString();
			return JSONTokenType::STRING_VALUE;
		case '-': 
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			ScanNumber();
			return JSONTokenType::NUMBER_VALUE;
		default:
			Error("Unexpected token : " + c);
			return JSONTokenType();
		}
	}

	bool Scanner::IsEnd()
	{
		return current_pos >= source.size();
	}

	char Scanner::Advance()
	{
		if (IsEnd()) return '\0';
		return source[current_pos++];
	}

	void Scanner::ScanNull()
	{
		// null 检测到n时,进入ScanNull,current_pos指向u
		if (source.compare(current_pos, 3, "ull") == 0)
			current_pos += 3;
		else
			Error("Match null Erorr");
	}

	void Scanner::ScanTrue()
	{
		if (source.compare(current_pos, 3, "rue") == 0)
			current_pos += 3;
		else
			Error("Match true Erorr");
	}

	void Scanner::ScanFalse()
	{
		if (source.compare(current_pos, 4, "alse") == 0)
			current_pos += 4;
		else
			Error("Match false Erorr");
	}

	void Scanner::ScanNumber()
	{
		// 123.456 检测到1时进入ScanNumber,此时current_pos指向2
		// -123.456 检测到-时进入ScanNumber,此时current_pos指向1
		// 未处理科学计数法如1e3
		size_t begin_pos = current_pos - 1;
		while (IsDigit(Peek())) {
			Advance();
		}
		if (Peek() == '.' && IsDigit(PeekNext())) {
			Advance();
			while (IsDigit(Peek())) {
				Advance();
			}
		}
		number_value = std::atof(source.substr(begin_pos, current_pos - begin_pos).c_str());
	}

	void Scanner::ScanString()
	{
		// "string_A" 检测到第一个"时进入ScanString
		// 此时current_pos指向第一个字符
		// 未处理转义字符
		size_t begin_pos = current_pos;
		while (Peek() != '\"' && !IsEnd()) {
			Advance();
		}
		if (IsEnd()) {
			Error("Invalid String Value : missing closing quote");
		}
		else {
			Advance(); //跳过右"
		}
		string_value = source.substr(begin_pos, current_pos - begin_pos - 1);
	}

	char Scanner::Peek()
	{
		//返回当前字符
		if (IsEnd()) return '\0';
		return source[current_pos];
	}

	char Scanner::PeekNext()
	{
		//返回下一个字符
		if (current_pos + 1 >= source.size()) return '\0';
		return source[current_pos + 1];

	}

	void Scanner::RollBack()
	{
		//回退到上一个字符
		current_pos = previous_pos;
	}

	bool Scanner::IsDigit(char c)
	{
		//判断是否是数字
		if (c >= '0' && c <= '9') return true;
		return false;
	}
	std::ostream& operator<<(std::ostream& os, Scanner::JSONTokenType type)
	{
		//重载<<运算符,用于输出JSONTokenType
		std::string prompt = "UNKNOWN";
		switch (type) {
		case Scanner::JSONTokenType::BEGIN_OBJECT:
			prompt = "BEGIN_OBJECT";
			break;
		case Scanner::JSONTokenType::END_OBJECT:
			prompt = "END_OBJECT";
			break;
		case Scanner::JSONTokenType::BEGIN_ARRAY:
			prompt = "BEGIN_ARRAY";
			break;
		case Scanner::JSONTokenType::END_ARRAY:
			prompt = "END_ARRAY";
			break;
		case Scanner::JSONTokenType::STRING_VALUE:
			prompt = "STRING_VALUE";
			break;
		case Scanner::JSONTokenType::NUMBER_VALUE:
			prompt = "NUMBER_VALUE";
			break;
		case Scanner::JSONTokenType::NULL_VALUE:
			prompt = "NULL_VALUE";
			break;
		case Scanner::JSONTokenType::LITERAL_TRUE:
			prompt = "LITERAL_TRUE";
			break;
		case Scanner::JSONTokenType::LITERAL_FALSE:
			prompt = "LITERAL_FALSE";
			break;
		case Scanner::JSONTokenType::VALUE_SEPARATOR:
			prompt = "VALUE_SEPARATOR";
			break;
		case Scanner::JSONTokenType::NAME_SEPARATOR:
			prompt = "NAME_SEPARATOR";
			break;
		case Scanner::JSONTokenType::END_OF_FILE:
			prompt = "END_OF_FILE";
			break;
		default:
			prompt = "UNKNOWN";
			break;
		}
		std::cout << prompt;
		return os;
	}
}
