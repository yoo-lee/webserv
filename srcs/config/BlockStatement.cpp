#include "BlockStatement.hpp"
#include "SimpleStatement.hpp"
#include <string>
#include <vector>
#ifdef UNIT_TEST
#include "doctest.h"
#endif

using std::ostream;
using std::out_of_range;

BlockStatement::BlockStatement(string directive, vector<string> params, vector<Statement const*> child_statements)
    : Statement(directive, params),
      _child_statements(child_statements)
{
}

BlockStatement::BlockStatement(string directive, vector<Statement const*> child_statements)
    : Statement(directive),
      _child_statements(child_statements)
{
}

BlockStatement::BlockStatement(const BlockStatement& b) : Statement(b)
{
    for (size_t i = 0; i < b._child_statements.size(); i++)
        _child_statements.push_back(b._child_statements[i]->clone());
}

BlockStatement::BlockStatement(Statement const* s) : Statement(*s)
{
    if (dynamic_cast<BlockStatement const*>(s))
        throw SyntaxError("BlockStatement: Taken statement is not a BlockStatement");
    BlockStatement const* b = dynamic_cast<BlockStatement const*>(s);
    for (size_t i = 0; i < b->_child_statements.size(); i++)
        _child_statements.push_back((new SimpleStatement(b->_child_statements[i]))->clone());
}

BlockStatement::~BlockStatement()
{
    for (size_t i = 0; i < _child_statements.size(); i++)
        delete _child_statements[i];
}

void BlockStatement::print(ostream& os, string indent) const
{
    Statement::print(os, indent);
    os << indent << "{\n";
    for (size_t i = 0; i < _child_statements.size(); i++) {
        if (_child_statements[i] == 0)
            os << indent << "NULL"
               << "\n";
        else if (dynamic_cast<BlockStatement const*>(_child_statements[i])) {
            dynamic_cast<BlockStatement const*>(_child_statements[i])->print(os, indent + "  ");
            os << "\n";
        } else
            os << indent << "  " << *_child_statements[i] << "\n";
    }
    os << indent << "}";
}

vector<Statement const*> BlockStatement::get_children() const
{
    return _child_statements;
}

vector<SimpleStatement const*> BlockStatement::get_simple_statement_children() const
{
    vector<SimpleStatement const*> simple_children;
    for (size_t i = 0; i < _child_statements.size(); i++) {
        if (dynamic_cast<SimpleStatement const*>(_child_statements[i]))
            simple_children.push_back(dynamic_cast<SimpleStatement const*>(_child_statements[i]));
    }
    return simple_children;
}

vector<BlockStatement const*> BlockStatement::get_block_statement_children() const
{
    vector<BlockStatement const*> block_children;
    for (size_t i = 0; i < _child_statements.size(); i++) {
        if (dynamic_cast<BlockStatement const*>(_child_statements[i]))
            block_children.push_back(dynamic_cast<BlockStatement const*>(_child_statements[i]));
    }
    return block_children;
}

vector<Statement const*> BlockStatement::get_children(string directive) const
{
    vector<Statement const*> result;
    for (size_t i = 0; i < _child_statements.size(); i++) {
        if (_child_statements[i]->get_directive() == directive)
            result.push_back(_child_statements[i]);
    }
    return result;
}

ostream& operator<<(ostream& os, const BlockStatement& statement)
{
    statement.print(os, "");
    return os;
}

// 複数要素があった場合、要素が見つからなかった場合、例外を投げる
Statement const* BlockStatement::operator[](string directive) const
{
    Statement const* result = 0;
    for (size_t i = 0; i < _child_statements.size(); i++) {
        if (_child_statements[i]->get_directive() == directive) {
            if (result != 0)
                throw out_of_range("BlockStatement[]: Multiple statements(" + directive + ") with same directive");
            result = _child_statements[i];
        }
    }
    if (result == 0)
        throw out_of_range("BlockStatement[]: No statement with directive: " + directive);
    return result;
}

Statement* BlockStatement::clone() const
{
    return new BlockStatement(*this);
}

#ifdef UNIT_TEST
TEST_CASE("BlockStatement: get_children and get_x_statement_children")
{
    vector<string> params;
    params.push_back("param1");
    params.push_back("param2");

    vector<Statement const*> child_statements;
    SimpleStatement const* s1 = new SimpleStatement("directive1", "value1");
    SimpleStatement const* s2 = new SimpleStatement("directive2", "value2");
    BlockStatement const* b1 = new BlockStatement("directive3", vector<string>(), vector<Statement const*>());
    child_statements.push_back(s1);
    child_statements.push_back(s2);
    child_statements.push_back(b1);

    BlockStatement b("directive", params, child_statements);
    vector<Statement const*> children = b.get_children();
    CHECK(children.size() == 3);
    CHECK(children[0] == s1);
    CHECK(children[1] == s2);
    CHECK(children[2] == b1);

    vector<SimpleStatement const*> simple_children = b.get_simple_statement_children();
    CHECK(simple_children.size() == 2);
    CHECK(simple_children[0] == s1);
    CHECK(simple_children[1] == s2);

    vector<BlockStatement const*> block_children = b.get_block_statement_children();
    CHECK(block_children.size() == 1);
    CHECK(block_children[0] == b1);
}

TEST_CASE("BlockStatement constructor")
{
    vector<string> params;
    params.push_back("param1");
    params.push_back("param2");

    vector<Statement const*> child_statements;
    SimpleStatement const* s1 = new SimpleStatement("directive1", "value1");
    SimpleStatement const* s2 = new SimpleStatement("directive2", "value2");
    child_statements.push_back(s1);
    child_statements.push_back(s2);

    BlockStatement b("directive", params, child_statements);
    CHECK(b.get_directive() == "directive");
    CHECK(b.get_params().size() == 2);
    CHECK(b.get_params()[0] == "param1");
    CHECK(b.get_params()[1] == "param2");
    CHECK(b.get_children().size() == 2);
    CHECK(b.get_children()[0] == s1);
    CHECK(b.get_children()[1] == s2);
}

TEST_CASE("BlockStatement copy constructor")
{
    vector<string> params;
    params.push_back("param1");
    params.push_back("param2");

    vector<Statement const*> child_statements;
    SimpleStatement const* s1 = new SimpleStatement("directive1", "value1");
    SimpleStatement const* s2 = new SimpleStatement("directive2", "value2");
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
    CHECK(b2.get_children().size() == 2);
    CHECK(b2.get_children()[0] != s1);
    CHECK(b2.get_children()[1] != s2);
    CHECK(&(b.get_children()[0]) != &(b2.get_children()[0]));
    CHECK(&(b.get_children()[1]) != &(b2.get_children()[1]));
}

TEST_CASE("BlockStatement nested")
{
    vector<Statement const*> grandchild;
    SimpleStatement const* s1 = new SimpleStatement("directive1", "value1");
    grandchild.push_back(s1);
    BlockStatement const* child = new BlockStatement("directive2", vector<string>(), grandchild);
    vector<Statement const*> children;
    children.push_back(child);

    BlockStatement b("directive", children);
}

TEST_CASE("BlockStatement []")
{
    vector<string> params;
    params.push_back("param1");
    params.push_back("param2");

    vector<Statement const*> child_statements;
    SimpleStatement const* s1 = new SimpleStatement("directive1", "value1");
    SimpleStatement const* s2 = new SimpleStatement("directive2", "value2");
    child_statements.push_back(s1);
    child_statements.push_back(s2);

    BlockStatement b("directive", params, child_statements);
    CHECK(b["directive1"] == s1);
    CHECK(b["directive2"] == s2);
    CHECK_THROWS_AS(b["directive3"], out_of_range);
}

#endif
