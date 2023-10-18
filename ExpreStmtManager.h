#ifndef EXPRESTMTMANAGER_H
#define EXPRESTMTMANAGER_H

#include <utility>

#include "Expression.h"

class ExpreStmtManager {
public:

	void clearMemory() {

		auto s = StmtAllocated.begin();
		auto e = ExprAllocated.begin();
		auto be = BoolExprAllocated.begin();
		for (; s != StmtAllocated.end(); ++s) {
			delete(*s);
		}

		for (; e != ExprAllocated.end(); ++e) {
			delete(*e);
		}

		for (; be != BoolExprAllocated.end(); ++be) {
			delete(*be);
		}


		ExprAllocated.resize(0);
		StmtAllocated.resize(0);
		BoolExprAllocated.resize(0);
	}

	ExpreStmtManager() = default;
	~ExpreStmtManager() {
		clearMemory();

	}

	ExpreStmtManager(const ExpreStmtManager& other) = delete;
	ExpreStmtManager& operator=(const ExpreStmtManager& other) = delete;

	// -------------------------------------------------- Expression Making start --------------------------------------------------------

	Expression* MakeNumber(long value) {
		Expression* expr = new Number(value);
		ExprAllocated.push_back(expr);
		return expr;
	}


	Expression* MakeOperator(Operator::OpCode op, Expression* left, Expression* right) {
		Expression* expr = new Operator(op, left, right);
		ExprAllocated.push_back(expr);
		return expr;
	}


	Expression* MakeVariable(std::string str) {
		Expression* expr = new Variable(std::move(str));
		ExprAllocated.push_back(expr);
		return expr;
	}


	// -------------------------------------------------- Expression Making end --------------------------------------------------------

	// -------------------------------------------------- BoolExpr making start ------------------------------------------------------


	BoolExpr* MakeBoolConst(int a) {
		BoolExpr* boolExpr = new BoolConst(a);
		BoolExprAllocated.push_back(boolExpr);
		return boolExpr;
	}


	BoolExpr* MakeRelOp(RelOp::BoolCode bc, Expression* firstNumbereExpr, Expression* secondNumbereExpr) {
		BoolExpr* boolExpr = new RelOp(bc, firstNumbereExpr, secondNumbereExpr);
		BoolExprAllocated.push_back(boolExpr);
		return boolExpr;
	}


	BoolExpr* MakeBoolOpAndOr(BoolOp::BoolOpertor op, BoolExpr* firstBoolExpr, BoolExpr* secondBoolExpr) {
		BoolExpr* boolExprAndOr = new BoolOp(op, firstBoolExpr, secondBoolExpr);
		BoolExprAllocated.push_back(boolExprAndOr);
		return boolExprAndOr;
	}

	BoolExpr* MakeBoolOpNot(BoolOp::BoolOpertor op, BoolExpr* firstBoolExpr) {
		BoolExpr* boolExprNot = new BoolOp(op, firstBoolExpr);
		BoolExprAllocated.push_back(boolExprNot);
		return boolExprNot;
	}

	// -------------------------------------------------- BoolExpr making end ------------------------------------------------------


	// --------------------------------------------------- Statement making start ----------------------------------------------------------
	// Factory method per la creazione di oggetti Expression
	Statement* MakeWhileStmt(BoolExpr* boolExpre, std::vector<Statement*> stmtBlock) {
		Statement* stmt = new Whilestmt(boolExpre, std::move(stmtBlock));
		StmtAllocated.push_back(stmt);
		return stmt;
	}

	Statement* MakeIfStmt(BoolExpr* boolExpre, std::vector<Statement*> firstStmtBlock, std::vector<Statement*> secondStmtBlock) {
		Statement* stmt = new Ifstmt(boolExpre, std::move(firstStmtBlock), std::move(secondStmtBlock));
		StmtAllocated.push_back(stmt);
		return stmt;
	}

	Statement* MakeSetStmt(Expression* variableId, Expression* numExpr) {
		Statement* stmt = new SetStmt(variableId, numExpr);
		StmtAllocated.push_back(stmt);
		return stmt;
	}

	Statement* MakeInputStmt(Expression* numExprVariableId) {
		Statement* stmt = new Inputstmt(numExprVariableId);
		StmtAllocated.push_back(stmt);
		return stmt;
	}

	Statement* makePrintStmt(Expression* numExprssion) {
		Statement* stmt = new Printstmt(numExprssion);
		StmtAllocated.push_back(stmt);
		return stmt;
	}

	// --------------------------------------------------- Statement making end ----------------------------------------------------------

private:
	std::vector<Statement*> StmtAllocated;
	std::vector<Expression*> ExprAllocated;
	std::vector<BoolExpr*> BoolExprAllocated;
	std::vector<std::vector<Statement*>> BlockStmtListAllocated;
};

// ----------------------------------------------------- Expression Manager End ---------------------------------------------------
#endif
