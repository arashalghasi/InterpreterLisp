#ifndef PARSER_H
#define	PARSER_H

#include "Token.h"
#include <vector>
#include "Exceptions.h"
#include "Expression.h"
#include "ExpreStmtManager.h"
#include "Visitor.h"
#include <stdexcept>
#include "Logger.h"
// here we need just to make out objects and we should use our beautiful manager

class Parser {
public:
	explicit Parser(ExpreStmtManager& manager) : exp_manager_{ manager }, INFO("INFO.txt"), ERROR("ERROR.txt") { }

	std::vector<Statement*> operator()(const std::vector<Token>& tokenStream) {

		auto tokenStreamIterator = tokenStream.begin();
		TokenStreamEnd = tokenStream.end();
		return StatementParse(tokenStreamIterator);

	}

private:

	std::vector<Token>::const_iterator TokenStreamEnd;
	int BlockNumber = 0;
	ExpreStmtManager& exp_manager_;
	std::vector<std::vector<Statement*>> InsideVector = {};
	int stmtBlockInside = 0;
	Logger INFO;
	Logger ERROR;



	// --------------------------------------------------------------------- numExpressionParser -------------------------------------------------------
	Expression* ExpressionParse(std::vector<Token>::const_iterator& tokenStreamIterator) {
		try
		{
			if (tokenStreamIterator->Tag > 3 && tokenStreamIterator->Tag <= 7) {
				//the operation between ADD, SUB, MUL, DIV
				auto opCode = Operator::TokenWordToOpCode(tokenStreamIterator->Tag);
				INFO.log("the operation opCode is {ADD = 0, SUB = 1, MUL = 2, DIV = 3} type is: ");
				SafeNext(tokenStreamIterator);
				Expression* first;
				Expression* second;
				if (tokenStreamIterator->Tag == Token::LP) {
					INFO.log("We are in a nested first numeric expression for arithmetic operation");
					SafeNext(tokenStreamIterator);
					first = ExpressionParse(tokenStreamIterator);
					INFO.log("I got the first expression in the nested form");
				}
				else {

					if (tokenStreamIterator->Tag >= 2 && tokenStreamIterator->Tag <= 7) {
						first = ExpressionParse(tokenStreamIterator);
						INFO.log("I got the first expression");

					}
					else {
						throw SintaticError("(ERROR in parsing for the first expression of the arithmetic operation)");
					}
				}

				if (tokenStreamIterator->Tag == Token::LP) {
					INFO.log("So we are in a nested second numeric expression for arithmetic operation");
					SafeNext(tokenStreamIterator);
					second = ExpressionParse(tokenStreamIterator);
					INFO.log("I got the second expression in the nested form");
				}
				else {

					if (tokenStreamIterator->Tag >= 2 && tokenStreamIterator->Tag <= 7) {
						second = ExpressionParse(tokenStreamIterator);
						INFO.log("I got the second expression");
					}
					else {
						throw SintaticError("(ERROR in parsing for the second expression of the arithmetic operation)");
					}
				}


				if (tokenStreamIterator->Tag == Token::RP) {
					INFO.log("I saw the ) of the arithmetic operator ");
					Expression* opert = exp_manager_.MakeOperator(opCode, first, second);
					SafeNext(tokenStreamIterator);
					return opert;
				}
				else {
					throw SintaticError("(ERROR I did not find the ) of arithmetic operator)");
				}
			}

			else if (tokenStreamIterator->Tag == Token::NEGETIVE) {
				INFO.log("it is a negative numeric expression ");
				SafeNext(tokenStreamIterator);
				std::stringstream tmp{};
				tmp << tokenStreamIterator->Word;
				long value;
				tmp >> value;
				Expression* num = exp_manager_.MakeNumber(-value);
				INFO.log("The Positive value is:   ", -value);
				SafeNext(tokenStreamIterator);
				return num;
			}

			else if (tokenStreamIterator->Tag == Token::NUM) {
				INFO.log("it is a numeric expression ");
				std::stringstream tmp{};
				tmp << tokenStreamIterator->Word;
				long value;
				tmp >> value;
				Expression* num = exp_manager_.MakeNumber(value);
				INFO.log("The Positive value is:   ", value);
				SafeNext(tokenStreamIterator);
				return num;
			}
			else if (tokenStreamIterator->Tag == Token::VARIABLE) {
				INFO.log("I got a variable with the name:  ", tokenStreamIterator->Word);
				Expression* variableName = exp_manager_.MakeVariable(tokenStreamIterator->Word);
				SafeNext(tokenStreamIterator);
				return variableName;

			}
			else {
				throw SintaticError("(Error It is not a numeric expression)");
			}
		}
		catch (const SintaticError& s) {
			std::cerr << s.what() << std::endl;
			ERROR.log(s.what());
			exit(0);
			//std::terminate();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			ERROR.log(e.what());
			exit(0);
			//std::terminate();
		}
	}

	// --------------------------------------------------------------------- numExpressionParser finished -----------------------------------------


	//  method to parse the bool expression ------------------------------------------------------------------------------------------------------
	BoolExpr* boolParse(std::vector<Token>::const_iterator& tokenStreamIterator) {
		try
		{
			if (tokenStreamIterator->Tag > 13 && tokenStreamIterator->Tag < 17) {
				INFO.log("we are in a boolean expression GT,LT,EQ");
				auto BoolCode = RelOp::BoolCode(tokenStreamIterator->Tag);
				SafeNext(tokenStreamIterator);
				// so we chech the bool op for num, the expressions of Add, Sub, Mul , Div , variableNum, 
				if (tokenStreamIterator->Tag >= 0 && tokenStreamIterator->Tag < 8 || tokenStreamIterator->Tag == Token::NEGETIVE) {
					Expression* first;
					Expression* second;



					if (tokenStreamIterator->Tag == Token::LP) {
						INFO.log("We are in first nested expression of GT,LT,EQ");
						SafeNext(tokenStreamIterator);
						if (tokenStreamIterator->Tag > 1 && tokenStreamIterator->Tag < 8 || tokenStreamIterator->Tag == Token::NEGETIVE) {
							first = ExpressionParse(tokenStreamIterator);
							INFO.log("I got the first numExpression for the OPERATOR GT,LT,EQ");
						}
						else {
							throw SintaticError("(ERROR parse Error after a bool expression must be a numeric expression to do the comparision)");
						}

					}
					else {
						if (tokenStreamIterator->Tag > 1 && tokenStreamIterator->Tag < 8 || tokenStreamIterator->Tag == Token::NEGETIVE) {
							first = ExpressionParse(tokenStreamIterator);
							INFO.log("I got the first numExpression for the boolean GT,LT,EQ");
						}
						else {
							throw SintaticError("(ERROR parse Error after a bool expression must be a numeric expression)");
						}
					}

					if (tokenStreamIterator->Tag == Token::LP) {
						INFO.log("We are in second nested expression of GT,LT,EQ");
						SafeNext(tokenStreamIterator);
						if (tokenStreamIterator->Tag > 1 && tokenStreamIterator->Tag < 8 || tokenStreamIterator->Tag == Token::NEGETIVE) {
							second = ExpressionParse(tokenStreamIterator);
							INFO.log("I got the second numExpression for the OPERATOR GT,LT,EQ");
						}
						else {
							throw SintaticError("(ERROR parse Error after a first  must be a numeric expression to do the comparision)");
						}

					}
					else {
						if (tokenStreamIterator->Tag > 1 && tokenStreamIterator->Tag < 8 || tokenStreamIterator->Tag == Token::NEGETIVE) {
							second = ExpressionParse(tokenStreamIterator);
							INFO.log("I got the second numExpression for the OPERATOR GT,LT,EQ");
						}
						else {
							throw SintaticError("(ERROR parse Error after a first  must be a numeric expression)");
						}
					}


					if (tokenStreamIterator->Tag == Token::RP) {
						INFO.log("I saw the ) of GT, LT, EQ ");
						BoolExpr* boolExpr = exp_manager_.MakeRelOp(BoolCode, first, second);
						SafeNext(tokenStreamIterator);
						return boolExpr;
					}
					else {
						throw SintaticError("(ERROR I did not find the ) of GT, LT, EQ)");
					}

				}
				else {
					throw SintaticError("(ERROR parse Error after a bool expression must be a numeric expression to do the comparision)");
				}
			} //  NOT check ---------------------------------------------------------------------------------------------
			else if (tokenStreamIterator->Tag == 19) { // I changed this part ---------------->>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				INFO.log("we are in NOT OPERATOR");
				auto x = BoolOp::BoolOpertor(tokenStreamIterator->Tag);
				SafeNext(tokenStreamIterator);
				if (tokenStreamIterator->Tag == Token::LP) {
					// if after an AND , OR ,NOT there is not a ( so we can Understand that after it must be a TURE, or FALSE
					INFO.log("We are in a nested boolean expression in NOT");
					SafeNext(tokenStreamIterator);
					BoolExpr* result = boolParse(tokenStreamIterator);
					INFO.log("I got the numExpr of the NOT operator");
					if (tokenStreamIterator->Tag == Token::RP) {
						INFO.log("I saw the ) of NOT OPERATOR");
						BoolExpr* BoolExpressionNot = exp_manager_.MakeBoolOpNot(x, result);
						SafeNext(tokenStreamIterator);
						return BoolExpressionNot;
					}
					else {
						throw SintaticError("(ERROR I did not find the ) of NOT Operator)");
					}
				}
				else {
					// we check the NOT here and the token is TRUE or FALSE
					if (tokenStreamIterator->Tag > 19 && tokenStreamIterator->Tag < 22) {
						INFO.log("We are NOT in a nested boolean expression so we have only one option left --> which it should be True of false");
						BoolExpr* firstBoolExpr1 = boolParse(tokenStreamIterator);
						if (tokenStreamIterator->Tag == Token::RP) {
							INFO.log("I saw ) of NOT Operator");
							BoolExpr* BoolExpressionNot = exp_manager_.MakeBoolOpNot(x, firstBoolExpr1);
							SafeNext(tokenStreamIterator);
							return BoolExpressionNot;
						}
						else {
							throw SintaticError("(ERROR I did not find the ) of NOT Operator)");
						}
					}
					else {
						throw SintaticError("(ERROR parse Error after a bool expression must be a numeric expression to do the comparision)");
					}
				}
			} // AND OR CONDITION ----------------------------------------------
			else if (tokenStreamIterator->Tag > 16 && tokenStreamIterator->Tag < 19 || tokenStreamIterator->Tag == 0) {
				INFO.log("we are in a boolean AND, OR");
				auto x = BoolOp::BoolOpertor(tokenStreamIterator->Tag);
				SafeNext(tokenStreamIterator);
				BoolExpr* firstResult;
				BoolExpr* secondResult;
				if (tokenStreamIterator->Tag == Token::LP) {
					// if after an AND , OR ,NOT there is not a ( so we can Understand that after it must be a TRUE, or FALSE)
					INFO.log("We are in a nested boolean expression in AND, OR OPERATOR");
					SafeNext(tokenStreamIterator);
					firstResult = boolParse(tokenStreamIterator);
					INFO.log("I got the first numeric expression of AND, OR OPERATOR");
					if (tokenStreamIterator->Tag == Token::LP) {
						SafeNext(tokenStreamIterator);
						secondResult = boolParse(tokenStreamIterator);
						INFO.log("I got the second numeric expression of AND, OR operator");

						if (tokenStreamIterator->Tag == Token::RP) {
							INFO.log("I saw ) of AND, OR operator");
							BoolExpr* BoolExpressionNot = exp_manager_.MakeBoolOpAndOr(x, firstResult, secondResult);
							SafeNext(tokenStreamIterator);
							return BoolExpressionNot;
						}
						else {
							throw SintaticError("(ERROR I did not find the ) of AND, OR operation)");
						}
					}


				}
				else {
					// we check the NOT here and the token is TRUE or FALSE
					if (tokenStreamIterator->Tag > 19 && tokenStreamIterator->Tag < 22) {
						INFO.log("We are in NOT OPERATOR");
						BoolExpr* firstBoolExpr1 = boolParse(tokenStreamIterator);

						if (tokenStreamIterator->Tag == Token::RP) {
							INFO.log("I saw ) of Not expression");
							BoolExpr* BoolExpressionNot = exp_manager_.MakeBoolOpNot(x, firstBoolExpr1);
							SafeNext(tokenStreamIterator);
							return BoolExpressionNot;
						}
						else {
							throw SintaticError("(ERROR I did not find the ) of AND, OR operation)");
						}
					}
				}
			}// we check the TRUE and FALSE
			else if (tokenStreamIterator->Tag == 20 || tokenStreamIterator->Tag == 21) {
				INFO.log("Am i here in pure TRUE and False");
				BoolExpr* boolExprTrueFalse;
				boolExprTrueFalse = exp_manager_.MakeBoolConst(tokenStreamIterator->Tag);
				SafeNext(tokenStreamIterator);
				return boolExprTrueFalse;
			}
			else {
				throw SintaticError("(ERROR parse Error after a bool expression must be a numeric expression to do the comparision)");
			}
		}
		catch (const SintaticError& s) {
			std::cerr << s.what() << std::endl;
			ERROR.log(s.what());
			exit(0);
			//std::terminate();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			ERROR.log(e.what());
			exit(0);
			//std::terminate();
		}
		return nullptr;
	}

	//  method to parse the bool parse finished ------------------------------------------------------------------------------------------------------



	// ----------------------------------------- statement implementation -----------------------------------------------------
	// so here we devied the tokens into the statements
	std::vector<Statement*> StatementParse(std::vector<Token>::const_iterator& tokenStreamIterator) {
		try
		{
			std::vector<Statement*> statementListFinal;
			while (tokenStreamIterator != TokenStreamEnd) {
				INFO.log("we are in StatementParse!!");
				if (tokenStreamIterator->Tag == Token::LP) {
					auto x = tokenStreamIterator + 1;
					if (x->Tag == Token::BLOCK) {
						SafeNext(tokenStreamIterator);
						RecursiveParse(tokenStreamIterator);
						statementListFinal = InsideVector[1];
						INFO.log("We read total blocks :", BlockNumber);
						INFO.log("Program ENDED SUCCESSFULLY");
						INFO.log("\n\n\n\n\n\n\n\n\n");
						return statementListFinal;

					}
					else if (x->Tag > 7 && x->Tag < 13) {
						INFO.log("So we have a single statements");
						SafeNext(tokenStreamIterator);
						int stmtType = tokenStreamIterator->Tag;
						Statement* stmt = RecursiveParse(tokenStreamIterator);
						statementListFinal.push_back(stmt);
						INFO.log("We read total blocks :", BlockNumber);
						INFO.log("Program ENDED SUCCESSFULLY");
						INFO.log("\n\n\n\n\n\n\n\n\n");
						return statementListFinal; // ---------------------------------> double check this part
					}

					else {
						throw SintaticError("(ERROR the program must start with a stmt_block which is : statement | ( BLOCK statement_list ))");
					}
				}
				else {
					throw SintaticError("(ERROR ParseError because the program did not start with a '(' )");
				}
			}
		}
		catch (const SintaticError& s) {
			std::cerr << s.what() << std::endl;
			ERROR.log(s.what());
			exit(0);
			//std::terminate();
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			ERROR.log(e.what());
			exit(0);
			//std::terminate();
		}

		return {};
	}

	// ----------------------------------------- statement implementation  finish  -------------------------------------------
	Statement* RecursiveParse(std::vector<Token>::const_iterator& tokenStreamIterator) {
		try {
			std::vector<Statement*> first = {  };
			// because we want to work with InsideVector[stmtBlockInside] this such a thing 
			InsideVector.push_back(first);

			while (tokenStreamIterator != TokenStreamEnd) {
				INFO.log(" We are now in RecursiveParse function");
				// so must understand if we have a block of statement or just a single statement
				if (tokenStreamIterator->Tag == Token::BLOCK) {
					++BlockNumber;
					++stmtBlockInside;
					// we instantiate the vector with empty element
					InsideVector.push_back(first);
					Statement* stmt;
					SafeNext(tokenStreamIterator);
					INFO.log("Here we are in a block list_statement");
					INFO.log(" We are in block: ", stmtBlockInside);
					INFO.log("To this moment we have Blocks : ", BlockNumber);
					while (tokenStreamIterator->Tag != Token::RP) {
						SafeNext(tokenStreamIterator);
						stmt = RecursiveParse(tokenStreamIterator);
						INFO.log("I Got a statement for the block ");
						InsideVector[stmtBlockInside].push_back(stmt);

						//Before seeing the RP is an Error
						if (tokenStreamIterator == TokenStreamEnd) {
							throw SintaticError("(ERROR I did not find the ) of Block)");
						}
					}
					if(InsideVector[stmtBlockInside].size() < 2){
						throw SintaticError("(ERROR The Block must have at least two Statements)");
					}

					if (tokenStreamIterator->Tag == Token::RP) {
						INFO.log("I saw ) of the block");
						SafeNext(tokenStreamIterator);
						break;
					}
					else {
						throw SintaticError("(ERROR I did not find the ) of Block)");
					}
				}
				// SET statement ------------------------------------------------------------------------
				else if (tokenStreamIterator->Tag == Token::SET) {
					INFO.log("We are in set statement!");
					SafeNext(tokenStreamIterator);
					// here we declare the variable or it goes to ERROR
					Expression* VariableId;
					Expression* numExprVariableNumber;
					if (tokenStreamIterator->Tag == Token::VARIABLE) {
						VariableId = ExpressionParse(tokenStreamIterator);
					}
					else {
						throw SintaticError("(ERROR parsing problem in definition a variable)");
					}
					// here we initialize the varible 
					if (tokenStreamIterator->Tag >= 0 && tokenStreamIterator->Tag < 8 || tokenStreamIterator->Tag == Token::NEGETIVE)
					{
						if (tokenStreamIterator->Tag == Token::LP) {
							INFO.log("We are in a nested statement");
							SafeNext(tokenStreamIterator);
							numExprVariableNumber = ExpressionParse(tokenStreamIterator);
							INFO.log("I got the numeric expression of the set statement and it was nested");
						}
						else {
							INFO.log("We are NOT in a nested statement ");
							numExprVariableNumber = ExpressionParse(tokenStreamIterator);
							INFO.log("I got the numeric expression of the set statement");
						}

					}
					else {
						throw SintaticError("(ERROR parsing problem in initialization of the  variable)");

					}

					if (tokenStreamIterator->Tag == Token::RP) {
						Statement* setStmt = exp_manager_.MakeSetStmt(VariableId, numExprVariableNumber);
						INFO.log("I made the set statement for you !!!");
						INFO.log("I saw the ) of set statement");
						SafeNext(tokenStreamIterator);
						return setStmt;
					}
					else {
						throw SintaticError("(ERROR I did not find the ) of Set statement)");

					}
				} // While statement -------------------------------------------------------------------------------------
				else if (tokenStreamIterator->Tag == Token::WHILE) {
					INFO.log("We are in WHILE statement!");
					SafeNext(tokenStreamIterator);
					BoolExpr* boolExpr;
					std::vector<Statement*> stmtBlock;
					Statement* stmt;
					if (tokenStreamIterator->Tag >= 0 && tokenStreamIterator->Tag < 22) {
						if (tokenStreamIterator->Tag == Token::LP) {
							SafeNext(tokenStreamIterator);
							boolExpr = boolParse(tokenStreamIterator);
							INFO.log("I got the bool of while statement");
						}
						else {
							boolExpr = boolParse(tokenStreamIterator);
							INFO.log("I got the bool of while statement");
						}

						if (tokenStreamIterator->Tag == Token::LP) {
							SafeNext(tokenStreamIterator);
							if (tokenStreamIterator->Tag > 7 && tokenStreamIterator->Tag < 14) {
								if (tokenStreamIterator->Tag == Token::BLOCK) {
									INFO.log("Here we are in block_stmt inside the while statement");
									RecursiveParse(tokenStreamIterator);
									stmtBlock = InsideVector[stmtBlockInside];
									INFO.log("The while is in block : ", stmtBlockInside);
									--stmtBlockInside;
									INFO.log("I decremented  the value of block by 1 and now is: ", stmtBlockInside);

								}
								else {
									stmt = RecursiveParse(tokenStreamIterator);
									stmtBlock.push_back(stmt);
								}

							}
							else {
								throw SintaticError("(ERROR after a boolExpr must be a stmt_block)");
							}
						}
						else {
							throw SintaticError("(ERROR parse error because after an IF should be a (stmt Block))");
						}
					}
					else {
						throw SintaticError("(ERROR after a while must be boolExpression)");
					}


					if (tokenStreamIterator->Tag == Token::RP) {
						INFO.log("I saw ) of while statement");
						Statement* whileResult = exp_manager_.MakeWhileStmt(boolExpr, stmtBlock);
						SafeNext(tokenStreamIterator);
						return whileResult;
					}
					else {
						throw SintaticError("(ERROR I did not find the ) of while statement)");
					}


				} // if statement ------------------------------------------------------------------------
				else if (tokenStreamIterator->Tag == Token::IF) {
					INFO.log("We are in IF statement");
					SafeNext(tokenStreamIterator);
					BoolExpr* boolExpr;
					std::vector<Statement*> firstStmtBlock;
					std::vector<Statement*> secondStmtBlock;
					Statement* firstStmt;
					Statement* secondStmt;
					if (tokenStreamIterator->Tag == 0 || tokenStreamIterator->Tag == 20 || tokenStreamIterator->Tag == 21) {

						if (tokenStreamIterator->Tag == Token::LP) {
							SafeNext(tokenStreamIterator);
							boolExpr = boolParse(tokenStreamIterator);
							INFO.log("I got he bool for if statement");
						}
						else {
							boolExpr = boolParse(tokenStreamIterator);
							INFO.log("I got he bool for if statement");
						}


						if (tokenStreamIterator->Tag == Token::LP) {
							SafeNext(tokenStreamIterator);
							if (tokenStreamIterator->Tag > 7 && tokenStreamIterator->Tag < 14) {
								if (tokenStreamIterator->Tag == Token::BLOCK) {
									RecursiveParse(tokenStreamIterator);
									INFO.log("The if block is : ", stmtBlockInside);
									firstStmtBlock = InsideVector[stmtBlockInside];
									--stmtBlockInside;
									INFO.log("HI decremented the value of block by 1 now is: ", stmtBlockInside);
								}
								else {
									INFO.log("Here we are in first stmt inside the IF statement");
									firstStmt = RecursiveParse(tokenStreamIterator);
									firstStmtBlock.push_back(firstStmt);
								}

							}
							else {
								throw SintaticError("(ERROR after a boolExpr must be a stmt_block)");
							}
						}
						else {
							throw SintaticError("(ERROR parse error because after an IF should be a (stmt Block)");
						}


						if (tokenStreamIterator->Tag == Token::LP) {
							SafeNext(tokenStreamIterator);
							if (tokenStreamIterator->Tag > 7 && tokenStreamIterator->Tag < 14) {
								if (tokenStreamIterator->Tag == Token::BLOCK) {
									RecursiveParse(tokenStreamIterator);
									secondStmtBlock = InsideVector[stmtBlockInside];
									INFO.log("The if block is : ", stmtBlockInside);
									--stmtBlockInside;
									INFO.log("I decremented the value of block by 1 ");
								}
								else {
									INFO.log("Here we are in second stmt inside the IF statement");
									secondStmt = RecursiveParse(tokenStreamIterator);
									secondStmtBlock.push_back(secondStmt);
									INFO.log("Tag code is in end of second statement of IF  : ", tokenStreamIterator->Tag);
								}

							}
							else {
								throw SintaticError("(ERROR after a boolExpr must be a stmt_block)");
							}
						}
						else {
							throw SintaticError("(ERROR parse error because after an IF should be a (stmt Block))");
						}
					}
					else {
						throw SintaticError("(ERROR after a while must be boolExpression)");
					}




					if (tokenStreamIterator->Tag == Token::RP) {
						INFO.log("We are at the end of the if statement");
						Statement* ifStatement = exp_manager_.MakeIfStmt(boolExpr, firstStmtBlock, secondStmtBlock);
						SafeNext(tokenStreamIterator);
						return ifStatement;
					}
					else {
						throw SintaticError("(ERROR I did not find the ) of IF statement)");
					}


				} // Print statement ----------------------------------------------------------------------------------------
				else if (tokenStreamIterator->Tag == Token::PRINT) {
					INFO.log("We are in print statement");
					SafeNext(tokenStreamIterator);
					// after statement we should find a numExpr
					Expression* numExpression;
					if (tokenStreamIterator->Tag >= 0 && tokenStreamIterator->Tag < 8 || tokenStreamIterator->Tag == Token::NEGETIVE) {
						if (tokenStreamIterator->Tag == Token::LP) {
							INFO.log("We are in a nested numeric expression for print statement");
							SafeNext(tokenStreamIterator);
							numExpression = ExpressionParse(tokenStreamIterator);
						}
						else {
							numExpression = ExpressionParse(tokenStreamIterator);
						}

						if (tokenStreamIterator->Tag == Token::RP) {
							//printResult->Interpret();
							Statement* printResult = exp_manager_.makePrintStmt(numExpression);
							INFO.log("I saw the ) of print statement");
							SafeNext(tokenStreamIterator);
							return printResult;
						}
						else {
							throw SintaticError("(ERROR I did not find the ) of Print statement)");
						}

					}
					else {
						throw SintaticError("(ERROR parsing error because after a print statement must be numeric expression)");
					}
				} else if(tokenStreamIterator->Tag == Token::INPUT)
				{
					INFO.log("We are in print statement");
					SafeNext(tokenStreamIterator);
					//after the input statement we should find a variable
					Expression* numExpression;
					numExpression = ExpressionParse(tokenStreamIterator);
					if(tokenStreamIterator->Tag == Token::RP)
					{
						INFO.log("I saw the ) of print statement");
						Statement* printResult = exp_manager_.MakeInputStmt(numExpression);
						SafeNext(tokenStreamIterator);
						return printResult;
					}
					else {
						throw SintaticError("(ERROR I did not find the ) of INPUT statement)");
					}

				}
				else if (tokenStreamIterator->Tag == Token::RP) {
					INFO.log("IF YOU SEE THIS IT MEANS THERE IS PROBLEM IN RECURSIVE PARSE !!!");
				}
				else {
					throw SintaticError("(ERROR in parsing end of tokenstream)");
				}

				ERROR.log("IF YOU SEE THIS IT MEANS THERE IS PROBLEM IN RECURSIVE PARSE !!!");
			}

		}
		catch (const SintaticError& s) {
			std::cerr << s.what() << std::endl;
			ERROR.log(s.what());
			exit(0);
			//std::terminate();

		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			ERROR.log(e.what());
			exit(0);
			//std::terminate();
		}

		return nullptr;
	}

	void SafeNext(std::vector<Token>::const_iterator& tokenStreamIterator) {
		try
		{
			if (tokenStreamIterator == TokenStreamEnd) {
				throw SintaticError("(ERROR tokenStreamIterator ended!)");
			}
			else {
				++tokenStreamIterator;
			}
		}
		catch (const SintaticError& s) {
			std::cerr << s.what() << std::endl;
			ERROR.log(s.what());
			exit(0);
			//std::terminate();
		}
	}
};
#endif