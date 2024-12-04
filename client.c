#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 2048

void usage(int argc, char **argv){
    // recebe como parâmetro o IP do servidor e o porto
	printf("usage: %s <server IP> <server port>\n", argv[0]);
	printf("example: %s 127.0.0.1 51511\n", argv[0]);
	exit(EXIT_FAILURE);
}


void startORmoveORreset(action action){
    // testar quais são os movimentos permitidos
    //set aux variables to tell which moves are possible
    //printf("vetor que chega:");
    //for(int j=0;j<100;j++){
    //    printf("%d", action.moves[j]);
    //}
    printf("\nPossible moves: ");
    for(int i=0; i<100;i++){
        if(action.moves[i+1] != 0){
            switch (action.moves[i]){
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
            switch (action.moves[i]){
            case 1:
                printf("up.\n");
                break;
            case 2:
                printf("right.\n");
                break;
            case 3:
                printf("down.\n");
                break;
            case 4:
                printf("left.\n");
                break;
            }
        }
    }
}

void map(action action) {
    char position[10][10][2]; // Array para armazenar os símbolos

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int serverLAB = action.board[i][j];
            switch (serverLAB) {
                case 0:
                    strcpy(position[i][j], "#");
                    break;
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
		case 9:
		strcpy(position[i][j], "");
		    break;
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%s \t", position[i][j]);
        }
        printf("\n");
    }
}

void hint(action action){
    printf("Hint: ");
    for(int i=0; i<100;i++){
        if(action.moves[i+1] != 0){
            switch (action.moves[i]){
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
            switch (action.moves[i]){
            case 1:
                printf("up.\n");
                break;
            case 2:
                printf("right.\n");
                break;
            case 3:
                printf("down.\n");
                break;
            case 4:
                printf("left.\n");
                break;
            }
        return;
        }
        break;
        return;
    }
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

    //printf("connected to %s\n", addrstr);

    action action, labServer;
    action.type = -1;
    size_t count;
    
    char buf[BUFSZ];
    char command[BUFSZ];
    int acaboudeganhar = 0;
    int client_exit = 0;
	
    // começa o jogo
    while(client_exit == 0){
        // comunicação cliente-servidor
        //pega novo comando do cliente
        memset(&buf, 0, sizeof(buf));

        if(action.type<0){
            printf("> ");
            fgets(buf, BUFSZ-1, stdin);
            strncpy(command, buf, BUFSZ); 
            command[strcspn(command, "\n")] = '\0';
            //testa pro primeiro ser obrigatoriamente um start
            while(strcmp(command, "start") != 0){
                printf("error: start the game first\n");
                memset(buf, 0, BUFSZ);
                printf("> ");
                // dado enviado para o servidor
	            fgets(buf, BUFSZ-1, stdin);
                strncpy(command, buf, BUFSZ); 
                command[strcspn(command, "\n")] = '\0';
            }
            action.type = 0;
        }else if (acaboudeganhar==1){
            printf("> ");
            fgets(buf, BUFSZ-1, stdin);
            strncpy(command, buf, BUFSZ); 
            command[strcspn(command, "\n")] = '\0';
            //testa pro primeiro ser obrigatoriamente um start
            while(strcmp(command, "reset") != 0 && strcmp(command, "exit") != 0){
                memset(buf, 0, BUFSZ);
                printf("> ");
                // dado enviado para o servidor
	            fgets(buf, BUFSZ-1, stdin);
                strncpy(command, buf, BUFSZ); 
                command[strcspn(command, "\n")] = '\0';
            }
            acaboudeganhar = 0;
            if(strcmp(command, "reset") == 0){
                action.type = 6;
            }else if(strcmp(command, "exit") == 0){
                action.type = 7;
            }
        }else{
            int ok=0;
            while (ok==0){
                printf("> ");
                // dado enviado para o servidor
	            fgets(buf, BUFSZ-1, stdin);
                strncpy(command, buf, BUFSZ); 
                command[strcspn(command, "\n")] = '\0';
            
                if (strcmp(command, "up") == 0){
                    for(int i=0;i<4;i++){
                        if(labServer.moves[i]==1){
                            ok=1;
                            action.type = 1;
                            action.moves[0] = 1;
                        }
                    }
                    if(ok==0){
                        printf("error: you cannot go this way\n");
                    }
                }else if (strcmp(command, "right") == 0){
                    for(int i=0;i<4;i++){
                        if(labServer.moves[i]==2){
                            ok=1;
                            action.type = 1;
                            action.moves[0] = 2;
                        }
                    }
                    if(ok==0){
                        printf("error: you cannot go this way\n");
                    }
                }else if (strcmp(command, "down") == 0){
                    for(int i=0;i<4;i++){
                        if(labServer.moves[i]==3){
                            ok=1;
                            action.type = 1;
                            action.moves[0] = 3;
                        }
                    }
                    if(ok==0){
                        printf("error: you cannot go this way\n");
                    }
                }else if (strcmp(command, "left") == 0){
                    for(int i=0;i<4;i++){
                        if(labServer.moves[i]==4){
                            ok=1;
                            action.type = 1;
                            action.moves[0] = 4;
                        }
                    }
                    if(ok==0){
                        printf("error: you cannot go this way\n");
                    }
                }else if (strcmp(command, "map") == 0){
                    action.type = 2;
                    ok = 1;
                }else if (strcmp(command, "hint") == 0){
                    action.type = 3;
                    ok = 1;
                }else if (strcmp(command, "reset") == 0){
                    action.type = 6;
                    ok = 1;
                }else if (strcmp(command, "exit") == 0){
                    action.type = 7;
                    ok = 1;
                }else{
                    printf("error: command not found\n");
                }
            }
        }

        //printf("action type: %d\n", action.type);
        //printf("comando selecionado ok\n");
        count = send(s, &action, sizeof(action), 0); //n de bits transmitido na rede
	    if (count != sizeof(action)) {
	    	logexit("send");
	    }
        //printf("mensagem transmitida\n");

        memset(&labServer, 0, sizeof(labServer));
        count = recv(s, &labServer, sizeof(labServer), 0);

        //printf("response type: %d\n", labServer.type);
        //puts(buf);

        //tratamento pós resposta do server
        if(labServer.type==5){
            printf("You escaped!\n");
            map(labServer);
            acaboudeganhar = 1;
        }else{
            switch(action.type){
                case 0:
                    startORmoveORreset(labServer);
                    break;
                case 1:
                    startORmoveORreset(labServer);
                    break;
                case 2:
                    map(labServer);
                    break;
                case 3:
                    hint(labServer);
                    break;
                case 6:
                    startORmoveORreset(labServer);
                    break;
                case 7:
                    client_exit = 1;
                    close(s);
                    break;
            }
        }

    }

    exit(EXIT_SUCCESS);
}
