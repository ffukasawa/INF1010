/*
LABORATORIO 4
Leonardo Cravo-2210333
Fernanda Fukasawa-2410444
*/
#include <stdio.h>
#define MAX 4
#define MIN 2

typedef struct no t_no;
struct no
{
int ndesc;
int chave[MAX]; 
t_no *ramo[MAX+1];
};

void intervalo(t_no *arv, int lim_inf, int lim_sup) {
    if (arv == NULL)
        return;

    int i;

    for (i = 0; i < arv->ndesc; i++) {

        intervalo(arv->ramo[i], lim_inf, lim_sup);

        if (arv->chave[i] > lim_inf && arv->chave[i] <= lim_sup) {
            printf("%d ", arv->chave[i]);
        }
    }

    intervalo(arv->ramo[arv->ndesc], lim_inf, lim_sup);
}

int main() {
//folhas
    t_no f1 = {2, {10, 40}, {NULL, NULL, NULL, NULL, NULL}};
    t_no f2 = {2, {60, 70}, {NULL, NULL, NULL, NULL, NULL}};
    t_no f3 = {2, {80, 90}, {NULL, NULL, NULL, NULL, NULL}};
    t_no f4 = {2, {110, 115}, {NULL, NULL, NULL, NULL, NULL}};
    t_no f5 = {4, {130, 135, 140, 170}, {NULL, NULL, NULL, NULL, NULL}};
    t_no f6 = {4, {220, 230, 240, 250}, {NULL, NULL, NULL, NULL, NULL}};
//intermediarios
    t_no n1 = {
        2, {50, 75},
        {&f1, &f2, &f3, NULL, NULL}   // 3 filhos
    };

    t_no n2 = {
        2, {120, 200},
        {&f4, &f5, &f6, NULL, NULL}   // 3 filhos
    };

//raiz
    t_no raiz = {
        1, {100},
        {&n1, &n2, NULL, NULL, NULL}
    };


    printf("Intervalo (0, 260): ");
    intervalo(&raiz, 0, 260);

    printf("\n");
    return 0;
}