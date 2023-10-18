#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <sstream>
struct Token {

	static constexpr const int LP = 0;
	static constexpr const int RP = 1;
	//the variables
	static constexpr const int VARIABLE = 2;
	// the numbers
	static constexpr const int NUM = 3;
	//The keywords from here
	static constexpr const int ADD = 4;
	static constexpr const int SUB = 5;
	static constexpr const int MUL = 6;
	static constexpr const int DIV = 7;
	static constexpr const int SET = 8;
	static constexpr const int PRINT = 9;
	static constexpr const int INPUT = 10;
	static constexpr const int IF = 11;
	static constexpr const int WHILE = 12;
	static constexpr const int BLOCK = 13;
	static constexpr const int GT = 14;
	static constexpr const int LT = 15;
	static constexpr const int EQ = 16;
	static constexpr const int AND = 17;
	static constexpr const int OR = 18;
	static constexpr const int NOT = 19;
	static constexpr const int TRUE = 20;
	static constexpr const int FALSE = 21;
	static constexpr const int NEGETIVE = 22;

	static constexpr const char* idToWord[]{
		"(",")","VARIABLE","NUM","ADD","SUB","MUL","DIV","SET","PRINT","INPUT","IF","WHILE","BLOCK","GT","LT","EQ","AND","OR","NOT","TRUE","FALSE", "NEGETIVE"
	};

	Token(int tag, const char* word) : Tag{ tag }, Word{ word } {}
	Token(int tag, std::string word) : Tag{ tag }, Word{ std::move(word) } {}
	int Tag;
	std::string Word;
};

inline std::ostream& operator<<(std::ostream& os, const Token& t) {
	std::stringstream tmp;
	tmp << t.Tag <<  " : " << t.Word << std::endl;
	os << tmp.str();
	return os;
}
#endif
