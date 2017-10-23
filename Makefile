CC=gcc -c
LIB_DIR=$(realpath ./lib)
INC_DIR=$(realpath ./include)
BIN_DIR=$(realpath ./bin)
SRC_DIR=$(realpath ./src)

SRC=$(wildcard $(SRC_DIR)/*.c)
BIN=$(addprefix $(BIN_DIR)/, $(notdir $(SRC:.c=.o)))
CFLAGS=-Wall -I$(INC_DIR) -std=gnu99 -m32

all: $(BIN)
	ar -cvq $(LIB_DIR)/libcthread.a $^ $(BIN_DIR)/support.o

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: install
.PHONY: debug	
.PHONY: clean
	
install:
	
debug:
	@echo $(SRC)
	@echo $(BIN)
	@echo $(LIB_DIR)
	@echo $(INC_DIR)
	@echo $(BIN_DIR)
	@echo $(SRC_DIR)

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN) $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
