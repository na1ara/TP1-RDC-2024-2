#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#define BUFSZ 2048

void start(int board[10][10], char *path){
    FILE *arq = fopen(path, "r");
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
    //for(int i=0;i<10;i++){
    //    for(int j=0;j<10;j++){
    //        printf("%d ", board[i][j]);
    //    }
    //    printf("\n");
    //}

    printf("starting new game\n");
}

void usage(int argc, char **argv) {
    //printf("usage: %s <v4|v6> <server port> -i <path to lab>\n", argv[0]);
    //printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

void move(action action, int board[10][10], int jogado[10][10]){
    int jog_coluna = 0;
    int jog_linha = 0;
    //acha primeiro o jogador
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(jogado[i][j] == 5){
                jog_linha = i;
                jog_coluna = j;
                break;
            }
        }
    }
    //tem que fazer a mudança da posição
    jogado[jog_linha][jog_coluna] = board[jog_linha][jog_coluna];
    switch(action.moves[0]){
        case 1:
            jogado[jog_linha-1][jog_coluna]=5;
            jog_coluna-=1;
            //printf("cima");
            break;
        case 2:
            jogado[jog_linha][jog_coluna+1]=5;
            jog_coluna+=1;
            //printf("direita");
            break;
        case 3:
            jogado[jog_linha+1][jog_coluna]=5;
            jog_linha+=1;
            //printf("baixo");
            break;
        case 4:
            jogado[jog_linha][jog_coluna-1]=5;
            jog_coluna-=1;
            //printf("esquerda");
            break;
    }
    
    
    //atualiza o tabuleiro com as novas posições visíveis
    //mostra a linha de cima
    if(jog_linha!=0){
        jogado[jog_linha-1][jog_coluna] = board[jog_linha-1][jog_coluna];
        if(jog_coluna!=0){
            jogado[jog_linha-1][jog_coluna-1] = board[jog_linha-1][jog_coluna-1];
        }
        if(jog_coluna!=9){
            jogado[jog_linha-1][jog_coluna+1] = board[jog_linha-1][jog_coluna+1];
        }
    }
    //mostra linha de baixo
    if(jog_linha!=9){
        jogado[jog_linha+1][jog_coluna] = board[jog_linha+1][jog_coluna];
        if(jog_coluna!=0){
            jogado[jog_linha+1][jog_coluna-1] = board[jog_linha+1][jog_coluna-1];
        }
        if(jog_coluna!=9){
            jogado[jog_linha+1][jog_coluna+1] = board[jog_linha+1][jog_coluna+1];
        }
    }
    //mostra esquerda
    if(jog_coluna!=0){
        jogado[jog_linha][jog_coluna-1] = board[jog_linha][jog_coluna-1];
    }
    //mostra direita
    if(jog_coluna!=9){
        jogado[jog_linha][jog_coluna+1] = board[jog_linha][jog_coluna+1];
    }

    
}

void map(action *action, int board[10][10]){
    // envia o mapa para o cliente
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            action->board[i][j] = board[i][j];
        }
    }
}

void hint(action *action, int board[10][10], int jogado[10][10]){
    // envia lista de movimentos
    // tem que chamar o possible moves algumas vezes
    int win = 0;
    while(win==0){
        char moves[4];
        for(int n=0;n<4;n++){
            moves[n] = 1; //inicializa podendo tudo
        }

        for(int i=0; i<10;i++){
            for(int j=0; j<10;j++){
                if(jogado[i][j]==5){ //acha o jogador
                    if(i==0 || board[i-1][j] == 0 ){
                        //se primeira linha ou de cima eh muro, up nao pode
                        moves[0] = 0; //up
                    }
                    if(j==9 || board[i][j+1] == 0){
                        //se ultima coluna ou coluna do lado eh muro, rught nao pode
                        moves[1] = 0; //right
                    }
                    if(i==9 || board[i+1][j] == 0){
                        //se ultima linha ou linha debaixo eh muro, down nao pode
                        moves[2] = 0; //down
                    }
                    if(j==0 || board[i][j-1] == 0){
                        //se primeira coluna ou coluna anteriore eh muro, left nao pode
                        moves[3] = 0; //left
                    }
                }
            }
        }

        //procura sempre pela parede da direita
        //ou seja, right->
    }

}

void update(action *action, int board[10][10]){
    //atualizar o labirinto a ser enviado
    //recursivo da possibleMoves e vai guardando o mais a direita que pode
    action->type = 4;
}

int win(int board[10][10], int jogado[10][10]){
    int jog_coluna = 0;
    int jog_linha = 0;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(jogado[i][j] == 5){
                jog_linha = i;
                jog_coluna = j;
                break;
            }
        }
    }
    if(board[jog_linha][jog_coluna]==3){
        return 1;
    }else{
        return 0;
    }
}

void possibleMoves(action *action, int board[10][10], int jogado[10][10]){
    //precisa ler o tabuleiro e dizer pra onde pode ir

    char pmoves[4];
    for(int n=0;n<4;n++){
        pmoves[n] = 1; //inicializa podendo tudo
    }
    for(int j=0;j<100;j++){
        action->moves[j] = 0;
    }
    int marked=0;

    for(int i=0; i<10;i++){
        for(int j=0; j<10;j++){
            if(jogado[i][j]==5){ //acha o jogador
                if(i==0 || board[i-1][j] == 0 ){
                    //se primeira linha ou de cima eh muro, up nao pode
                    pmoves[0] = 0; //up
                }
                if(j==9 || board[i][j+1] == 0){
                    //se ultima coluna ou coluna do lado eh muro, rught nao pode
                    pmoves[1] = 0; //right
                }
                if(i==9 || board[i+1][j] == 0){
                    //se ultima linha ou linha debaixo eh muro, down nao pode
                    pmoves[2] = 0; //down
                }
                if(j==0 || board[i][j-1] == 0){
                    //se primeira coluna ou coluna anteriore eh muro, left nao pode
                    pmoves[3] = 0; //left
                }
            }
        }
    }

    for(int k=0;k<4;k++){
        if(pmoves[k]==1){ //entra se o movimento dessa pos pode
            if(k==0){ //olha se eh o up que pode
                action->moves[marked] = 1;
            }else if(k==1){
                action->moves[marked] = 2;
            }else if(k==2){
                action->moves[marked] = 3;
            }else if(k==3){
                action->moves[marked] = 4;
            }
            marked++;
        }
    }
    //for(int z=0;z<4;z++){
    //    printf("%d", action->moves[z]);
    //    printf(" ");
    //}
}

int main(int argc, char **argv) {

    // leitura do labirinto vindo do arquivo só um board
    int board[10][10]; // labirinto INICIAL do jogo
    int jogado[10][10]; // labirinto com as coisas que o jogador já descobriu 

    if (argc < 5) {
        usage(argc, argv);
    }

    if(strcmp(argv[3], "-i") != 0){
        usage(argc,argv);
    }

    char *path = argv[4];


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
    //printf("bound to %s, waiting connections\n", addrstr);
    //printf("client connected\n");

    while (1) {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        // socket criado quando o cliente conecta, socket que conversa com o cliente
        int csock = accept(s, caddr, &caddrlen);
        if (csock == -1) {
            logexit("accept");
        }
        printf("client connected\n");

        // caminho do arquivo tem que passar como "./server v4 51511 -i input/in.txt"
        char caddrstr[BUFSZ];
        addrtostr(caddr, caddrstr, BUFSZ);
        
        action buf; //mensagem recebida vinda do cliente
        int ganhou=0;
        while(1){
            //printf(".\n");
            //char buf[BUFSZ];
            //memset(buf, 0, BUFSZ);
            memset(&buf, 0, sizeof(buf));

            //recebe mensagem do cliente
            size_t count;
            count = recv(csock, &buf, sizeof(buf),0);
            
            //printf("[msg] %s, %d bytes: %d\n", caddrstr, (int)count, buf.type);
            //printf("a\n");
            //printf("msg type: %d\n", buf.type);
            action response;
            //eh o response que ele envia para o cliente
            response.type = 4;

            switch (buf.type)
            {
            case 0:
                //start
                start(board, path);
                int ini_i = 0;
                int ini_j = 0;
                for(int i=0;i<10;i++){
                    for(int j=0;j<10;j++){
                        jogado[i][j] = 4;
                        if(board[i][j]==2){
                            jogado[i][j]=5;
                            ini_i = i;
                            ini_j = j;
                        }
                    }
                }
                //mostra a linha de cima
                if(ini_i!=0){
                    jogado[ini_i-1][ini_j] = board[ini_i-1][ini_j];
                    if(ini_j!=0){
                        jogado[ini_i-1][ini_j-1] = board[ini_i-1][ini_j-1];
                    }
                    if(ini_j!=9){
                        jogado[ini_i-1][ini_j+1] = board[ini_i-1][ini_j+1];
                    }
                }
                //mostra linha de baixo
                if(ini_i!=9){
                    jogado[ini_i+1][ini_j] = board[ini_i+1][ini_j];
                    if(ini_j!=0){
                        jogado[ini_i+1][ini_j-1] = board[ini_i+1][ini_j-1];
                    }
                    if(ini_j!=9){
                        jogado[ini_i+1][ini_j+1] = board[ini_i+1][ini_j+1];
                    }
                }
                //mostra esquerda
                if(ini_j!=0){
                    jogado[ini_i][ini_j-1] = board[ini_i][ini_j-1];
                }
                //mostra direita
                if(ini_j!=9){
                    jogado[ini_i][ini_j+1] = board[ini_i][ini_j+1];
                }

                possibleMoves(&response, board, jogado);
                break;
            case 1:
                //move
                move(buf, board, jogado);
                ganhou = win(board, jogado);
                if(ganhou==1){
                    response.type = 5;
                    map(&response, board);
                }
                possibleMoves(&response, board, jogado);
                break;
            case 2:
                //map
                map(&response, jogado);
                break;
            case 3:
                //hint
                break;
            case 6:
                //reset
                start(board, path);
                ini_i = 0;
                ini_j = 0;
                for(int i=0;i<10;i++){
                    for(int j=0;j<10;j++){
                        jogado[i][j] = 4;
                        if(board[i][j]==2){
                            jogado[i][j]=5;
                            ini_i = i;
                            ini_j = j;
                        }
                    }
                }
                //mostra a linha de cima
                if(ini_i!=0){
                    jogado[ini_i-1][ini_j] = board[ini_i-1][ini_j];
                    if(ini_j!=0){
                        jogado[ini_i-1][ini_j-1] = board[ini_i-1][ini_j-1];
                    }
                    if(ini_j!=9){
                        jogado[ini_i-1][ini_j+1] = board[ini_i-1][ini_j+1];
                    }
                }
                //mostra linha de baixo
                if(ini_i!=9){
                    jogado[ini_i+1][ini_j] = board[ini_i+1][ini_j];
                    if(ini_j!=0){
                        jogado[ini_i+1][ini_j-1] = board[ini_i+1][ini_j-1];
                    }
                    if(ini_j!=9){
                        jogado[ini_i+1][ini_j+1] = board[ini_i+1][ini_j+1];
                    }
                }
                //mostra esquerda
                if(ini_j!=0){
                    jogado[ini_i][ini_j-1] = board[ini_i][ini_j-1];
                }
                //mostra direita
                if(ini_j!=9){
                    jogado[ini_i][ini_j+1] = board[ini_i][ini_j+1];
                }

                possibleMoves(&response, board, jogado);
                break;
            case 7:
                //exit
                close(csock);
                printf("client disconnected");
                break;
            }

            

            //sprintf(&buf, "remote endpoint: %.1000s\n", caddrstr);
            //printf("b\n");


            //precisa ter um update aqui no meio do caminho
            
            count = send(csock, &response, sizeof(response), 0);
            if (count != sizeof(response)) {
                logexit("send");
            }

        }
    }

    exit(EXIT_SUCCESS);
}
