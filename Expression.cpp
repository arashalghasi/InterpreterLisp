#include "Expression.h"
#include "Visitor.h"

void Number::Accept(Visitor* v) {
	v->visitNumberExpr(this);
}

void Operator::Accept(Visitor* v) {
	v->visitOperatorExpr(this);
}

void Variable::Accept(Visitor* v) {
	v->visitVariableExpr(this);
}

void BoolConst::Accept(Visitor* v)  {
	v->visitBoolConstBoolExpr(this);
}

void RelOp::Accept(Visitor* v)  {
	v->visitRelOpBoolExpr(this);
}

void BoolOp::Accept(Visitor* v)  {
	v->visitBoolOpBoolExpr(this);
}

void Ifstmt::Accept(Visitor* v)  {
	v->visitIfStmt(this);
}

void Whilestmt::Accept(Visitor* v) {
	v->visitWhileStmt(this);
}

void SetStmt::Accept(Visitor* v)  {
	v->visitSetStmt(this);
}

void Inputstmt::Accept(Visitor* v)  {
	v->visitInputStmt(this);
}

void Printstmt::Accept(Visitor* v)  {
	v->visitPrintStmt(this);
}