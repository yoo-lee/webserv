#include "ASTNode.hpp"
ASTNode::Type ASTNode::TokenTypeToASTNodeType(Token::Type type) {
    switch (type) {
        case Token::ID:
            return ASTNode::ID;
        case Token::STRING:
            return ASTNode::STRING;
        case Token::INT:
            return ASTNode::INT;
        case Token::SEMI:
            return ASTNode::SEMI;
        case Token::LCURLY:
            return ASTNode::LCURLY;
        case Token::RCURLY:
            return ASTNode::RCURLY;
        default:
            throw std::runtime_error(
                "Unknown token type converted to AST node");
    }
}

std::string ASTNode::ASTNodeTypeToStr(Type type) {
    switch (type) {
        case PROGRAM:
            return "PROGRAM";
        case STATEMENT:
            return "STATEMENT";
        case SIMPLE_STATEMENT:
            return "SIMPLE_STATEMENT";
        case BLOCK_STATEMENT:
            return "BLOCK_STATEMENT";
        case DIRECTIVE:
            return "DIRECTIVE";
        case PARAMETERS:
            return "PARAMETERS";
        case PARAMETER:
            return "PARAMETER";
        case ID:
            return "ID";
        case STRING:
            return "STRING";
        case INT:
            return "INT";
        case SEMI:
            return "SEMI";
        case LCURLY:
            return "LCURLY";
        case RCURLY:
            return "RCURLY";
        default:
            return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, const ASTNode& node) {
    os << "ASTNode(" << (node.getType()) << ", " << node.getValue() << ")";
    return os;
}

ASTNode::Type ASTNode::getType() const {
    return _type;
}

std::string ASTNode::getValue() const {
    return _value;
}

ASTNode::ASTNode(ASTNode::Type type, std::vector<ASTNode*> children)
    : _type(type), _children(children), _value("") {}

ASTNode::ASTNode(ASTNode::Type type, std::string value)
    : _type(type), _value(value) {
    _children = std::vector<ASTNode*>();
}

ASTNode::ASTNode(ASTNode::Type type, ASTNode* child) : _type(type), _value("") {
    _children = std::vector<ASTNode*>();
    _children.push_back(child);
}

ASTNode::ASTNode(ASTNode* at_node)
    : _type(at_node->_type), _value(at_node->_value) {
    _children = std::vector<ASTNode*>();
    for (std::size_t i = 0; i < at_node->_children.size(); ++i) {
        _children.push_back(new ASTNode(at_node->_children[i]));
    }
}

ASTNode::~ASTNode() {
    for (std::size_t i = 0; i < _children.size(); ++i) {
        delete _children[i];
    }
}