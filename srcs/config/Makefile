UNIT_TEST_SRCS = Lexer.cpp Token.cpp ASTNode.cpp AST.cpp NotFound.cpp SyntaxError.cpp NonTerminalASTNode.cpp TerminalASTNode.cpp InvalidValue.cpp
SRCS = Config.cpp Parser.cpp ${UNIT_TEST_SRCS}
OBJS = ${SRCS:.cpp=.o}
CXX = c++
CXXFLAGS = -Wall -Wextra -std=c++98 -g3

test: ${SRCS}
	${CXX} ${SRCS} ${CXXFLAGS} -D TEST -o test

run:
	./test

tr: test run

re : ${SRCS}
	- rm test
	${CXX} ${SRCS} ${CXXFLAGS} -D TEST -o test
	# ./test

utest: ${UNIT_TEST_SRCS}
	${CXX} ${UNIT_TEST_SRCS} -std=c++11 -g3 -D UNIT_TEST -o test

utr: utest run
