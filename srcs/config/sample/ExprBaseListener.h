
// Generated from ./srcs/Expr.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "ExprListener.h"


/**
 * This class provides an empty implementation of ExprListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  ExprBaseListener : public ExprListener {
public:

  virtual void enterProgram(ExprParser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(ExprParser::ProgramContext * /*ctx*/) override { }

  virtual void enterStatement(ExprParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(ExprParser::StatementContext * /*ctx*/) override { }

  virtual void enterSimpleStatement(ExprParser::SimpleStatementContext * /*ctx*/) override { }
  virtual void exitSimpleStatement(ExprParser::SimpleStatementContext * /*ctx*/) override { }

  virtual void enterBlockStatement(ExprParser::BlockStatementContext * /*ctx*/) override { }
  virtual void exitBlockStatement(ExprParser::BlockStatementContext * /*ctx*/) override { }

  virtual void enterDirective(ExprParser::DirectiveContext * /*ctx*/) override { }
  virtual void exitDirective(ExprParser::DirectiveContext * /*ctx*/) override { }

  virtual void enterParameters(ExprParser::ParametersContext * /*ctx*/) override { }
  virtual void exitParameters(ExprParser::ParametersContext * /*ctx*/) override { }

  virtual void enterParameter(ExprParser::ParameterContext * /*ctx*/) override { }
  virtual void exitParameter(ExprParser::ParameterContext * /*ctx*/) override { }

  virtual void enterString(ExprParser::StringContext * /*ctx*/) override { }
  virtual void exitString(ExprParser::StringContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

