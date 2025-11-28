CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic
ALT = #-DALT


ifeq ($(OS), Windows_NT)
    PLATFORM = windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Linux)
        PLATFORM = linux
    endif
    ifeq ($(UNAME_S), Darwin)
        PLATFORM = macos
    endif
endif


NAME = choice
SRC_DIR = src
OBJ_DIR = build


SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))


all: $(NAME)


$(NAME): $(OBJS)
ifeq ($(PLATFORM), windows)
	@powershell -Command "if (-not (Test-Path $(OBJ_DIR))) \
	{[void](New-Item -ItemType Directory -Path $(OBJ_DIR))}"
else
	@mkdir -p $(OBJ_DIR)
endif
	@$(CXX) $(CXXFLAGS) $^ -o $(OBJ_DIR)/$(NAME)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(PLATFORM), windows)
	@powershell -Command "if (-not (Test-Path $(OBJ_DIR))) \
	{[void](New-Item -ItemType Directory -Path $(OBJ_DIR))}"
else
	@mkdir -p $(OBJ_DIR)
endif
	@$(CXX) $(CXXFLAGS) $(ALT) -c $< -o $@


clean:
ifeq ($(PLATFORM), windows)
	@powershell -Command "if (Test-Path $(OBJ_DIR)) \
	{Remove-Item -Recurse -Force $(OBJ_DIR)/*.o}"
else
	@rm -f $(OBJ_DIR)/*.o
endif


fclean:
ifeq ($(PLATFORM), windows)
	@powershell -Command "if (Test-Path $(OBJ_DIR)) \
	{Remove-Item -Path $(OBJ_DIR) -Recurse -Force}"
else
	@rm -rf $(OBJ_DIR)
endif


re: fclean all


.PHONY: all clean fclean re