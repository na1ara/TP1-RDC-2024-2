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
    printf("Possible moves: ");
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
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%s ", position[i][j]);
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

void invalidMove(action action){

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

    action action, labServer;
    action.type = -1;
    size_t count;
    
    char buf[BUFSZ];
    char command[BUFSZ];
	
    // começa o jogo
    while(1){
        // comunicação cliente-servidor
        //pega novo comando do cliente
        printf(".\n");
        memset(&buf, 0, sizeof(buf));

        if(action.type<0){
            printf("mensagem de start> ");
            fgets(buf, BUFSZ-1, stdin);
            strncpy(command, buf, BUFSZ); 
            command[strcspn(command, "\n")] = '\0';
            //testa pro primeiro ser obrigatoriamente um start
            while(strcmp(command, "start") != 0){
                printf("error: start the game first\n");
                memset(buf, 0, BUFSZ);
                printf("mensagem de start> ");
                // dado enviado para o servidor
	            fgets(buf, BUFSZ-1, stdin);
                strncpy(command, buf, BUFSZ); 
                command[strcspn(command, "\n")] = '\0';
            }
            action.type = 0;
        }else{
            printf("mensagem de comando> ");
            // dado enviado para o servidor
	        fgets(buf, BUFSZ-1, stdin);
            strncpy(command, buf, BUFSZ); 
            command[strcspn(command, "\n")] = '\0';

            int ok=0;
            if (strcmp(command, "up") == 0){
                for(int i=0;i<4;i++){
                    if(labServer.moves[i]==1){
                        ok=1;
                    }
                }
                if(ok==0){
                    invalidMove(labServer);
                }
                action.type = 1;
                action.moves[0] = 1;
            }else if (strcmp(command, "right") == 0){
                for(int i=0;i<4;i++){
                    if(labServer.moves[i]==2){
                        ok=1;
                    }
                }
                if(ok==0){
                    invalidMove(labServer);
                }
                action.type = 1;
                action.moves[0] = 2;
            }else if (strcmp(command, "down") == 0){
                for(int i=0;i<4;i++){
                    if(labServer.moves[i]==3){
                        ok=1;
                    }
                }
                if(ok==0){
                    invalidMove(labServer);
                }
                action.type = 1;
                action.moves[0] = 3;
            }else if (strcmp(command, "left") == 0){
                for(int i=0;i<4;i++){
                    if(labServer.moves[i]==4){
                        ok=1;
                    }
                }
                if(ok==0){
                    invalidMove(labServer);
                }
                action.type = 1;
                action.moves[0] = 4;
            }else if (strcmp(command, "map") == 0){
                action.type = 2;
            }else if (strcmp(command, "hint") == 0){
                action.type = 3;
            }else if (strcmp(command, "reset") == 0){
                action.type = 6;
            }else if (strcmp(command, "exit") == 0){
                action.type = 7;
                close(s);
            }else{
                printf("error: command not found\n");
            }
        }

        printf("action type: %d\n", action.type);
        printf("comando selecionado ok\n");
        count = send(s, &action, sizeof(action), 0); //n de bits transmitido na rede
	    if (count != sizeof(action)) {
	    	logexit("send");
	    }
        printf("mensagem transmitida\n");

        memset(&labServer, 0, sizeof(labServer));
        count = recv(s, &labServer, sizeof(labServer), 0);

        printf("response type: %d\n", labServer.type);
        puts(buf);

        //tratamento pós resposta do server
        switch(action.type){
            case 0:
                break;
            case 1:
                break;
            case 2:
                map(labServer);
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;

        }

    }

    exit(EXIT_SUCCESS);
}