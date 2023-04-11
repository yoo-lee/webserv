grammar Expr;

program : statement* ;
statement : simpleStatement
          | blockStatement
          ;
simpleStatement : directive parameters? SEMI ;
blockStatement : directive parameters? LCURLY statement* RCURLY ;
directive : ID ;
parameters : parameter
           | parameter parameters
           ;
parameter : ID
          | string
          | INT
          ;
string : DQUOTE ANYCHAR DQUOTE ;

COMMA : ',' ;
SEMI : ';' ;
DQUOTE : '"' ;
LCURLY : '{' ;
RCURLY : '}' ;
INT : [0-9]+ ;
ID: [a-zA-Z_][a-zA-Z_0-9]* ;
WS: [ \t\n\r\f]+ -> skip ;
ANYCHAR : [a-zA-Z_0-9]+ ;