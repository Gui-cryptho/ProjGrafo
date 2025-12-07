#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h"

int main()
{
    int eh_digrafo = 1;
    Grafo* gr = cria_Grafo(5, 5, 0);
    insereAresta(gr, 0, 1, eh_digrafo, 0);
    insereAresta(gr, 1, 3, eh_digrafo, 0);
    insereAresta(gr, 1, 2, eh_digrafo, 0);
    insereAresta(gr, 2, 4, eh_digrafo, 0);
    insereAresta(gr, 3, 0, eh_digrafo, 0);
    insereAresta(gr, 3, 4, eh_digrafo, 0);
    insereAresta(gr, 4, 1, eh_digrafo, 0);
    imprime_Grafo(gr);

    printf("\nBusca de nos:\n");
    int nos_buscar[] = {0, 2, 4, 7};
    int i;
    for(i = 0; i < 4; i++)
    {
        if(encontraNo(gr, nos_buscar[i]))
            printf("No %d existe no grafo\n", nos_buscar[i]);
        else
            printf("No %d NAO existe no grafo\n", nos_buscar[i]);
    }

    printf("\nBusca de menor aresta:\n");
    int orig, dest;
    float peso;
    if(encontraMenorAresta(gr, &orig, &dest, &peso))
    {
        printf("Aresta de menor peso: %d -> %d (peso: %.2f)\n", orig, dest, peso);
    }
    else
    {
        printf("Grafo nao eh ponderado ou nao possui arestas\n");
    }

    libera_Grafo(gr);

    // ========== TESTE DO ALGORITMO DE PRIM ==========
    printf("\n\n========== ALGORITMO DE PRIM ==========\n");
    printf("Criando grafo ponderado nao direcionado para AGM...\n\n");

    // Criar um grafo ponderado não direcionado
    Grafo* gr_prim = cria_Grafo(6, 6, 1);

    // Inserir arestas com pesos (grafo não direcionado: eh_digrafo = 0)
    insereAresta(gr_prim, 0, 1, 0, 4.0);
    insereAresta(gr_prim, 0, 2, 0, 3.0);
    insereAresta(gr_prim, 1, 2, 0, 1.0);
    insereAresta(gr_prim, 1, 3, 0, 2.0);
    insereAresta(gr_prim, 2, 3, 0, 4.0);
    insereAresta(gr_prim, 2, 4, 0, 5.0);
    insereAresta(gr_prim, 3, 4, 0, 1.0);
    insereAresta(gr_prim, 3, 5, 0, 6.0);
    insereAresta(gr_prim, 4, 5, 0, 2.0);

    printf("Grafo ponderado:\n");
    imprime_Grafo(gr_prim);

    // Executar algoritmo de Prim
    imprime_AGM_Prim(gr_prim, 0);

    libera_Grafo(gr_prim);

    system("pause");
    return 0;
}
