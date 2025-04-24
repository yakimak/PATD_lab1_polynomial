CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c11
SRC_DIR = c_code/src
OBJ_DIR = obj
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
TARGET = c_code/main.exe

$(info SRC_DIR = $(SRC_DIR))
$(info SRC = $(SRC))
$(info OBJ = $(OBJ))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)  # Создаём папку obj, если её нет
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	if exist $(OBJ_DIR)\*.o del $(OBJ_DIR)\*.o
	if exist $(TARGET) del $(subst /,\,$(TARGET))
