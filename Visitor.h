#ifndef VISITOR_H
#define VISITOR_H
#include "Expression.h"
#include <iostream>
#include "Exceptions.h"
#include <vector>
#include "GlobalTest.h"
#include "Logger.h"


// ------------------------------------------------ Global finished ----------------------
class Visitor {
public:
	virtual ~Visitor() = default;
	virtual void visitWhileStmt(Whilestmt* whileStmt) = 0;
	virtual void visitIfStmt(Ifstmt* ifStmt) = 0;
	virtual void visitSetStmt(SetStmt* setStmt) = 0;
	virtual void visitInputStmt(Inputstmt* inputStmt) = 0;
	virtual void visitPrintStmt(Printstmt* printStmt) = 0;
	virtual void visitNumberExpr(Number* numExpression) = 0;
	virtual void visitOperatorExpr(Operator* operatorExpression) = 0;
	virtual void visitVariableExpr(Variable* variableNameExpression) = 0;
	virtual void visitBoolConstBoolExpr(BoolConst* boolCOnstBoolExpr) = 0;
	virtual void visitRelOpBoolExpr(RelOp* RelOpBoolExpr) = 0;
	virtual void visitBoolOpBoolExpr(BoolOp* BoolOpBoolExpr) = 0;
};


//visitor concreto per la valutazione delle espressioni

class EvaluationVisitor : public Visitor {
public:



	EvaluationVisitor() : AccumulatorNumExpressions{}, AccumulatorExpression{}, AccumulatorName{},
		AccumulatorBoolExpr{}, AccumulatorStatement{}, INFO("INFO.txt"), ERROR("ERROR.txt")
	{
	}


	void visitWhileStmt(Whilestmt* whileStmt) override {
		try
		{
			INFO.log("Visitor ---->  We are in visitWhileStmt");
			bool boolexpression;
			BoolExpr* boolExpr;
			boolExpr = whileStmt->GetBoolExpr();
			boolExpr->Accept(this);
			std::vector<Statement*> firstStmtBlock;
			firstStmtBlock = whileStmt->GetStmtBlock();
			boolexpression = AccumulatorBoolExpr.back(); AccumulatorBoolExpr.pop_back(); --Counter;
			INFO.log("The result of the while is: ", boolexpression);

			while (boolexpression) {
				for (Statement* element : firstStmtBlock) {
					element->Accept(this);
				}

				boolExpr->Accept(this);
				boolexpression = AccumulatorBoolExpr.back(); AccumulatorBoolExpr.pop_back();
				INFO.log("The result of the while is: ", boolexpression);
			}
			INFO.log("The result of the while is: ", boolexpression);
			INFO.log("The WHILE FINISHED BYE");

		}
		catch (const SintaticError& se)
		{
			std::cerr << se.what() << std::endl;
			ERROR.log(se.what());
			exit(0);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			ERROR.log(e.what());
			exit(0);
		}

	}

	void visitIfStmt(Ifstmt* ifStmt)  override {
		INFO.log("Visitor ---->  We are in visitIfStmt");
		bool boolexpression;
		BoolExpr* boolExpr;
		std::vector<Statement*> firstStmtBlock;
		std::vector<Statement*> secondStmtBlock;
		boolExpr = ifStmt->GetBoolExpr();
		boolExpr->Accept(this);
		boolexpression = AccumulatorBoolExpr.back(); AccumulatorBoolExpr.pop_back(); --Counter;
		firstStmtBlock = ifStmt->GetFirstStmtBlock();
		secondStmtBlock = ifStmt->GetSecondStmtBlock();
		INFO.log("visitIfStmt ---> and the bool of the If is :", boolexpression);
		if (boolexpression)
		{

			for (Statement* element : firstStmtBlock) {
				element->Accept(this);
			}
		}
		else
		{

			for (Statement* element : secondStmtBlock) {
				element->Accept(this);
			}
		}
	}

	void visitSetStmt(SetStmt* setStmt) override {
		try
		{
			INFO.log("Visitor ---->  We are in visitSetStmt");
			Expression* variableId = setStmt->GetVariableId();
			variableId->Accept(this);
			std::string name = AccumulatorName.back(); AccumulatorName.pop_back();
			INFO.log("The name of first of the set stmt is:", name);
			// In set statement we just need the name and we do not need any number
			if (!AccumulatorNumExpressions.empty()) { AccumulatorNumExpressions.pop_back(); }
			Expression* numExpr = setStmt->GetNumExpr();
			numExpr->Accept(this);
			if (AccumulatorNumExpressions.size() == 0)
			{
				throw SemanticError("(ERROR: The second variable is not Declared!)");
			}
			long num = AccumulatorNumExpressions.back(); AccumulatorNumExpressions.pop_back();
			if (!AccumulatorName.empty()) { --Counter; AccumulatorName.pop_back(); }
			INFO.log("The value of second of the set stmt is: ", num);
			if (!AccumulatorName.empty())
			{

				throw SintaticError("(ERROR: AccumulatorName is not empty!)");
			}

			Global::Get().addvariable(name, num);
		}
		catch (const SintaticError& se)
		{
			std::cerr << se.what() << std::endl;
			ERROR.log(se.what());
			exit(0);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			ERROR.log(e.what());
			exit(0);
		}
	}

	void visitInputStmt(Inputstmt* inputStmt) override {
		INFO.log("Visitor ---->  We are in visitInputStmt");
		Expression* variableId = inputStmt->GetNumExprVariableId();
		variableId->Accept(this);

		long num;
		std::cin >> num;

		std::string name = AccumulatorName.back(); AccumulatorName.pop_back();

		Global::Get().addvariable(name, num);
	}

	void visitPrintStmt(Printstmt* printStmt) override {
		INFO.log("Visitor ---->  We are in visitPrintStmt");
		Expression* numExpr = printStmt->GetNumExprssion();
		numExpr->Accept(this);
		if (AccumulatorNumExpressions.size() == 0)
		{
			throw SemanticError("(ERROR: The second variable is not Declared!)");
		}
		auto num = AccumulatorNumExpressions.back(); AccumulatorNumExpressions.pop_back();
		std::cout << num << std::endl;
		INFO.log("Print:  ", num);
	}


	void visitNumberExpr(Number* numberExpression) override {
		INFO.log("Visitor ---->  We are in visitNumberExpr ");
		AccumulatorNumExpressions.push_back(numberExpression->GetValue());
	}

	void visitOperatorExpr(Operator* operatorExpression) override {

		try
		{
			INFO.log("Visitor ---->  We are in visitOperatorExpr");

			Expression* left = operatorExpression->GetLeft();
			left->Accept(this);
			if (AccumulatorNumExpressions.size() == 0)
			{
				throw SemanticError("(ERROR: The second variable is not Declared!)");
			}
			long lval = AccumulatorNumExpressions.back();AccumulatorNumExpressions.pop_back();
			if (!AccumulatorName.empty()) { --Counter; AccumulatorName.pop_back(); }
			INFO.log("Visitor---- > We are in visitOperatorExpr -------> value of first : ", lval);



			Expression* right = operatorExpression->GetRight();
			right->Accept(this);
			if (AccumulatorNumExpressions.size() == 0)
			{
				throw SemanticError("(ERROR: The second variable is not Declared!)");
			}

			long rval = AccumulatorNumExpressions.back();AccumulatorNumExpressions.pop_back();
			if (!AccumulatorName.empty()) { --Counter; AccumulatorName.pop_back(); }
			INFO.log("Visitor---- > We are in visitOperatorExpr -------> value of second : ", rval);
			auto op = operatorExpression->GetOpCode();

			switch (op)
			{
			case Operator::ADD: AccumulatorNumExpressions.push_back(lval + rval); INFO.log("ADD: ", lval + rval); break;
			case Operator::SUB: AccumulatorNumExpressions.push_back(lval - rval); INFO.log("SUB: ", lval - rval); break;
			case Operator::MUL: AccumulatorNumExpressions.push_back(lval * rval); INFO.log("MUL: ", lval * rval); break;
			case Operator::DIV:
				if (rval == 0) {
					throw SintaticError("(ERROR division by zero in NOT !)");
				}
				else {
					AccumulatorNumExpressions.push_back(lval / rval); INFO.log("DIV: ", lval / rval); break;
				}

			default: ERROR.log("Visitor ---->  ERROR: Unknown operator!", Counter); break;
			}
		}
		catch (const SintaticError& si) {
			std::cerr << si.what() << std::endl;
			ERROR.log(si.what());
			exit(0);
		}
		catch (const SemanticError& se) {
			std::cerr << se.what() << std::endl;
			ERROR.log(se.what());
			exit(0);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			ERROR.log(e.what());
			exit(0);
		}
	}

	void visitVariableExpr(Variable* variableNameExpression) override {
		try
		{
			INFO.log("Visitor ---->  We are in visitVariableExpr");
			AccumulatorName.push_back(variableNameExpression->GetValue());
			INFO.log("The variable name is: ", variableNameExpression->GetValue());
			// HERE WE CHECK IF THE VARIABLE EXIST IN THE GLOBAL MAP
			if (Global::Get().check_varible(variableNameExpression->GetValue())) {
				++Counter;
				INFO.log("The counter value is --->>>>><<<<.>>>>>>>", Counter);
				INFO.log("Visitor ---->  We are in visitVariableExpr ---> the variable exist ");
				const long value = Global::Get().GetVarNumeric(variableNameExpression->GetValue());
				AccumulatorNumExpressions.push_back(value);
				INFO.log("The value of the variable added to the stack! ");
				INFO.log(" We got the value of variable from **GLOBAL**", variableNameExpression->GetValue());
			}
		}
		catch (const SemanticError& se) {
			std::cerr << se.what() << std::endl;
			ERROR.log(se.what());
			exit(0);
			//std::terminate();
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			ERROR.log(e.what());
			exit(0);
			//std::terminate();
		}

	}

	void visitBoolConstBoolExpr(BoolConst* boolConstBoolExpr) override {
		INFO.log("Visitor ---->  We are in visitBoolConstBoolExpr");
		AccumulatorBoolExpr.push_back(boolConstBoolExpr->GetBool());

	}

	void visitRelOpBoolExpr(RelOp* RelOpBoolExpr) override {
		try
		{
			INFO.log("Visitor ---->  We are in visitRelOpBoolExpr");
			auto op = RelOpBoolExpr->GetBoolCode();
			Expression* firstNumbereExpr = RelOpBoolExpr->GetFirst();
			firstNumbereExpr->Accept(this);
			if(AccumulatorNumExpressions.size() == 0)
			{
				throw SemanticError("(ERROR: The first variable is not Declared!)");
			}
			long first = AccumulatorNumExpressions.back(); AccumulatorNumExpressions.pop_back();
			INFO.log("I got the first number of RelOp: ", first);
			if (!AccumulatorName.empty()) { --Counter; AccumulatorName.pop_back(); }
			Expression* SecondNumbereExpr = RelOpBoolExpr->GetSecond();
			SecondNumbereExpr->Accept(this);
			if (AccumulatorNumExpressions.size() == 0)
			{
				throw SemanticError("(ERROR: The second variable is not Declared!)");
			}
			long second = AccumulatorNumExpressions.back(); AccumulatorNumExpressions.pop_back();
			if (!AccumulatorName.empty()) { --Counter; AccumulatorName.pop_back(); }
			INFO.log("I got the second number of RelOp: ", second);

			switch (op)
			{
			case RelOp::GT: AccumulatorBoolExpr.push_back(first > second); INFO.log("GT: ", (first > second)); break;
			case RelOp::LT: AccumulatorBoolExpr.push_back(first < second); INFO.log("LT: ", (first < second)); break;
			case RelOp::EQ: AccumulatorBoolExpr.push_back(first == second); INFO.log("EQ: ", (first == second)); break;
			default: ERROR.log("Visitor ---->  ERROR: MUST be GT, LT, EQ!"); break;

			}

		}
		catch (const SemanticError& se)
		{
			std::cerr << se.what() << std::endl;
			ERROR.log(se.what());
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

	void visitBoolOpBoolExpr(BoolOp* BoolOpBoolExpr) override {

		INFO.log("Visitor ---->  We are in visitBoolOpBoolExpr");
		BoolExpr* firstBoolExpr;
		BoolExpr* secondBoolExpr;
		bool first;
		bool second;
		auto op = BoolOpBoolExpr->GetBoolOperator();

		if (op != BoolOp::NOT)
		{
			if (op == BoolOp::AND)
			{
				firstBoolExpr = BoolOpBoolExpr->GetFirstBoolExpr();
				firstBoolExpr->Accept(this);
				first = AccumulatorBoolExpr.back(); AccumulatorBoolExpr.pop_back();
				INFO.log("visitBoolOpBoolExpr ->>> the first bool is: ", first);

				if (!first)
				{
					AccumulatorBoolExpr.push_back(false);
					INFO.log("AND: ", false);

					return;
				}

				secondBoolExpr = BoolOpBoolExpr->GetSecondBoolExpr();
				secondBoolExpr->Accept(this);
				second = AccumulatorBoolExpr.back(); AccumulatorBoolExpr.pop_back();
				INFO.log("visitBoolOpBoolExpr ->>> the second bool is: ", second);

				if (!second)
				{
					AccumulatorBoolExpr.push_back(false);
					INFO.log("AND: ",false);
					return;
				}

				AccumulatorBoolExpr.push_back(true);
				INFO.log("AND: ", true);

			}
			else if (op == BoolOp::OR)
			{
				firstBoolExpr = BoolOpBoolExpr->GetFirstBoolExpr();
				firstBoolExpr->Accept(this);
				first = AccumulatorBoolExpr.back(); AccumulatorBoolExpr.pop_back();
				INFO.log("visitBoolOpBoolExpr ->>> the first bool is: ", first);

				if (first)
				{
					AccumulatorBoolExpr.push_back(true);
					INFO.log("OR: ", true);
					return;
				}

				secondBoolExpr = BoolOpBoolExpr->GetSecondBoolExpr();
				secondBoolExpr->Accept(this);
				second = AccumulatorBoolExpr.back(); AccumulatorBoolExpr.pop_back();
				INFO.log("visitBoolOpBoolExpr ->>> the second bool is: ", second);

				if (second)
				{

					AccumulatorBoolExpr.push_back(true);
					INFO.log("OR: ", true);
					return;
				}

				AccumulatorBoolExpr.push_back(false);
				INFO.log("OR: ", false);
			}

		}
		else
		{
			firstBoolExpr = BoolOpBoolExpr->GetFirstBoolExpr();
			firstBoolExpr->Accept(this);
			first = AccumulatorBoolExpr.back(); AccumulatorBoolExpr.pop_back();
			INFO.log("visitBoolOpBoolExpr ->>> the first bool is: ", first);
			AccumulatorBoolExpr.push_back(!first);INFO.log("NOT: ", (!first)); return;

		}

	}

private:
	std::vector<long> AccumulatorNumExpressions;
	std::vector<Expression*> AccumulatorExpression;
	std::vector<std::string> AccumulatorName;
	std::vector<bool> AccumulatorBoolExpr;
	std::vector<Statement*> AccumulatorStatement;
	int Counter = 0;
	Logger INFO;
	Logger ERROR;
};

#endif
