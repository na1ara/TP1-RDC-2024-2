#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 1024

void usage(int argc, char **argv){
    // recebe como parâmetro o IP do servidor e o porto
	printf("usage: %s <server IP> <server port>\n", argv[0]);
	printf("example: %s 127.0.0.1 51511\n", argv[0]);
	exit(EXIT_FAILURE);
}


void startORmoveORreset(char possibleMoves[4]){
    // testar quais são os movimentos permitidos
    //set aux variables to tell which moves are possible
    printf("Possible moves: ");
    for(int i=0; i<strlen(possibleMoves);i++){
        if(possibleMoves[i+1] != 0){
            switch (possibleMoves[i]){
            case 1:
                printf("up, ");
                break;
            case 2:
                printf("right, ");
                break;
            case 3:
                printf("down, ");
                break;
            case 4:
                printf("left, ");
                break;
            }
        }else{
            switch (possibleMoves[i]){
            case 1:
                printf("up.");
                break;
            case 2:
                printf("right.");
                break;
            case 3:
                printf("down.");
                break;
            case 4:
                printf("left.");
                break;
            }
        }
    }
}

void map(int matriz[5][5]) {
    char position[5][5][2]; // Array para armazenar os símbolos

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int serverLAB = matriz[i][j];
            switch (serverLAB) {
                case 1:
                    strcpy(position[i][j], "_");
                    break;
                case 2:
                    strcpy(position[i][j], ">");
                    break;
                case 3:
                    strcpy(position[i][j], "X");
                    break;
                case 4:
                    strcpy(position[i][j], "?");
                    break;
                case 5:
                    strcpy(position[i][j], "+");
                    break;
            }
        }
    }

    // Exemplo de impressão do mapa
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%s ", position[i][j]);
        }
        printf("\n");
    }
}



void hint(char resolucao[100]){
    printf("Hint: ");
    for(int i=0; i<strlen(resolucao);i++){
        if(resolucao[i+1] != 0){
            switch (resolucao[i]){
            case 1:
                printf("up, ");
                break;
            case 2:
                printf("right, ");
                break;
            case 3:
                printf("down, ");
                break;
            case 4:
                printf("left, ");
                break;
            }
        }else{
            switch (resolucao[i]){
            case 1:
                printf("up.");
                break;
            case 2:
                printf("right.");
                break;
            case 3:
                printf("down.");
                break;
            case 4:
                printf("left.");
                break;
            }
        }
    }
}

void exit(){
    // desconecta do servidor
}

int main(int argc, char **argv){
    if(argc < 3){
        usage(argc, argv);
    }


	struct sockaddr_storage storage;
	if (0 != addrparse(argv[1], argv[2], &storage)) {
		usage(argc, argv);
	}

	int s;
	s = socket(storage.ss_family, SOCK_STREAM, 0);
	if (s == -1) {
        //teste de erro de conexao
		logexit("socket");
	}
	struct sockaddr *addr = (struct sockaddr *)(&storage);
	if (0 != connect(s, addr, sizeof(storage))) {
		logexit("connect");
	}

	char addrstr[BUFSZ];
	addrtostr(addr, addrstr, BUFSZ);

    printf("connected to %s\n", addrstr);

    // comunicação cliente-servidor
    char buf[BUFSZ];
    memset(buf, 0, BUFSZ);
    printf("mensagem> ");
    // dado enviado para o servidor
	fgets(buf, BUFSZ-1, stdin);
	size_t count = send(s, buf, strlen(buf)+1, 0); //n de bits transmitido na rede
	if (count != strlen(buf)+1) {
		logexit("send");
	}
    
    memset(buf, 0, BUFSZ);
    unsigned total = 0;
    while(1){
        // fica recebendo dados do servidor
        // total ordena os dados dentro do buffer no caso de receber mais de 1024 bytes
        count = recv(s, buf + total, BUFSZ-total, 0);
        if(count == 0){
            // só retorna 0 do recv se a conexao tiver fechada o que é um erro
            break;
        }
        total += count;
    }
    close(s);

    printf("recebemos %u bytes\n ", total);
    puts(buf);

    exit(EXIT_SUCCESS);
}