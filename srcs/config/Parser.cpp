#include "Parser.hpp"

Parser::Parser(std::string file_text)
{
    // Lexer lexer(file_text);
    (void)file_text;
    Lexer lexer("simple statement;");
    std::vector<Token> tokens = lexer.get_token_list();
    _tree = AST(tokens);
}

Parser::~Parser()
{
}

void Parser::print()
{
    _tree.print_tree();
}

std::vector<Statement *> Parser::get_root()
{
    return _tree.get_root();
};

HTTP Parser::get_https() const
{

}

#ifdef TEST
int main()
{
    Parser parser("simple statement;");
    return 0;
}
#endif
