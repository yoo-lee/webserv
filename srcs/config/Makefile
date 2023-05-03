UNIT_TEST_SRCS = AST.cpp \
				 Lexer.cpp \
				 Token.cpp \
				 NotFound.cpp \
				 SyntaxError.cpp \
				 InvalidValue.cpp \
				 BlockStatement.cpp \
				 Statement.cpp \
				 SimpleStatement.cpp \
				 Location.cpp \
				 Parser.cpp \
				 Server.cpp \
				 utils.cpp \
				 Config.cpp \
				 Http.cpp
SRCS = ${UNIT_TEST_SRCS}
UNIT_OBJS = ${UNIT_TEST_SRCS:.cpp=.o}
OBJS = ${SRCS:.cpp=.o}
CXX = c++
CXXFLAGS = -Wall -Wextra -std=c++98 -g3

test: ${SRCS}
	${CXX} ${SRCS} ${CXXFLAGS} -D TEST -o test

run:
	./test

tr: test run

%.o: %.cpp
	${CXX} -std=c++11 -g3 -D UNIT_TEST -c $< -o $@

re : ${SRCS}
	- rm test
	${CXX} ${SRCS} ${CXXFLAGS} -D TEST -o test
	# ./test

utest: ${UNIT_OBJS}
	${CXX} ${UNIT_OBJS} -std=c++11 -o test

clean:
	- rm *.o

fclean:
	- rm test

utr: utest run
