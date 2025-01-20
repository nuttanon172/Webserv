NAME      = webserv

CXX       = c++

CXXFLAGS  = -Wall -Wextra -Werror -std=c++98 -I$(DIR_INC)
#CXXFLAGS  = -Wall -Wextra -Werror -std=c++98 -g -I$(DIR_INC)
#CXXFLAGS  = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address -I$(DIR_INC)

DIR_INC   = inc
DIR_SRC   = src
OBJ_DIR   = obj

HEADERS   = $(DIR_INC)/WebServer.hpp \
            $(DIR_INC)/CGI.hpp \
            $(DIR_INC)/Client.hpp \
            $(DIR_INC)/config.hpp \
            $(DIR_INC)/Request.hpp \
            $(DIR_INC)/Response.hpp \
            $(DIR_INC)/Server.hpp \
            $(DIR_INC)/util.hpp

SRCS      = $(DIR_SRC)/main.cpp \
            $(DIR_SRC)/config.cpp \
            $(DIR_SRC)/util.cpp \
            $(DIR_SRC)/Server.cpp \
            $(DIR_SRC)/Request.cpp \
            $(DIR_SRC)/Response.cpp \
            $(DIR_SRC)/Client.cpp \
            $(DIR_SRC)/CGI.cpp

OBJS      = $(SRCS:$(DIR_SRC)/%.cpp=$(OBJ_DIR)/%.o)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(DIR_SRC)/%.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean fclean re

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
