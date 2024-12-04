# Variáveis
BIN_DIR = bin
CC = gcc
CFLAGS = -Wall

# Alvo principal
all: $(BIN_DIR)/client $(BIN_DIR)/server

# Compilação do arquivo comum
common.o: common.c
	$(CC) $(CFLAGS) -c common.c

# Compilação do executável do cliente
$(BIN_DIR)/client: client.c common.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) client.c common.o -o $@

# Compilação do executável do servidor
$(BIN_DIR)/server: server.c common.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) server.c common.o -o $@

# Limpeza dos arquivos gerados
clean:
	rm -rf common.o $(BIN_DIR)

# Phony targets
.PHONY: all clean
