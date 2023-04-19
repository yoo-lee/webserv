SRCS = Config.cpp Parser.cpp AST.cpp ASTNode.cpp Lexer.cpp Token.cpp
OBJS = ${SRCS:.cpp=.o}
CXX = g++
CXXFLAGS = -Wall -Wextra  -std=c++98 -g3
test: ${SRCS}
	${CXX} ${SRCS} ${CXXFLAGS} -D TEST -o test
	./test

run:
	./test

tr: test run

re :
	- rm test
	${CXX} ${SRCS} ${CXXFLAGS} -D TEST -o test
	# ./test

utest: ${SRCS}
	${CXX} ${SRCS} -g3 -D UNIT_TEST -o test

utr: utest run
