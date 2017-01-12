NAME := ParticleSystem

CC := clang++ 
CFLAGS := -c -std=c++11 -Wall -Wextra -O3
#-Werror

LIB_PATH := Library/glfw/
LOC_LIB := $(LIB_PATH)libGLEW.a $(LIB_PATH)libglfw3.a 

FRAMEWORKS := -framework Cocoa -framework OpenGL -framework IOKit -framework Opencl

SRC_DIR := srcs
INC_DIRS := $(shell find includes/* -name '*.hpp' -exec dirname {} \; | sort | uniq)
BUILD_DIR := build

SRCEXT := cpp
SOURCES := $(shell find $(SRC_DIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INCLIST := $(patsubst includes/%,-I includes/%,$(INC_DIR))

all: $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRCEXT)
		@mkdir -p $(BUILD_DIR)
		@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(NAME):$(OBJECTS)
		@echo "Linking..."
		@echo "		$(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(NAME) $(LOC_LIB) $(FRAMEWORKS)
		@echo "\033[0;32m$(NAME) : Compilation successful !! \033[0;32m"

clean:
		@echo "Cleaning...";
		@echo "		$(RM) -r $(BUILD_DIR) $(TARGET)"; $(RM) -r $(BUILD_DIR)

fclean: clean
		@echo "		$(RM) -r $(NAME)"; $(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re