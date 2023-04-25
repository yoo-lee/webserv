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

ASTNode *Parser::get_root()
{
    return _tree.get_root();
};

HTTP Parser::get_https() const
{
    NonTerminalASTNode *root = _tree.get_root();
    std::vector<ASTNode *> children = root->get_children();
    for (size_t i = 0; i < children.size(); i++)
    {
        if (dynamic_cast<TerminalASTNode *>(children[i]) != 0)
            continue;
        NonTerminalASTNode *node = dynamic_cast<NonTerminalASTNode *>(children[0]);
        if (node[ASTNode::STATEMENT][ASTNode::DIRECTIVE][ASTNode::ID])
        {
            return dynamic_cast<HTTP *>(children[i]);
        }
    }
}

#ifdef TEST
int main()
{
    Parser parser("simple statement;");
    return 0;
}
#endif
