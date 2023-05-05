OBJ_DIR = objs

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
				 Http.cpp \
				 LimitExcept.cpp

SRCS = ${UNIT_TEST_SRCS}
UNIT_OBJS = $(addprefix $(OBJ_DIR)/,$(notdir ${UNIT_TEST_SRCS:.cpp=.o}))
OBJS = $(addprefix $(OBJ_DIR)/,$(notdir ${SRCS:.cpp=.o}))
CXX = c++
CXXFLAGS = -Wall -Wextra -std=c++98 -g3

utest: objs $(UNIT_OBJS)
	$(CXX) $(UNIT_OBJS) -std=c++11 -fsanitize=address -o test

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -std=c++11 -g3 -D UNIT_TEST -c $< -o $@

run:
	./test

clean:
	- rm $(OBJ_DIR)/*.o

fclean:
	- rm test

objs:
	mkdir -p $(OBJ_DIR)

utr: objs utest run