/*
Alunos: Fernanda Fukasawa - 2410444
Leonardo Cravo Sampaio - 2210333
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARQ_ENTRADA "cpfs.txt"
#define M        1201     // 1º primo após 1200
#define MAXK     1000     // até 1000 CPFs
#define CPF_LEN  12       // 11 dígitos + '\0'

typedef struct {
    char cpf[CPF_LEN];
    int  usado;           // 0 = livre, 1 = ocupado
} Tabela;
/*
 Função hash polinomial
 Transforma uma string em um valor inteiro (0..mod-1)
 usando uma base p (geralmente um número primo pequeno).
 */
int hash_polinomial(char *s, int mod, int p) {
    long long h = 0;
    int i = 0;
    while (s[i] != '\0') {
        h = (h * p + (unsigned char)s[i]) % mod;
        i++;
    }
    return (int)h;
}

// Inserção com double hashing e retorna colisões desta inserção
int inserir(Tabela *tab, char *cpf, int p1, int p2) {
    int h1 = hash_polinomial(cpf, M,   p1);  // hash primário
    int h2 = 1 + hash_polinomial(cpf, M-1, p2); // hash secundário (sempre > 0)
    int col = 0;
    for (int i = 0; i < M; i++) {  // Tenta inserir em até M posições
        int pos = (h1 + i * h2) % M;      // double hashing
        if (!tab[pos].usado) {
            strncpy(tab[pos].cpf, cpf, CPF_LEN - 1);  // posição livre -> insere
            tab[pos].cpf[CPF_LEN - 1] = '\0';
            tab[pos].usado = 1;
            return col;  // retorna nº de colisões até achar espaço
        }
        col++;  // posição ocupada -> conta colisão
    }
    return M; // tabela cheia (não deve ocorrer com 1000 em M=1201)
}


int main(void) {
    //leitura e armazenamento dos dados do arquivo com os cpfs
    FILE *f = fopen(ARQ_ENTRADA, "rt"); 
    if (!f) { printf("Erro abrindo %s\n", ARQ_ENTRADA); return 1; }

    char cpfs[MAXK][CPF_LEN];
    int ncpfs = 0;

   while (ncpfs < MAXK && fscanf(f, "%11s", cpfs[ncpfs]) == 1) {
    ncpfs++;
    }
    fclose(f);

    if (ncpfs == 0) { printf("Nenhum CPF valido.\n"); return 0; }

    //inicializamos a tabela e o arquivo de CSV que vamos usar para montar o gráfico depois
    Tabela *tab = (Tabela*)malloc(M * sizeof(Tabela));
    if (!tab) { printf("Memoria insuficiente.\n"); return 1; }

    FILE *csv = fopen("colisoes.csv", "w");
    if (!csv) { printf("Erro criando colisoes.csv\n"); free(tab); return 1; }
    fprintf(csv, "num_chaves,colisoes_acumuladas\n");

    int p1 = 31, p2 = 37;

    printf("num_chaves,colisoes_acumuladas\n");
    int limite_final = ncpfs < 1000 ? (ncpfs/100)*100 : 1000; // múltiplo de 100
    for (int lim = 100; lim <= limite_final; lim += 100) {
        // zera tabela
        for (int i = 0; i < M; i++) { tab[i].usado = 0; tab[i].cpf[0] = '\0'; }

        long long acc = 0;
        int erro = 0;
        for (int k = 0; k < lim; k++) {
            int c = inserir(tab, cpfs[k], p1, p2);
            if (c >= M) { erro = 1; break; }
            acc += c;
        }

         // imprime e salva no CSV o total de colisões até esse ponto, usamos o csv para facilitar na criação do gráfico
        if (erro) {
            printf("%d,TABELA_CHEIA\n", lim);
            fprintf(csv, "%d,TABELA_CHEIA\n", lim);
        } else {
            printf("%d,%lld\n", lim, acc);
            fprintf(csv, "%d,%lld\n", lim, acc);
        }
    }

    fclose(csv);
    free(tab);
    printf("CSV gerado: colisoes.csv\n");
    return 0;
}