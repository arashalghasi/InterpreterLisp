#include <iostream>
#include <fstream>
#include "Expression.h"
//#include "Expression.cpp"
#include "Token.h"
#include "Tokenizer.h"
#include "Exceptions.h"
#include "Parser.h"
#include "ExpreStmtManager.h"
#include "Visitor.h"
#include "Logger.h"


Logger ERROR("ERROR.txt");
Logger INFO("INFO.txt");
// A simple LIPS like Interpreter (Project of the course Information and Computation at Genoa University)
int main(int argc, char* argv[])
{

	try
	{
		if (argc < 2) {
			throw FileError("(ERROR The file is not specified )");
		}
		std::ifstream inputFile;
		inputFile.open(argv[1]);
		//the first step is to tokenize the inputFile

		Tokenizer tokenize;
		std::vector<Token> inputTokens;

		//Lexical Analysis
		inputTokens = std::move ( tokenize(inputFile));
		inputFile.close();
		for (const Token& token : inputTokens) {
			INFO.log(token);
		}
		ExpreStmtManager manager;
		Parser parser{ manager };

		//Synthetic Analysis
		std::vector<Statement*> stmtList = parser(inputTokens);

		EvaluationVisitor* EvalVisitor;
		EvalVisitor = new EvaluationVisitor();

		//Semantic Analysis
		for (Statement* stmt : stmtList) {
			stmt->Accept(EvalVisitor);
		}

	}
	catch (const FileError& ef) 
	{
		std::cerr << ef.what() << std::endl;
		ERROR.log(ef.what());
		return EXIT_FAILURE;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		ERROR.log(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
