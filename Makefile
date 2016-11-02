NAME = ParticleSystem

CC = clang++ -Wall -Wextra -Werror -std=c++11
LIB_PATH = Library/glfw/
LOC_LIB = $(LIB_PATH)libGLEW.a $(LIB_PATH)libglfw3.a
FRAMEWORKS = -framework Cocoa -framework OpenGL -framework IOKit -framework Opencl

SRC_DIR = srcs

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)

%.o: %.c
		$(CC) -o $@  -c $<

all: $(NAME)

$(NAME):$(OBJ)
		$(CC) $(FRAMEWORKS) $(LOC_LIB) -o $@ $^
		@echo "\033[0;32m$(NAME) : Compilation successful !! \033[0;32m"

clean:
		@rm $(OBJ)

fclean: clean
		@rm $(NAME)

re: fclean all

.PHONY: all clean fclean re