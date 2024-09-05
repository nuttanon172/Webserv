NAME      = webserv

CXX       = c++

CXXFLAGS  = -Wall -Wextra -Werror -std=c++98

OBJ_DIR   = obj

# Update HEADERS to include the single main header file
HEADERS   = config.hpp util.hpp

# Add all the necessary .cpp files to SRCS
SRCS      = main.cpp config.cpp util.cpp

# Create object files from the source files
OBJS      = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Build the executable
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# Rule for creating object files in the OBJ_DIR directory
$(OBJ_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Standard Makefile rules
.PHONY: all clean fclean re

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
