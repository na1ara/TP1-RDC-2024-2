#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#define BUFSZ 2048

void start(int board[10][10]){
    FILE *arq = fopen("/home/naiara/TP1-RDC-2024-2/input/in.txt", "r");
    if (!arq){
        perror("error: file not found");
        return;
    }

    for (int i=0;i<10;i++){
        for (int j=0;j<10;j++){
            board[i][j] = 0; //inicializa tudo como muro
        }
    }

    int i=0, j=0;
    while (!feof(arq)){
        int num;
        if (fscanf(arq, "%d", &num) == 1){ //pega apenas num inteiros
            board[i][j] = num;
            j++;
            if (j>=10){ //pula p prox linha
                j = 0;
                i++;
            }
            if (i>=10){ //pega só o max de colunas
                break;
            }
        }
    }
    fclose(arq);
    printf("starting new game\n");

    // tem que chamar a possiblemoves
    //tem que chamar a update

}

void usage(int argc, char **argv) {
    printf("usage: %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}


void move(action *action, int board[10][10]){
    // atualiza o estado do jogo
    // envia a lista de movimentos validos
}

void map(){
    // envia o mapa para o cliente
}

void hint(){
    // envia lista de movimentos
}

void update(action *action, int board[10][10]){
    //atualizar o labirinto a ser enviado
    //recursivo da possibleMoves e vai guardando o mais a direita que pode
    action->type = 4;
}

void win(){

}

void reset(){
    // reseta o estado do jogo
    printf("starting new game\n");
}

void possibleMoves(action *action, int board[10][10]){
    //precisa ler o tabuleiro e dizer pra onde pode ir
    int lab_env[10][10];
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            lab_env[i][j] = action->board[i][j];
        }
    }

    int up = 0;
    int right = 0;
    int down = 0;
    int left = 0;
    
    for(int i=0; i<10;i++){
        for(int j=0; j<10;j++){
            if(lab_env[i][j]==5){ //acha o jogador
                if(i==0 || board[i-1][j] == 0 ){
                    up = 0;
                }
                if(j==9 || board[i][j+1] == 0){
                    right = 0;
                }
                if(i==9 || board[i+1][j] == 0){
                    down = 0;
                }
                if(j==0 || board[i][j-1] == 0){
                    left = 0;
                }
            }
        }
    }

    //falta armazenar em ordem no array
}

int main(int argc, char **argv) {

    // leitura do labirinto vindo do arquivo só um board
    int board[10][10]; // labirinto INICIAL do jogo
    int jogado[10][10]; // labirinto com as coisas que o jogador já descobriu 

    if (argc < 3) {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage)) {
        usage(argc, argv);
    }

    // socket que aceita a conexao
    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    }

    int enable = 1;
    //set uma opção de reusar um endereço
    if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        logexit("setsockopt");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage))) {
        logexit("bind");
    }

    // conexoes pendentes
    if (0 != listen(s, 10)) {
        logexit("listen");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("bound to %s, waiting connections\n", addrstr);
    printf("client connected\n");

    while (1) {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        // socket criado quando o cliente conecta, socket que conversa com o cliente
        int csock = accept(s, caddr, &caddrlen);
        if (csock == -1) {
            logexit("accept");
        }
        printf("socket criado\n");

        // caminho do arquivo tem que passar como "./server v4 51511 -i input/in.txt"
        char caddrstr[BUFSZ];
        addrtostr(caddr, caddrstr, BUFSZ);
        
        //printf("[log] connection from %s\n", caddrstr);
        while(1){
            printf("?\n");
            //char buf[BUFSZ];
            //memset(buf, 0, BUFSZ);
            action buf; //mensagem recebida vinda do cliente
            memset(&buf, 0, sizeof(buf));

            //recebe mensagem do cliente
            size_t count = recv(csock, &buf, sizeof(buf) - 1, 0);
            printf("[msg] %s, %d bytes: %d\n", caddrstr, (int)count, buf.type);
            printf("a\n");
            printf("msg type: %d", buf.type);

            switch (buf.type)
            {
            case 0:
                //start
                start(board);
                for(int i=0;i<10;i++){
                    for(int j=0;j<10;j++){
                        jogado[i][j] = board[i][j];
                    }
                }
                break;
            case 1:
                //move
                break;
            case 2:
                //map
                break;
            case 3:
                //hint
                break;
            case 4:
                //update
                //isso tb n vai existir eu acho
                break;
            case 5:
                //win
                //isso nao vai existir eu acho
                break;
            case 6:
                //reset
                break;
            case 7:
                //exit
                close(csock);
                printf("client disconnected");
                break;
            }

            

            //sprintf(&buf, "remote endpoint: %.1000s\n", caddrstr);
            //printf("b\n");

            //eh o response que ele envia para o cliente
            action response;
            response.type = 4;

            //precisa ter um update aqui no meio do caminho
            
            count = send(csock, &response, sizeof(response) + 1, 0);
            printf("c\n");
            if (count != sizeof(response) + 1) {
                logexit("send");
            }

        }
    }

    exit(EXIT_SUCCESS);
}
