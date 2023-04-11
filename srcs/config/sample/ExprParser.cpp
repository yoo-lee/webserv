
// Generated from ./srcs/Expr.g4 by ANTLR 4.12.0


#include "ExprListener.h"

#include "ExprParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct ExprParserStaticData final {
  ExprParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  ExprParserStaticData(const ExprParserStaticData&) = delete;
  ExprParserStaticData(ExprParserStaticData&&) = delete;
  ExprParserStaticData& operator=(const ExprParserStaticData&) = delete;
  ExprParserStaticData& operator=(ExprParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag exprParserOnceFlag;
ExprParserStaticData *exprParserStaticData = nullptr;

void exprParserInitialize() {
  assert(exprParserStaticData == nullptr);
  auto staticData = std::make_unique<ExprParserStaticData>(
    std::vector<std::string>{
      "program", "statement", "simpleStatement", "blockStatement", "directive", 
      "parameters", "parameter", "string"
    },
    std::vector<std::string>{
      "", "'and'", "'or'", "'not'", "'='", "','", "';'", "'\"'", "'{'", 
      "'}'"
    },
    std::vector<std::string>{
      "", "AND", "OR", "NOT", "EQ", "COMMA", "SEMI", "DQUOTE", "LCURLY", 
      "RCURLY", "INT", "ID", "WS", "ANYCHAR", "NUMBER"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,14,63,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,7,
  	7,7,1,0,5,0,18,8,0,10,0,12,0,21,9,0,1,1,1,1,3,1,25,8,1,1,2,1,2,3,2,29,
  	8,2,1,2,1,2,1,3,1,3,3,3,35,8,3,1,3,1,3,5,3,39,8,3,10,3,12,3,42,9,3,1,
  	3,1,3,1,4,1,4,1,5,1,5,1,5,1,5,3,5,52,8,5,1,6,1,6,1,6,3,6,57,8,6,1,7,1,
  	7,1,7,1,7,1,7,0,0,8,0,2,4,6,8,10,12,14,0,0,62,0,19,1,0,0,0,2,24,1,0,0,
  	0,4,26,1,0,0,0,6,32,1,0,0,0,8,45,1,0,0,0,10,51,1,0,0,0,12,56,1,0,0,0,
  	14,58,1,0,0,0,16,18,3,2,1,0,17,16,1,0,0,0,18,21,1,0,0,0,19,17,1,0,0,0,
  	19,20,1,0,0,0,20,1,1,0,0,0,21,19,1,0,0,0,22,25,3,4,2,0,23,25,3,6,3,0,
  	24,22,1,0,0,0,24,23,1,0,0,0,25,3,1,0,0,0,26,28,3,8,4,0,27,29,3,10,5,0,
  	28,27,1,0,0,0,28,29,1,0,0,0,29,30,1,0,0,0,30,31,5,6,0,0,31,5,1,0,0,0,
  	32,34,3,8,4,0,33,35,3,10,5,0,34,33,1,0,0,0,34,35,1,0,0,0,35,36,1,0,0,
  	0,36,40,5,8,0,0,37,39,3,2,1,0,38,37,1,0,0,0,39,42,1,0,0,0,40,38,1,0,0,
  	0,40,41,1,0,0,0,41,43,1,0,0,0,42,40,1,0,0,0,43,44,5,9,0,0,44,7,1,0,0,
  	0,45,46,5,11,0,0,46,9,1,0,0,0,47,52,3,12,6,0,48,49,3,12,6,0,49,50,3,10,
  	5,0,50,52,1,0,0,0,51,47,1,0,0,0,51,48,1,0,0,0,52,11,1,0,0,0,53,57,5,11,
  	0,0,54,57,3,14,7,0,55,57,5,14,0,0,56,53,1,0,0,0,56,54,1,0,0,0,56,55,1,
  	0,0,0,57,13,1,0,0,0,58,59,5,7,0,0,59,60,5,13,0,0,60,61,5,7,0,0,61,15,
  	1,0,0,0,7,19,24,28,34,40,51,56
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  exprParserStaticData = staticData.release();
}

}

ExprParser::ExprParser(TokenStream *input) : ExprParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

ExprParser::ExprParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  ExprParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *exprParserStaticData->atn, exprParserStaticData->decisionToDFA, exprParserStaticData->sharedContextCache, options);
}

ExprParser::~ExprParser() {
  delete _interpreter;
}

const atn::ATN& ExprParser::getATN() const {
  return *exprParserStaticData->atn;
}

std::string ExprParser::getGrammarFileName() const {
  return "Expr.g4";
}

const std::vector<std::string>& ExprParser::getRuleNames() const {
  return exprParserStaticData->ruleNames;
}

const dfa::Vocabulary& ExprParser::getVocabulary() const {
  return exprParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ExprParser::getSerializedATN() const {
  return exprParserStaticData->serializedATN;
}


//----------------- ProgramContext ------------------------------------------------------------------

ExprParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ExprParser::StatementContext *> ExprParser::ProgramContext::statement() {
  return getRuleContexts<ExprParser::StatementContext>();
}

ExprParser::StatementContext* ExprParser::ProgramContext::statement(size_t i) {
  return getRuleContext<ExprParser::StatementContext>(i);
}


size_t ExprParser::ProgramContext::getRuleIndex() const {
  return ExprParser::RuleProgram;
}

void ExprParser::ProgramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProgram(this);
}

void ExprParser::ProgramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProgram(this);
}

ExprParser::ProgramContext* ExprParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, ExprParser::RuleProgram);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(19);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ExprParser::ID) {
      setState(16);
      statement();
      setState(21);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

ExprParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ExprParser::SimpleStatementContext* ExprParser::StatementContext::simpleStatement() {
  return getRuleContext<ExprParser::SimpleStatementContext>(0);
}

ExprParser::BlockStatementContext* ExprParser::StatementContext::blockStatement() {
  return getRuleContext<ExprParser::BlockStatementContext>(0);
}


size_t ExprParser::StatementContext::getRuleIndex() const {
  return ExprParser::RuleStatement;
}

void ExprParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void ExprParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}

ExprParser::StatementContext* ExprParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 2, ExprParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(24);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(22);
      simpleStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(23);
      blockStatement();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleStatementContext ------------------------------------------------------------------

ExprParser::SimpleStatementContext::SimpleStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ExprParser::DirectiveContext* ExprParser::SimpleStatementContext::directive() {
  return getRuleContext<ExprParser::DirectiveContext>(0);
}

tree::TerminalNode* ExprParser::SimpleStatementContext::SEMI() {
  return getToken(ExprParser::SEMI, 0);
}

ExprParser::ParametersContext* ExprParser::SimpleStatementContext::parameters() {
  return getRuleContext<ExprParser::ParametersContext>(0);
}


size_t ExprParser::SimpleStatementContext::getRuleIndex() const {
  return ExprParser::RuleSimpleStatement;
}

void ExprParser::SimpleStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSimpleStatement(this);
}

void ExprParser::SimpleStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSimpleStatement(this);
}

ExprParser::SimpleStatementContext* ExprParser::simpleStatement() {
  SimpleStatementContext *_localctx = _tracker.createInstance<SimpleStatementContext>(_ctx, getState());
  enterRule(_localctx, 4, ExprParser::RuleSimpleStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(26);
    directive();
    setState(28);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 18560) != 0)) {
      setState(27);
      parameters();
    }
    setState(30);
    match(ExprParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockStatementContext ------------------------------------------------------------------

ExprParser::BlockStatementContext::BlockStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ExprParser::DirectiveContext* ExprParser::BlockStatementContext::directive() {
  return getRuleContext<ExprParser::DirectiveContext>(0);
}

tree::TerminalNode* ExprParser::BlockStatementContext::LCURLY() {
  return getToken(ExprParser::LCURLY, 0);
}

tree::TerminalNode* ExprParser::BlockStatementContext::RCURLY() {
  return getToken(ExprParser::RCURLY, 0);
}

ExprParser::ParametersContext* ExprParser::BlockStatementContext::parameters() {
  return getRuleContext<ExprParser::ParametersContext>(0);
}

std::vector<ExprParser::StatementContext *> ExprParser::BlockStatementContext::statement() {
  return getRuleContexts<ExprParser::StatementContext>();
}

ExprParser::StatementContext* ExprParser::BlockStatementContext::statement(size_t i) {
  return getRuleContext<ExprParser::StatementContext>(i);
}


size_t ExprParser::BlockStatementContext::getRuleIndex() const {
  return ExprParser::RuleBlockStatement;
}

void ExprParser::BlockStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlockStatement(this);
}

void ExprParser::BlockStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlockStatement(this);
}

ExprParser::BlockStatementContext* ExprParser::blockStatement() {
  BlockStatementContext *_localctx = _tracker.createInstance<BlockStatementContext>(_ctx, getState());
  enterRule(_localctx, 6, ExprParser::RuleBlockStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(32);
    directive();
    setState(34);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 18560) != 0)) {
      setState(33);
      parameters();
    }
    setState(36);
    match(ExprParser::LCURLY);
    setState(40);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ExprParser::ID) {
      setState(37);
      statement();
      setState(42);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(43);
    match(ExprParser::RCURLY);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DirectiveContext ------------------------------------------------------------------

ExprParser::DirectiveContext::DirectiveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ExprParser::DirectiveContext::ID() {
  return getToken(ExprParser::ID, 0);
}


size_t ExprParser::DirectiveContext::getRuleIndex() const {
  return ExprParser::RuleDirective;
}

void ExprParser::DirectiveContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDirective(this);
}

void ExprParser::DirectiveContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDirective(this);
}

ExprParser::DirectiveContext* ExprParser::directive() {
  DirectiveContext *_localctx = _tracker.createInstance<DirectiveContext>(_ctx, getState());
  enterRule(_localctx, 8, ExprParser::RuleDirective);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(45);
    match(ExprParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParametersContext ------------------------------------------------------------------

ExprParser::ParametersContext::ParametersContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ExprParser::ParameterContext* ExprParser::ParametersContext::parameter() {
  return getRuleContext<ExprParser::ParameterContext>(0);
}

ExprParser::ParametersContext* ExprParser::ParametersContext::parameters() {
  return getRuleContext<ExprParser::ParametersContext>(0);
}


size_t ExprParser::ParametersContext::getRuleIndex() const {
  return ExprParser::RuleParameters;
}

void ExprParser::ParametersContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParameters(this);
}

void ExprParser::ParametersContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParameters(this);
}

ExprParser::ParametersContext* ExprParser::parameters() {
  ParametersContext *_localctx = _tracker.createInstance<ParametersContext>(_ctx, getState());
  enterRule(_localctx, 10, ExprParser::RuleParameters);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(51);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(47);
      parameter();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(48);
      parameter();
      setState(49);
      parameters();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterContext ------------------------------------------------------------------

ExprParser::ParameterContext::ParameterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ExprParser::ParameterContext::ID() {
  return getToken(ExprParser::ID, 0);
}

ExprParser::StringContext* ExprParser::ParameterContext::string() {
  return getRuleContext<ExprParser::StringContext>(0);
}

tree::TerminalNode* ExprParser::ParameterContext::NUMBER() {
  return getToken(ExprParser::NUMBER, 0);
}


size_t ExprParser::ParameterContext::getRuleIndex() const {
  return ExprParser::RuleParameter;
}

void ExprParser::ParameterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParameter(this);
}

void ExprParser::ParameterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParameter(this);
}

ExprParser::ParameterContext* ExprParser::parameter() {
  ParameterContext *_localctx = _tracker.createInstance<ParameterContext>(_ctx, getState());
  enterRule(_localctx, 12, ExprParser::RuleParameter);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(56);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ExprParser::ID: {
        enterOuterAlt(_localctx, 1);
        setState(53);
        match(ExprParser::ID);
        break;
      }

      case ExprParser::DQUOTE: {
        enterOuterAlt(_localctx, 2);
        setState(54);
        string();
        break;
      }

      case ExprParser::NUMBER: {
        enterOuterAlt(_localctx, 3);
        setState(55);
        match(ExprParser::NUMBER);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StringContext ------------------------------------------------------------------

ExprParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ExprParser::StringContext::DQUOTE() {
  return getTokens(ExprParser::DQUOTE);
}

tree::TerminalNode* ExprParser::StringContext::DQUOTE(size_t i) {
  return getToken(ExprParser::DQUOTE, i);
}

tree::TerminalNode* ExprParser::StringContext::ANYCHAR() {
  return getToken(ExprParser::ANYCHAR, 0);
}


size_t ExprParser::StringContext::getRuleIndex() const {
  return ExprParser::RuleString;
}

void ExprParser::StringContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterString(this);
}

void ExprParser::StringContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitString(this);
}

ExprParser::StringContext* ExprParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 14, ExprParser::RuleString);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(58);
    match(ExprParser::DQUOTE);
    setState(59);
    match(ExprParser::ANYCHAR);
    setState(60);
    match(ExprParser::DQUOTE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void ExprParser::initialize() {
  ::antlr4::internal::call_once(exprParserOnceFlag, exprParserInitialize);
}
