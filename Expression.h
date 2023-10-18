#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <utility>
#include <vector>
#include "Exceptions.h"

class Visitor; // The Visitor class is declared, but the actual implementation is in the file Expression.cpp.

class Expression {
public:
	virtual ~Expression() = default;
	virtual void Accept(Visitor* V) = 0;
};


class Number : public Expression {
public:
	explicit Number(long value) : Value{ value } {}
	~Number() override = default;
	void Accept(Visitor* v) override;
	long GetValue() const {
		return Value;
	}
private:
	long Value;
};


class Variable : public Expression {
public:
	~Variable() override = default;
	explicit Variable(std::string varName) : VarName{std::move(varName)} {}

	const std::string& GetValue() const {
		return VarName;
	}
	void Accept(Visitor* v) override;
private:
	std::string VarName;
};


class Operator : public Expression {
public:
	enum OpCode { ADD=4, SUB, MUL, DIV };
	Operator(OpCode op, Expression* left, Expression* right) : Left{left}, Right{right}, Op{op} {}
	//Operator(OpCode op, Expression* left, Expression* right) : Op {op}, Left{left}, Right{right}{}
	~Operator() override = default;
	Operator() = default;
	Operator(const Operator& other) = default;
	Operator& operator=(const Operator& other) = default;

	OpCode GetOpCode() {
		return Op;
	}
	Expression* GetLeft() {
		return Left;
	}
	Expression* GetRight() {
		return Right;
	}

	static OpCode TokenWordToOpCode(int tag) {
		switch (tag)
		{
		case 4: return OpCode::ADD;
		case 5: return OpCode::SUB;
		case 6: return OpCode::MUL;
		case 7: return OpCode::DIV;
		default: throw SintaticError("(ERROR it should be 4,5,6 or 7!!!)");
		}
	}



	void Accept(Visitor* v) override;

private:
	Expression* Left;
	Expression* Right;
	OpCode Op;
};


// -------------------------------------------------------------------------------------------- Bool --------------------------------

class BoolExpr {
public:
	virtual ~BoolExpr() = default;
	virtual void Accept(Visitor* V) = 0;
};


class BoolConst : public BoolExpr {
public:

	~BoolConst() override = default;
	BoolConst& operator=(const BoolConst& other) = default;

	explicit BoolConst(int b) {
		if (b == 20) {
			boolConst = true;
		}
		else if (b == 21) {
			boolConst = false;
		}
		else {
			throw SintaticError("(ERROR it should be 20 or 21!!!)");
		}

	}

	bool GetBool() const{
		return boolConst;
	}

	void Accept(Visitor* v) override;

private:
	bool boolConst;
};


class RelOp : public BoolExpr {
public:
	enum BoolCode { GT = 14, LT, EQ };
	~RelOp() override = default;
	RelOp(BoolCode ex, Expression* first, Expression* second) : BC{ ex }, FirstNumberExpr{ first }, SecondNumberExpr{ second } {}

	BoolCode GetBoolCode() {
		return BC;
	}

	Expression* GetFirst() {
		return FirstNumberExpr;
	}

	Expression* GetSecond() {
		return SecondNumberExpr;
	}

	void Accept(Visitor* v) override;

private:

	BoolCode BC;
	Expression* FirstNumberExpr;
	Expression* SecondNumberExpr;
};


class BoolOp : public BoolExpr {
public:
	enum BoolOpertor { AND = 17, OR, NOT };
	~BoolOp() override = default;
	BoolOp() = default;
	BoolOp(BoolOpertor op, BoolExpr* firstBoolExpr, BoolExpr* secondBoolExpr)
		: OP{ op }, FirstBoolExpr{ firstBoolExpr }, SecondBoolExpr{ secondBoolExpr } {}
	BoolOp(BoolOpertor op, BoolExpr* firstBoolExpr) : OP{ op }, FirstBoolExpr{ firstBoolExpr }, SecondBoolExpr{ nullptr } {}

	BoolExpr* GetFirstBoolExpr() const{
		return FirstBoolExpr;
	}

	BoolExpr* GetSecondBoolExpr() const {
		return SecondBoolExpr;
	}

	BoolOpertor GetBoolOperator() const {
		return OP;
	}


	void Accept(Visitor* v) override;


private:
	BoolOpertor OP;
	BoolExpr* FirstBoolExpr;
	BoolExpr* SecondBoolExpr;
};

// -------------------------------------------------------------------------------------------- BOOl finished ----------------------------

// ------------------------------------------------------------------------------------ Statement start -----------------------------------
class Statement {
public:
	virtual ~Statement() = default;
	virtual void Accept(Visitor* V) = 0;
};



class Ifstmt : public Statement {
public:
	~Ifstmt() override = default;
	Ifstmt(BoolExpr* boolexpr, std::vector<Statement*> firstStmtBlock, std::vector<Statement*> secondStmtBlock)
		: BoolExpre{ boolexpr }, FirstStmtBlock{std::move(firstStmtBlock)}, SecondStmtBlock{std::move(secondStmtBlock)} {}
	void Accept(Visitor* v) override;

	BoolExpr* GetBoolExpr() {
		return BoolExpre;

	}

	std::vector<Statement*> GetFirstStmtBlock() {
		return FirstStmtBlock;

	}

	std::vector<Statement*> GetSecondStmtBlock() {
		return SecondStmtBlock;

	}

private:
	BoolExpr* BoolExpre;
	std::vector<Statement*> FirstStmtBlock;
	std::vector<Statement*> SecondStmtBlock;
};



class Whilestmt : public Statement {
public:
	~Whilestmt() override = default;
	Whilestmt(BoolExpr* boolExpre, std::vector<Statement*> stmtBlock) {
		BoolExpre = boolExpre;
		StmtBlock = std::move(stmtBlock);
	}

	void Accept(Visitor* v) override;


	BoolExpr* GetBoolExpr() {
		return BoolExpre;
	}


	std::vector<Statement*> GetStmtBlock() {
		return StmtBlock;
	}


private:
	BoolExpr* BoolExpre;
	std::vector<Statement*> StmtBlock;
};

class SetStmt : public Statement {
public:

	SetStmt(Expression* variableId, Expression* numExpr) : VariableId{ variableId }, NumExpr{ numExpr } {}

	void Accept(Visitor* v) override;

	Expression* GetVariableId() {
		return VariableId;
	}
	Expression* GetNumExpr() {
		return NumExpr;
	}
private:
	Expression* VariableId;
	Expression* NumExpr;
};



class Inputstmt : public Statement {
public:
	~Inputstmt() override = default;
	Inputstmt(Expression* numExprVariableId) : NumExprVariableId{ numExprVariableId } {}
	void Accept(Visitor* v) override;

	Expression* GetNumExprVariableId() {
		return NumExprVariableId;
	}

private:
	Expression* NumExprVariableId;
};



class Printstmt : public Statement {
public:
	~Printstmt() override = default;
	explicit Printstmt(Expression* numExpression) : NumExprssion{ numExpression } {}

	void Accept(Visitor* v) override;

	Expression* GetNumExprssion() {
		return NumExprssion;
	}

private:
	Expression* NumExprssion;
};

// ------------------------------------------------------------------------------------ Statement ends -----------------------------------

// ----------------------------------------------------- Expression Manager Start ---------------------------------------------------




#endif
