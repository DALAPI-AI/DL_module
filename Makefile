ifeq ($(OS),Windows_NT)
    DIR_CHECK = if not exist $(OBJ_DIR) mkdir
    RM = del /Q
    MKDIR = $(DIR_CHECK)
else
    DIR_CHECK = test -d $(OBJ_DIR) || mkdir -p
    RM = rm -f
    MKDIR = $(DIR_CHECK)
endif

CC = gcc
FLAG = -g -Wall -I./include
SRC_DIR = ./src
INCLUDE_DIR = ./include
OBJ_DIR = ./bin
OBJ_DELIMITER = /

all: test

utils.o: $(SRC_DIR)/utils/utils.c
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(FLAG) -c $< -o $(OBJ_DIR)$(OBJ_DELIMITER)$@

model.o: $(SRC_DIR)/model.c
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(FLAG) -c $< -o $(OBJ_DIR)$(OBJ_DELIMITER)$@

test: model.o utils.o
	$(CC) $(wildcard $(OBJ_DIR)/*.o) -lm -o test

clean:
	$(RM) $(OBJ_DIR)$(OBJ_DELIMITER)*.o *.a *.gch test
