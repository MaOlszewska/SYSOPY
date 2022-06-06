#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLIENTS 10
#define MESSAGE_LENGTH 200
#define NAME_LENGTH 20
#define CMD_LENGTH 15
#define MESSAGE_STR_LENGTH 250

typedef struct player{
    int fd;
    char* nick;
    int gameId;
}player;

typedef enum SIGN{
    O = 0,
    X = 1,
    FREE = 2
}SIGN;

typedef struct game{
    int player1_id;
    int player2_id;
    SIGN player1_sign;
    SIGN player2_sign;
    SIGN board[9];
}game;

typedef enum GAME_STATUS{
    RUNNING = 0,
    O_WINNER = 1,
    X_WINNER = 2,
    DRAW = 3,
    END = 4
}STATUS;

typedef enum TURN{
    PLAYER_1,
    PLAYER_2
}TURN;



typedef struct message{
    char* cmd;
    char* name;
    char* message;
}message;

typedef struct gameMessage{
    char* opponent;
    char* board;
    char sign;
}gameMessage;

message* parseMessage(char* string){
    message* res = (message*) calloc(1, sizeof(message));
    res->cmd = (char*) calloc(1, CMD_LENGTH);
    res->message = (char*) calloc(1, MESSAGE_LENGTH);
    res->name = (char*) calloc(1, NAME_LENGTH);
    char* pch;
    char* end;
    pch = strtok_r(string, ":", &end);
    strcpy(res->cmd, pch);
    pch = strtok_r(NULL, ":", &end);
    strcpy(res->name, pch);
    strcpy(res->message, end);

    return res;
}

message* createMessage(char* cmd, char* name, char* msg){
    message* res = (message*) calloc(1, sizeof(message));
    res->cmd = (char*) calloc(1, CMD_LENGTH);
    res->message = (char*) calloc(1, MESSAGE_LENGTH);
    res->name = (char*) calloc(1, NAME_LENGTH);
    strcpy(res->cmd, cmd);
    strcpy(res->name, name);
    strcpy(res->message, msg);

    return res;
}

game* createGamee(int player1, int player2){
    game* new_game = (game*) calloc(1, sizeof(game));
    new_game->player1_id = player1;
    new_game->player1_id = player1;
    new_game->player1_sign = O;
    new_game->player2_sign = X;
    for (int i = 0; i < 9; i++){new_game->board[i] = FREE;}
    return new_game;
}

char signToChar(SIGN sign){
    switch (sign){
        case X: return 'X';
        case O: return 'O';
    }
}

STATUS getGameStatus(game g){
    //Sprawdzenie Wierszy
    for (int i = 0; i < 3; i++){
        if (g.board[i*3] == g.board[i*3+1] && g.board[i*3] == g.board[i*3+2] && g.board[i*3] != FREE){
            switch (g.board[i * 3]){
                case X: return 'X';
                case O: return 'O';
            }
        }
    }

    //Sprawdzenie kolumn
    for (int i = 0; i < 3; i++){
        if (g.board[i] == g.board[i+3] && g.board[i] == g.board[i+6] && g.board[i] != FREE){
            switch (g.board[i]){
                case X: return 'X';
                case O: return 'O';
            }
        }
    }

    //Sprawdzenie po prawym ukosie
    if (g.board[0] == g.board[4] && g.board[0] == g.board[8] && g.board[0] != FREE){
        if (g.board[0] == X) return X_WINNER;
        else if (g.board[0] == O) return O_WINNER;
        else return RUNNING;

    }

    //sprawdzenie po lewym ukosie
    if (g.board[2] == g.board[4] && g.board[2] == g.board[6] && g.board[2] != FREE){
        if (g.board[2] == X) return X_WINNER;
        else if (g.board[2] == O) return O_WINNER;
        else return RUNNING;
    }

    for (int i = 0; i < 9; i++){
        if (g.board[i] == FREE) return RUNNING; }

    return DRAW;

}

// funkcja służąca do rysowania planszy z grą
char* boardToString(SIGN* board){
    char* result = (char*) calloc(10*20+1, sizeof(char));
    memset(result, ' ', 20*10);

    // Tworzenie planszy do gry
    for(int i = 0; i < 10; i++){result[i*20 + 19] = '\n';}

    for(int i = 1; i < 6; i++){
        for (int j = 0; j < 4; j++){
            for (int z = 0; z < 3; z++){
                result[i+60*j+z*6] = '-';
            }
        }
    }

    for(int i = 1; i < 10; i++){
        for (int j = 0; j < 4; j++){
            result[i*20+j*6] = '|';
        }
    }

    //WYPEŁNIENIE PLANSZY KÓŁKAMI ALBO KRZYŻYKAMI
    for (int i = 0; i < 9; i++){
        char sign;
        switch (board[i]){
            case X:
                sign = 'X';
                break;
            case O:
                sign = 'O';
                break;
            default:
                sign = '0' + i;
        }
        result[43+(i%3)*6+(i/3)*60] = sign;
    }

    return result;
}
char* messageToString(message* msg){
    char* res = (char*) calloc(CMD_LENGTH + MESSAGE_LENGTH + NAME_LENGTH + 3, sizeof(char));
    sprintf(res, "%s:%s:%s", msg->cmd, msg->name, msg->message);
    return res;
}