#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h" //inclui os Protótipos

//Definição do tipo Grafo
struct grafo{
    int eh_ponderado;
    int nro_vertices;
    int grau_max;
    int** arestas;
    float** pesos;
    int* grau;
};


/*
A função cria_Grafo aloca dinamicamente uma estrutura de grafo e inicializa todos os seus componentes básicos: armazena o número de vértices,
o grau máximo e se o grafo é ponderado; cria um vetor para controlar o grau de cada vértice e aloca matrizes (listas) que armazenam as arestas e,
caso necessário, seus pesos. No final, o grafo é devolvido totalmente preparado para receber arestas.
*/
Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado){
    Grafo *gr;
    gr = (Grafo*) malloc(sizeof(struct grafo));
    if(gr != NULL){
        int i;
        gr->nro_vertices = nro_vertices;
        gr->grau_max = grau_max;
        gr->eh_ponderado = (eh_ponderado != 0)?1:0;
        gr->grau = (int*) calloc(nro_vertices,sizeof(int));

        gr->arestas = (int**) malloc(nro_vertices * sizeof(int*));
        for(i=0; i<nro_vertices; i++)
            gr->arestas[i] = (int*) malloc(grau_max * sizeof(int));

        if(gr->eh_ponderado){
            gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
            for(i=0; i<nro_vertices; i++)
                gr->pesos[i] = (float*) malloc(grau_max * sizeof(float));
        }

    }
    return gr;
}

/*
A função libera_Grafo libera toda a memória alocada durante a criação do grafo. Ela percorre cada vértice e
libera suas listas de adjacência, libera a matriz de pesos caso o grafo seja ponderado, libera o vetor de graus
 e por fim libera a própria estrutura do grafo, garantindo que não haja vazamento de memória.
*/
void libera_Grafo(Grafo* gr){
    if(gr != NULL){
        int i;
        for(i=0; i<gr->nro_vertices; i++)
            free(gr->arestas[i]);
        free(gr->arestas);

        if(gr->eh_ponderado){
            for(i=0; i<gr->nro_vertices; i++)
                free(gr->pesos[i]);
            free(gr->pesos);
        }
        free(gr->grau);
        free(gr);
    }
}

/*
A função insereAresta insere uma nova aresta na lista de adjacências do vértice de origem, adicionando o destino
na próxima posição livre e registrando o peso quando o grafo é ponderado. Após isso, incrementa o grau do vértice e,
se o grafo não for direcionado, insere automaticamente a aresta inversa, garantindo uma ligação bidirecional.
*/
int insereAresta(Grafo* gr, int orig, int dest, int eh_digrafo, float peso){
    if(gr == NULL)
        return 0;
    if(orig < 0 || orig >= gr->nro_vertices)
        return 0;
    if(dest < 0 || dest >= gr->nro_vertices)
        return 0;

    gr->arestas[orig][gr->grau[orig]] = dest;
    if(gr->eh_ponderado)
        gr->pesos[orig][gr->grau[orig]] = peso;
    gr->grau[orig]++;

    if(eh_digrafo == 0)
        insereAresta(gr,dest,orig,1,peso);
    return 1;
}

int removeAresta(Grafo* gr, int orig, int dest, int eh_digrafo){
    if(gr == NULL)
        return 0;
    if(orig < 0 || orig >= gr->nro_vertices)
        return 0;
    if(dest < 0 || dest >= gr->nro_vertices)
        return 0;

    int i = 0;
    while(i<gr->grau[orig] && gr->arestas[orig][i] != dest)
        i++;
    if(i == gr->grau[orig])//elemento nao encontrado
        return 0;
    gr->grau[orig]--;
    gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];
    if(gr->eh_ponderado)
        gr->pesos[orig][i] = gr->pesos[orig][gr->grau[orig]];
    if(eh_digrafo == 0)
        removeAresta(gr,dest,orig,1);
    return 1;
}
/*
A função removeAresta procura a aresta do vértice de origem que aponta para o destino e, ao encontrá-la,
remove-a substituindo-a pela última posição da lista, evitando deslocamentos. Em seguida, reduz o grau do vértice e,
caso o grafo seja não direcionado, remove também a aresta inversa, garantindo consistência na estrutura.
*/
void imprime_Grafo(Grafo *gr){
    if(gr == NULL)
        return;

    int i, j;
    for(i=0; i < gr->nro_vertices; i++){
        printf("%d: ", i);
        for(j=0; j < gr->grau[i]; j++){
            if(gr->eh_ponderado)
                printf("%d(%.2f), ", gr->arestas[i][j], gr->pesos[i][j]);
            else
                printf("%d, ", gr->arestas[i][j]);
        }
        printf("\n");
    }
}

//ex5
float algoritmo_Prim(Grafo *gr, int orig, int *pai) {
    if(gr == NULL || !gr->eh_ponderado)
        return -1;
    if(orig < 0 || orig >= gr->nro_vertices)
        return -1;

    int i, j;
    float custo_total = 0;

    // Vetores auxiliares
    int *visitado = (int*) calloc(gr->nro_vertices, sizeof(int));
    float *custo = (float*) malloc(gr->nro_vertices * sizeof(float));

    // Inicialização
    for(i = 0; i < gr->nro_vertices; i++) {
        custo[i] = __FLT_MAX__;  // Infinito
        pai[i] = -1;
    }

    // Vértice inicial
    custo[orig] = 0;

    // Processar todos os vértices
    for(int count = 0; count < gr->nro_vertices; count++) {
        // Encontrar o vértice não visitado com menor custo
        float menor_custo = __FLT_MAX__;
        int u = -1;

        for(i = 0; i < gr->nro_vertices; i++) {
            if(!visitado[i] && custo[i] < menor_custo) {
                menor_custo = custo[i];
                u = i;
            }
        }

        if(u == -1)  // Grafo desconexo
            break;

        // Marcar vértice como visitado
        visitado[u] = 1;
        custo_total += custo[u];

        // Atualizar custos dos vértices adjacentes
        for(j = 0; j < gr->grau[u]; j++) {
            int v = gr->arestas[u][j];
            float peso = gr->pesos[u][j];

            if(!visitado[v] && peso < custo[v]) {
                custo[v] = peso;
                pai[v] = u;
            }
        }
    }

    // Liberar memória
    free(visitado);
    free(custo);

    return custo_total;
}

void imprime_AGM_Prim(Grafo *gr, int orig) {
    if(gr == NULL || !gr->eh_ponderado) {
        printf("Erro: Grafo invalido ou nao ponderado!\n");
        return;
    }

    int *pai = (int*) malloc(gr->nro_vertices * sizeof(int));
    float custo_total = algoritmo_Prim(gr, orig, pai);

    if(custo_total < 0) {
        printf("Erro ao executar o algoritmo de Prim!\n");
        free(pai);
        return;
    }

    printf("\n=== Arvore Geradora Minima (Prim) ===\n");
    printf("Vertice inicial: %d\n\n", orig);
    printf("Arestas da AGM:\n");

    for(int i = 0; i < gr->nro_vertices; i++) {
        if(pai[i] != -1) {
            // Encontrar o peso da aresta
            float peso = 0;
            for(int j = 0; j < gr->grau[pai[i]]; j++) {
                if(gr->arestas[pai[i]][j] == i) {
                    peso = gr->pesos[pai[i]][j];
                    break;
                }
            }
            printf("%d -- %d (peso: %.2f)\n", pai[i], i, peso);
        }
    }

    printf("\nCusto total da AGM: %.2f\n", custo_total);
    free(pai);
}

//ex6
//a
int encontraNo(Grafo* gr, int no){
    if(gr == NULL)
        return 0;

    if(no >= 0 && no < gr->nro_vertices)
        return 1;

    return 0;
}

//b
int encontraMenorAresta(Grafo* gr, int* orig, int* dest, float* menor_peso){
    if(gr == NULL)
        return 0;

    if(gr->eh_ponderado == 0)
        return 0;

    int i, j;
    int encontrou = 0;
    float menor = 0.0;
    int vertice_orig = -1;
    int vertice_dest = -1;

    for(i = 0; i < gr->nro_vertices; i++){

        for(j = 0; j < gr->grau[i]; j++){

            if(encontrou == 0 || gr->pesos[i][j] < menor){
                menor = gr->pesos[i][j];
                vertice_orig = i;
                vertice_dest = gr->arestas[i][j];
                encontrou = 1;
            }
        }
    }

    if(encontrou){
        *orig = vertice_orig;
        *dest = vertice_dest;
        *menor_peso = menor;
        return 1;
    }

    return 0;
}
