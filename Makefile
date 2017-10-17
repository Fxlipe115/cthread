#
# Makefile de EXEMPLO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# É NECESSARIO ADAPTAR ESSE ARQUIVO de makefile para suas necessidades.
#  1. Cuidado com a regra "clean" para não apagar o "support.o"
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "cthread"
#

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

SRC=$(wildcard $(SRC_DIR)*.c)
BIN=$(addprefix $(BIN_DIR),$(notdir $(SRC:.c=.o)))

CFLAGS=-Wall -I$(INC_DIR)

all: $(BIN)

%.o: %.c
	@mkdir -p $(BIN_DIR)
	$(CC) -c $(BIN_DIR)$@ $(SRC_DIR)$^ $(CFLAGS)

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
