SRCS = Config.cpp Lexer.cpp Token.cpp
OBJS = ${SRCS:.cpp=.o}
CXX = g++
test: ${SRCS}
	${CXX} ${SRCS} -D TEST -o test
	./test
re :
	- rm test
	${CXX} ${SRCS} -D TEST -o test
	./test
