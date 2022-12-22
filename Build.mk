NAME=ce

# Directories
SRCD=./Source
BLDD=./Build
OBJD=$(BLDD)/Objects

# Files
SRCS=$(wildcard $(SRCD)/*.cpp)
OBJS=$(patsubst $(SRCD)/%.cpp,$(OBJD)/%.obj,$(SRCS))
BIN=$(BLDD)/$(NAME).exe

# Command
CC=clang++
FLGS=-std=c++20 -O3 -Wall -Wextra -Werror

all:$(BIN)

$(BIN):$(OBJS)
	$(CC) $^ -o $@

$(OBJD)/%.obj:$(SRCD)/%.cpp
	$(CC) -c $^ -o $@ $(FLGS)

clean:
	rm -rf $(OBJS) $(BIN)

format:
	clang-format -i $(SRCS) $(wildcard $(SRCD)/*.hpp)

.PHONY:all clean format
