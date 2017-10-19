CC=gcc -c
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

SRC=$(notdir $(wildcard $(SRC_DIR)/*.c))
BIN=$(SRC:.c=.o)

CFLAGS=-Wall -I$(INC_DIR) -std=gnu99

all: $(addprefix $(BIN_DIR)/, $(BIN))

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $< $(CFLAGS)

debug:
	@echo $(SRC)
	@echo $(BIN)

.PHONY: clean

clean:
	rm -rf $(LIB_DIR)/*.a $(addprefix $(BIN_DIR)/, $(BIN)) $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
