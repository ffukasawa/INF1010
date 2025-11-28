/*
ALUNOS:
Leonardo Cravo Sampaio -2210333
Fernanda Fukasawa - 2410444
*/

#include <stdio.h>
#include <stdlib.h>


#define N   10              // número de nós (1..10)
#define INF 1000000000      // numero muito grande para inicialização das distâncias

// Representação: LISTA DE ADJACÊNCIAS
typedef struct No {
    int v;              // vértice vizinho
    int peso;           // peso da aresta (u,v)
    struct No *prox;    // próximo vizinho na lista
} No;

No *adj[N+1];           // adj[u] = lista de vizinhos de u (1..N)

// Construção do grafo (item a)
void inicializa_lista(void) {
    for (int i = 1; i <= N; i++) {
        adj[i] = NULL;
    }
}

void adiciona_vizinho(int u, int v, int w) {
    No *novo = (No *) malloc(sizeof(No));
    if (!novo) {
        printf("Erro de alocacao de memoria.\n");
        exit(1);
    }
    novo->v    = v;
    novo->peso = w;
    novo->prox = adj[u];
    adj[u]     = novo;
}

// grafo não-direcionado: cria aresta u--v com peso w
void adiciona_aresta_lista(int u, int v, int w) {
    adiciona_vizinho(u, v, w);   // u -> v
    adiciona_vizinho(v, u, w);   // v -> u
}

// IMPRESSÕES DO ITEM (a)

// Matriz de adjacências gerada a partir da lista
void imprime_matriz_adj(void) {
    int mat[N+1][N+1];

    // zera matriz
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            mat[i][j] = 0;

    // preenche com base na lista
    for (int u = 1; u <= N; u++) {
        for (No *p = adj[u]; p != NULL; p = p->prox) {
            int v = p->v;
            mat[u][v] = p->peso;
        }
    }

    printf("\nITEM A: MATRIZ DE ADJACENCIAS (Simulada a partir do grafo) \n");

    // cabeçalho
    printf("    ");
    for (int j = 1; j <= N; j++)
        printf("%2d ", j);
    printf("\n");

    for (int i = 1; i <= N; i++) {
        printf("%2d ", i);
        for (int j = 1; j <= N; j++) {
            printf("%2d ", mat[i][j]);
        }
        printf("\n");
    }
}

// Lista de adjacências no estilo "[v | peso]"
void imprime_lista_adj(void) {
    printf("\n--- ITEM A: LISTA DE ADJACENCIAS ---\n\n");

    for (int u = 1; u <= N; u++) {
        printf("No %2d -> ", u);
        No *p = adj[u];
        while (p != NULL) {
            printf("[%d | %d]", p->v, p->peso);
            if (p->prox != NULL) printf(" -> ");
            p = p->prox;
        }
        printf(" -> NULL\n");
    }
}


// BFS – Busca em Largura (item b)

void bfs(int origem) {
    int visitado[N+1];
    int fila[N+1];
    int ini = 0, fim = 0;

    // marca todos como não visitados
    for (int i = 1; i <= N; i++) {
        visitado[i] = 0;
    }

    // coloca o nó de origem na fila
    visitado[origem] = 1;
    fila[fim++] = origem;

    printf("\nITEM B: BFS INICIANDO EM %d\n", origem);

    printf("Fila inicial: [ %d ]\n", origem);

    while (ini < fim) {
        int u = fila[ini++];   // remove da fila

        // pega os vizinhos de u em um vetor para poder ordenar
        int viz[N+1];
        int k = 0;
        for (No *p = adj[u]; p != NULL; p = p->prox) {
            viz[k++] = p->v;
        }

        // ordena vizinhos em ordem crescente (só pra saída ficar organizada)
        for (int i = 0; i < k - 1; i++) {
            for (int j = i + 1; j < k; j++) {
                if (viz[j] < viz[i]) {
                    int tmp = viz[i];
                    viz[i] = viz[j];
                    viz[j] = tmp;
                }
            }
        }

        // explora vizinhos (enfileira os ainda não visitados)
        for (int i = 0; i < k; i++) {
            int v = viz[i];
            if (!visitado[v]) {
                visitado[v] = 1;
                fila[fim++] = v;
            }
        }

        // imprime estado da fila DEPOIS de visitar u
        printf("Visitou %d --> Fila atual: [ ", u);
        for (int i = ini; i < fim; i++) {
            printf("%d ", fila[i]);
        }
        printf("]\n");
    }

    // opcional: resumo dos visitados
    printf("\nNós visitados a partir de %d: ", origem);
    for (int i = 1; i <= N; i++) {
        if (visitado[i]) printf("%d ", i);
    }
    printf("\n");
    printf("nenhum nó deixou de ser visitado");
}

// Guloso / Dijkstra (item c)
void imprime_caminho(int pai[], int v, int origem) {
    if (v == origem) {
        printf("%d", origem);
        return;
    }
    imprime_caminho(pai, pai[v], origem);
    printf(" -> %d", v);
}


void guloso(int origem) {
    int dist[N+1];
    int usado[N+1];
    int pai[N+1];

    // inicializa distâncias e marcações
    for (int i = 1; i <= N; i++) {
        dist[i] = INF;
        usado[i] = 0;
        pai[i] = -1;
    }

    dist[origem] = 0;
    pai[origem] = origem;

    printf("\n=========== ITEM C: MENOR CAMINHO (DIJKSTRA GULOSO) ==========\n");
    printf("Vértice de origem: %d\n", origem);

    for (int k = 1; k <= N; k++) {
        // escolhe vértice ainda não usado com menor distância conhecida
        int u = -1;
        for (int i = 1; i <= N; i++)
            if (!usado[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        // se não há mais vértice alcançável, pode encerrar
        if (u == -1 || dist[u] == INF)
            break;

        usado[u] = 1;

        printf("\nIteração %d\n", k);
        printf("  Vértice escolhido (menor dist atual): %d (dist = %d)\n", u, dist[u]);

        // relaxa arestas que saem de u
        for (No *p = adj[u]; p != NULL; p = p->prox) {
            int v = p->v;
            int w = p->peso;

            if (dist[u] + w <= dist[v]) {
                dist[v] = dist[u] + w;
                pai[v] = u;                 
            }
        }

        // mostra vetor de distâncias após a iteração
        printf("  Vetor de distâncias após a iteração %d:\n  ", k);
        for (int i = 1; i <= N; i++)
            if (dist[i] == INF) printf("INF ");
            else printf("%3d ", dist[i]);
        printf("\n");
    }

    // resultado final
    printf("\n=============== DISTANCIAS FINAIS ===============\n");
    for (int i = 1; i <= N; i++) {
        if (dist[i] == INF) {
            printf("Vértice %2d: inalcançável a partir de %d\n", i, origem);
        } else {
            printf("Vértice %2d: custo mínimo = %d a partir de %d\n",
                   i, dist[i], origem);
            printf(" | Caminho: ");
            imprime_caminho(pai, i, origem);
            printf("\n");
        }
    }
    printf("=================================================\n");
}

// ----------------------------
// Programa principal
// ----------------------------
int main(void) {
    inicializa_lista();

    // Arestas do grafo (lista oficial)
    adiciona_aresta_lista(1, 3, 3);
    adiciona_aresta_lista(1, 8, 3);
    adiciona_aresta_lista(1, 9, 3);
    adiciona_aresta_lista(1, 10, 1);
    adiciona_aresta_lista(1, 5, 4);

    adiciona_aresta_lista(2, 3, 4);
    adiciona_aresta_lista(2, 10, 1);

    adiciona_aresta_lista(3, 9, 4);
    adiciona_aresta_lista(3, 10, 4);
    adiciona_aresta_lista(3, 8, 2);
    adiciona_aresta_lista(3, 4, 1);
    adiciona_aresta_lista(3, 6, 1);

    adiciona_aresta_lista(4, 5, 2);
    adiciona_aresta_lista(4, 7, 2);

    adiciona_aresta_lista(5, 7, 2);
    adiciona_aresta_lista(5, 8, 4);

    adiciona_aresta_lista(6, 7, 3);

    adiciona_aresta_lista(8, 9, 1);
    adiciona_aresta_lista(8, 10, 2);

    adiciona_aresta_lista(9, 10, 4);

    // ITEM (a): imprime matriz e lista de adjacências
    imprime_matriz_adj();
    imprime_lista_adj();

    // ITEM (b): percurso em largura a partir do nó 3
    bfs(3);

    // ITEM (c): menor caminho (Dijkstra guloso) a partir do nó 3
    guloso(3);

    return 0;
}
