NAME	:= webserv
LIB		:= ./lib/
OBJDIR	:= ./obj
SRCDIR	:= ./srcs/
CFGDIR	:= config/

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
SOCKET 	:=  fd_manager.cpp socket_data.cpp request.cpp response.cpp socket.cpp tcp_socket.cpp
CGI 	:=
SERVER 	:= webserv.cpp
UTILITY := split.cpp get_next_line.cpp utility.cpp
SRC			:= $(CONFIG_SRCS) $(SOCKET) $(CGI) $(SERVER) $(UTILITY)

MANDATORY	:= main.cpp
BONUS		:= main_bonus.cpp


ifdef WITH_BONUS
SRC	+= $(BONUS)
DELENTRY	:= $(addprefix $(OBJDIR)/, $(MANDATORY))
else
SRC	+= $(MANDATORY)
DELENTRY	:= $(addprefix $(OBJDIR)/, $(BONUS))
endif

INCS	:= ./include ./srcs/config
IFLAGS	:= $(addprefix -I,$(INCS))
SRCS	:= $(addprefix $(SRCDIR), $(SRC))
OBJS	:= $(SRCS:.cpp=.o)
OBJECTS	:= $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))
DEPS	:= $(OBJECTS:.o=.d)

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98

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

run:
	./webserv ./srcs/config/config/mini.nginx.conf



ifeq ($(findstring clean,$(MAKECMDGOALS)),)
-include $(DEPS)
endif

.PHONY: all clean fclean bonus re
