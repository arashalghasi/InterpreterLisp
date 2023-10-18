#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <string>
#include <cctype>
#include <fstream>
#include <vector>
#include "Token.h"
#include <sstream>
#include "Exceptions.h"


class Tokenizer {
public:
	//Overloading Operator()
	std::vector<Token> operator()(std::ifstream& inputFile) {
		std::vector<Token> inputTokens;
		Tokenize(inputFile, inputTokens);
		return inputTokens;
	}
private:
	//we must do the recursive
	static void Tokenize(std::ifstream& inputFile, std::vector<Token>& inputTokens) {
		try
		{
			char ch;
			ch = inputFile.get();
			while (!inputFile.eof())
			{
				std::stringstream tmp;
				//clearing the string stream
				tmp.str(std::string());
				if (std::isspace(ch)) {
					ch = inputFile.get();
					continue;
				}
				else {
					if (ch == '(') {
						inputTokens.emplace_back(Token{ Token::LP, Token::idToWord[Token::LP] });
					}
					else if (ch == ')') {
						inputTokens.emplace_back(Token::RP, Token::idToWord[Token::RP]);
					}
					else if (ch == '-') {
						inputTokens.emplace_back(Token::NEGETIVE, Token::idToWord[Token::NEGETIVE]);
					}
					else if (std::isdigit(ch)) {
						tmp << ch;
						do {
							ch = inputFile.get();
							if (std::isalpha(ch)) throw SintaticError("(ERROR no valid Number)");
							if (std::isdigit(ch)) {
								tmp << ch;
							}
						} while (std::isdigit(ch));
						inputTokens.emplace_back(Token::NUM, tmp.str());
						continue;
					}
					else if (std::isalpha(ch)) {
						tmp << ch;
						do {
							ch = inputFile.get();
							if (std::isdigit(ch)) throw SintaticError("(ERROR no valid variable or keyWord)");
							if (std::isalpha(ch)) {
								tmp << ch;
							}
						} while (std::isalpha(ch));


						if (tmp.str() == "BLOCK" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::BLOCK, Token::idToWord[Token::BLOCK]);
							continue;
						}
						else if (tmp.str() == "SET" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::SET, Token::idToWord[Token::SET]);
							continue;
						}
						else if (tmp.str() == "ADD" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::ADD, Token::idToWord[Token::ADD]);
							continue;
						}
						else if (tmp.str() == "SUB" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::SUB, Token::idToWord[Token::SUB]);
							continue;
						}
						else if (tmp.str() == "DIV" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::DIV, Token::idToWord[Token::DIV]);
							continue;
						}
						else if (tmp.str() == "MUL" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::MUL, Token::idToWord[Token::MUL]);
							continue;
						}
						else if (tmp.str() == "PRINT" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::PRINT, Token::idToWord[Token::PRINT]);
							continue;
						}
						else if (tmp.str() == "INPUT" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::INPUT, Token::idToWord[Token::INPUT]);
							continue;
						}
						else if (tmp.str() == "IF" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::IF, Token::idToWord[Token::IF]);
							continue;
						}
						else if (tmp.str() == "WHILE" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::WHILE, Token::idToWord[Token::WHILE]);
							continue;
						}
						else if (tmp.str() == "LT" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::LT, Token::idToWord[Token::LT]);
							continue;
						}
						else if (tmp.str() == "GT" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::GT, Token::idToWord[Token::GT]);
							continue;
						}
						else if (tmp.str() == "EQ" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::EQ, Token::idToWord[Token::EQ]);
							continue;
						}
						else if (tmp.str() == "AND" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::AND, Token::idToWord[Token::AND]);
							continue;
						}
						else if (tmp.str() == "OR" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::OR, Token::idToWord[Token::OR]);
							continue;
						}
						else if (tmp.str() == "NOT" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::NOT, Token::idToWord[Token::NOT]);
							continue;
						}
						else if (tmp.str() == "TRUE" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::TRUE, Token::idToWord[Token::TRUE]);
							continue;
						}
						else if (tmp.str() == "FALSE" && (ch == '\n' || ch == ' ' || ch == '\t')) {
							inputTokens.emplace_back(Token::FALSE, Token::idToWord[Token::FALSE]);
							continue;
						}
						else {

							inputTokens.emplace_back(Token::VARIABLE, tmp.str());
							continue;
						}
					}
					else {
						std::stringstream tmp1{};
						tmp1 << "(ERROR  Symbol Unknown:   " << ch <<" )";
						throw LexicalError(tmp1.str());
					}
				}

				ch = inputFile.get();
			}

			if(inputTokens.size() < 4)
			{
				throw LexicalError("(ERROR  The Input file must have at least 4 symbols)");
			}

		}
		catch (const LexicalError& l) {
			std::cerr << l.what() << std::endl;
			exit(0);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			exit(0);
		}
	}
};
#endif
