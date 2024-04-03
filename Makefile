ifeq ($(OS),Windows_NT)
    DIR_CHECK = if not exist $(OBJ_DIR) mkdir
    RM = del /Q
    MKDIR = $(DIR_CHECK)
	EXTRA_LIB = 
	EXTRA_FILE = 
else
    DIR_CHECK = test -d $(OBJ_DIR) || mkdir -p
    RM = rm -f
    MKDIR = $(DIR_CHECK)
	EXTRA_LIB = -l wiringPi
	EXTRA_FILE = comm.o
endif

CC = gcc
FLAG = -g -Wall -I./include $(EXTRA_LIB)
SRC_DIR = ./src
INCLUDE_DIR = ./include
OBJ_DIR = bin
OBJ_DELIMITER = /

all: dalapi_AI

dalapi_AI: creatDir model.o utils.o imageProcessing.o file.o $(EXTRA_FILE)
	$(CC) $(wildcard $(OBJ_DIR)/*.o) -lm $(EXTRA_LIB) -o dalapi_AI

creatDir:
	$(MKDIR) $(OBJ_DIR)

utils.o: $(SRC_DIR)/utils/utils.c
	$(CC) $(FLAG) -c $< -o $(OBJ_DIR)$(OBJ_DELIMITER)$@

model.o: $(SRC_DIR)/model.c
	$(CC) $(FLAG) -c $< -o $(OBJ_DIR)$(OBJ_DELIMITER)$@

imageProcessing.o : $(SRC_DIR)/process/imageProcessing.c
	$(CC) $(FLAG) -c $< -o $(OBJ_DIR)$(OBJ_DELIMITER)$@

ifneq ($(OS),Windows_NT)
comm.o : $(SRC_DIR)/process/comm.c
	$(CC) $(FLAG) -c $< -o $(OBJ_DIR)$(OBJ_DELIMITER)$@
endif


file.o : $(SRC_DIR)/utils/file.c $(INCLUDE_DIR)/process/imageProcessing.h
	$(CC) $(FLAG) -c $< -o $(OBJ_DIR)$(OBJ_DELIMITER)$@

doc : 
	doxygen Doxyfile
clean:
	$(RM) $(OBJ_DIR)$(OBJ_DELIMITER)*.o *.a *.gch dalapi_AI
