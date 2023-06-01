NAME	:= webserv
LIB		:= ./lib
OBJDIR	:= ./obj
SRCDIR	:= ./srcs
CFGDIR	:= config

CONFIG := \
				 AST.cpp \
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
CONFIG_SRCS = $(addprefix $(CFGDIR)/,$(CONFIG))
SOCKET 				:= fd_manager.cpp socket_data.cpp request.cpp response.cpp socket.cpp tcp_socket.cpp content_type.cpp transfer_encoding.cpp
CGI 				:= cgi.cpp base64.cpp
SERVER 				:= webserv.cpp
UTILITY 			:= splitted_string.cpp raw_request_reader.cpp byte_vector.cpp utility.cpp http_method.cpp file_utility.cpp
SRC					:= $(CONFIG_SRCS) $(SOCKET) $(CGI) $(SERVER) $(UTILITY)
UNIT_SRCS 			:= $(addprefix $(SRCDIR)/, $(SRC))
MANDATORY	:= main.cpp
BONUS		:= main_bonus.cpp



ifdef WITH_BONUS
SRC	+= $(BONUS)
DELENTRY	:= $(addprefix $(OBJDIR)/, $(MANDATORY))
else
SRC	+= $(MANDATORY)
DELENTRY	:= $(addprefix $(OBJDIR)/, $(BONUS))
endif

INCS			:= ./include ./srcs/config
IFLAGS			:= $(addprefix -I,$(INCS))
SRCS			:= $(addprefix $(SRCDIR)/, $(SRC))
OBJS			:= $(SRCS:.cpp=.o)
OBJECTS			:= $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))
DEPS			:= $(OBJECTS:.o=.d)

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98 -g3
UNIT_CXXFLAGS := -Wall -Wextra -Werror -std=c++11 -g3 -D UNIT_TEST

all:
	@make $(NAME)

$(NAME)	:	$(OBJECTS) | $(OBJDIR)
		$(CXX)  $(OBJECTS) -o $@
		$(RM) $(DELENTRY:.cpp=.o)
		$(RM) $(DELENTRY:.cpp=.d)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(IFLAGS) -c $< -MMD -MP -MF $(OBJDIR)/$*.d  -o $@

$(OBJDIR) :
	@mkdir $(OBJDIR)
	@mkdir $(OBJDIR)/config

clean	:
			$(RM) -r $(OBJDIR)
			$(RM) $(DELENTRY:.cpp=.o)
			$(RM) $(DELENTRY:.cpp=.d)

fclean	:	clean
			$(RM) $(NAME)

re	:		fclean all

bonus	:
			@make WITH_BONUS=1

r: run
run:
	./webserv ./srcs/config/config/mini.nginx.conf

mr: make_run
make_run:
	@make re
	@make run

ut: unit_test
unit_test: $(UNIT_SRCS)
	$(CXX) $(UNIT_CXXFLAGS) $(IFLAGS) -D UNIT_TEST -o unit_test $(UNIT_SRCS)
	touch statics/no_permission
	chmod 000 statics/no_permission
	mkdir statics/no_permission_dir
	chmod 000 statics/no_permission_dir
	touch statics/empty_file
	./unit_test

ifeq ($(findstring clean,$(MAKECMDGOALS)),)
-include $(DEPS)
endif

.PHONY: all clean fclean bonus re
