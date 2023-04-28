#include "BlockStatement.hpp"
#include <string>
#include <vector>
#ifdef UNIT_TEST
#include "doctest.h"
#endif

BlockStatement::BlockStatement(std::string directive, std::vector<std::string> params,
                               std::vector<Statement *> child_statements)
    : Statement(directive, params),
      _child_statements(child_statements)
{
}

BlockStatement::BlockStatement(std::string directive, std::vector<Statement *> child_statements)
    : Statement(directive),
      _child_statements(child_statements)
{
}

BlockStatement::BlockStatement(const BlockStatement &b) : Statement(b)
{
    for (size_t i = 0; i < b._child_statements.size(); i++)
    {
        _child_statements.push_back(new Statement(*b._child_statements[i]));
    }
}
// #include <iostream>
BlockStatement::~BlockStatement()
{
    for (size_t i = 0; i < _child_statements.size(); i++)
        delete _child_statements[i];
}

void BlockStatement::print(std::ostream &os, std::string indent) const
{
    Statement::print(os, indent);
    os << indent << "{\n";
    for (size_t i = 0; i < _child_statements.size(); i++)
    {
        if (_child_statements[i] == 0)
            os << indent << "NULL"
               << "\n";
        else if (dynamic_cast<BlockStatement *>(_child_statements[i]))
        {
            dynamic_cast<BlockStatement *>(_child_statements[i])->print(os, indent + "  ");
            os << "\n";
        }
        else
            os << indent << "  " << *_child_statements[i] << "\n";
    }
    os << indent << "}";
}

std::vector<Statement *> BlockStatement::get_child_statements() const
{
    return _child_statements;
}

std::vector<Statement *> BlockStatement::get_child_statements(std::string directive) const
{
    std::vector<Statement *> result;
    for (size_t i = 0; i < _child_statements.size(); i++)
    {
        if (_child_statements[i]->get_directive() == directive)
            result.push_back(_child_statements[i]);
    }
    return result;
}

std::ostream &operator<<(std::ostream &os, const BlockStatement &statement)
{
    statement.print(os, "");
    return os;
}

// 複数要素があった場合、要素が見つからなかった場合、例外を投げる
Statement *BlockStatement::operator[](std::string directive) const
{
    Statement *result = 0;
    for (size_t i = 0; i < _child_statements.size(); i++)
    {
        if (_child_statements[i]->get_directive() == directive)
        {
            if (result != 0)
                throw std::out_of_range("multiple statements(" + directive + ") with same directive");
            result = _child_statements[i];
        }
    }
    if (result == 0)
        throw std::out_of_range("no statement with directive: " + directive);
    return result;
}

#ifdef UNIT_TEST
TEST_CASE("BlockStatement constructor")
{
    std::vector<std::string> params;
    params.push_back("param1");
    params.push_back("param2");

    std::vector<Statement *> child_statements;
    Statement *s1 = new Statement("directive1", "value1");
    Statement *s2 = new Statement("directive2", "value2");
    child_statements.push_back(s1);
    child_statements.push_back(s2);

    BlockStatement b("directive", params, child_statements);
    CHECK(b.get_directive() == "directive");
    CHECK(b.get_params().size() == 2);
    CHECK(b.get_params()[0] == "param1");
    CHECK(b.get_params()[1] == "param2");
    CHECK(b.get_child_statements().size() == 2);
    CHECK(b.get_child_statements()[0] == s1);
    CHECK(b.get_child_statements()[1] == s2);
}

TEST_CASE("BlockStatement copy constructor")
{
    std::vector<std::string> params;
    params.push_back("param1");
    params.push_back("param2");

    std::vector<Statement *> child_statements;
    Statement *s1 = new Statement("directive1", "value1");
    Statement *s2 = new Statement("directive2", "value2");
    child_statements.push_back(s1);
    child_statements.push_back(s2);

    BlockStatement b("directive", params, child_statements);
    BlockStatement b2(b);
    CHECK(b2.get_directive() == "directive");
    CHECK(b2.get_params().size() == 2);
    CHECK(b2.get_params()[0] == "param1");
    CHECK(b2.get_params()[1] == "param2");
    CHECK(&(b.get_params()[0]) != &(b2.get_params()[0]));
    CHECK(&(b.get_params()[1]) != &(b2.get_params()[1]));
    CHECK(b2.get_child_statements().size() == 2);
    CHECK(b2.get_child_statements()[0] != s1);
    CHECK(b2.get_child_statements()[1] != s2);
    CHECK(&(b.get_child_statements()[0]) != &(b2.get_child_statements()[0]));
    CHECK(&(b.get_child_statements()[1]) != &(b2.get_child_statements()[1]));
}

TEST_CASE("BlockStatement nested")
{
    std::vector<Statement *> grandchild;
    Statement *s1 = new Statement("directive1", "value1");
    grandchild.push_back(s1);
    BlockStatement *child = new BlockStatement("directive2", std::vector<std::string>(), grandchild);
    std::vector<Statement *> children;
    children.push_back(child);

    BlockStatement b("directive", children);
}

TEST_CASE("BlockStatement []")
{
    std::vector<std::string> params;
    params.push_back("param1");
    params.push_back("param2");

    std::vector<Statement *> child_statements;
    Statement *s1 = new Statement("directive1", "value1");
    Statement *s2 = new Statement("directive2", "value2");
    child_statements.push_back(s1);
    child_statements.push_back(s2);

    BlockStatement b("directive", params, child_statements);
    CHECK(b["directive1"] == s1);
    CHECK(b["directive2"] == s2);
    CHECK_THROWS_AS(b["directive3"], std::out_of_range);
}

#endif
