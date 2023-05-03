#include "Parser.hpp"

#ifdef UNIT_TEST
#include "doctest.h"
#endif

Parser::Parser(std::string file_text) : _tree(Lexer(file_text).get_token_list()) {}

Parser::~Parser() {}

void Parser::print()
{
    _tree.print_tree();
}

std::vector<Statement *> const &Parser::get_root()
{
    return _tree.get_root();
};

#ifdef UNIT_TEST
TEST_CASE("Parser: constructor and get_root test: simple statement")
{
    Parser parser("simple statement;");
    std::vector<Statement *> root = parser.get_root();
    CHECK(root.size() == 1);
    CHECK(root[0]->get_directive() == "simple");
    CHECK(root[0]->get_params().size() == 1);
    CHECK(root[0]->get_params()[0] == "statement");
}

TEST_CASE("Parser: get_root test: statement having child statements")
{
    Parser parser("statement { a child1; a child2; }");
    std::vector<Statement *> root = parser.get_root();
    CHECK(root.size() == 1);
    CHECK(root[0]->get_directive() == "statement");
    CHECK(root[0]->get_params().size() == 0);
    BlockStatement *block = dynamic_cast<BlockStatement *>(root[0]);
    std::vector<Statement *> children = block->get_children();
    CHECK(children.size() == 2);
    CHECK(children[0]->get_directive() == "a");
    CHECK(children[0]->get_params().size() == 1);
    CHECK(children[0]->get_params()[0] == "child1");
    CHECK(children[1]->get_directive() == "a");
    CHECK(children[1]->get_params().size() == 1);
    CHECK(children[1]->get_params()[0] == "child2");
}
#endif