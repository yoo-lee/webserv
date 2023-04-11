
// Generated from ./srcs/Expr.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "ExprParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by ExprParser.
 */
class  ExprListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(ExprParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(ExprParser::ProgramContext *ctx) = 0;

  virtual void enterStatement(ExprParser::StatementContext *ctx) = 0;
  virtual void exitStatement(ExprParser::StatementContext *ctx) = 0;

  virtual void enterSimpleStatement(ExprParser::SimpleStatementContext *ctx) = 0;
  virtual void exitSimpleStatement(ExprParser::SimpleStatementContext *ctx) = 0;

  virtual void enterBlockStatement(ExprParser::BlockStatementContext *ctx) = 0;
  virtual void exitBlockStatement(ExprParser::BlockStatementContext *ctx) = 0;

  virtual void enterDirective(ExprParser::DirectiveContext *ctx) = 0;
  virtual void exitDirective(ExprParser::DirectiveContext *ctx) = 0;

  virtual void enterParameters(ExprParser::ParametersContext *ctx) = 0;
  virtual void exitParameters(ExprParser::ParametersContext *ctx) = 0;

  virtual void enterParameter(ExprParser::ParameterContext *ctx) = 0;
  virtual void exitParameter(ExprParser::ParameterContext *ctx) = 0;

  virtual void enterString(ExprParser::StringContext *ctx) = 0;
  virtual void exitString(ExprParser::StringContext *ctx) = 0;


};

