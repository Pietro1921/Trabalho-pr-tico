#ifndef SLEM_H
#define SLEM_H

#define MAX_NOME 51
#define MAX_PLACA 8
#define MAX_MODELO 31
#define MAX_STATUS 12

struct Local {
    char nome[MAX_NOME];
    int coordenadaX;
    int coordenadaY;
};

struct Veiculo {
    char placa[MAX_PLACA];
    char modelo[MAX_MODELO];
    char status[MAX_STATUS];
    char localAtual[MAX_NOME];
};

struct Pedido {
    int id;
    char localOrigem[MAX_NOME];
    char localDestino[MAX_NOME];
    float peso;
};

#endif