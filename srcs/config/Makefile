SRCS = Config.cpp Parser.cpp AST.cpp ASTNode.cpp Lexer.cpp Token.cpp
OBJS = ${SRCS:.cpp=.o}
CXX = g++
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

utest: Lexer.cpp Token.cpp
	${CXX} Lexer.cpp Token.cpp -std=c++11 -g3 -D UNIT_TEST -o test

utr: utest run
