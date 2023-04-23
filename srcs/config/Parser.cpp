#include "Parser.hpp"

Parser::Parser(std::string file_text)
{
    // Lexer lexer(file_text);
    (void)file_text;
    Lexer lexer("simple statement;");
    std::vector<Token> tokens = lexer.getTokenList();
    _tree = AST(tokens);
    print();
}

Parser::~Parser()
{
}

void Parser::print()
{
    _tree.print_tree();
}

#ifdef TEST
int main()
{
    Parser parser("simple statement;");
    return 0;
}
#endif
