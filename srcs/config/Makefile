SRCS = Config.cpp Lexer.cpp Token.cpp
OBJS = ${SRCS:.cpp=.o}
CXX = g++
CXXFLAGS = -Wall -Wextra  -std=c++98 -fsanitize=address -g3
test: ${SRCS}
	${CXX} ${SRCS} ${CXXFLAGS} -D TEST -o test
	./test

re :
	- rm test
	${CXX} ${SRCS} ${CXXFLAGS} -D TEST -o test
	./test

utest: ${SRCS}
	${CXX} ${SRCS} -D UNIT_TEST -o test
	./test