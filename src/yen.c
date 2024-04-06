#include "../include/yen.h"
#include "../include/graph.h"
#include "../include/paths.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

void imprimirCaminho(ShortestPath* caminho) {
    printf("Caminho: ");
    for (int i = 0; i < caminho->comprimentoCaminho; i++) {
        printf("%d ", caminho->caminho[i]);
    }
    printf(" - Custo: %d\n", caminho->custo);
}

void removerArestasYen(Graph* grafoCopia, ShortestPaths* caminhos, ShortestPath* rootPath, int spurNode) {
    removerAresta(grafoCopia, caminhos->cabeca->caminho[spurNode-1], caminhos->cabeca->caminho[spurNode]);
    ShortestPath* aux = caminhos->cabeca->prox;
    while (aux != NULL) {
        int igual = 1;
        for (int i = 0; i < rootPath->comprimentoCaminho; i++) {
            if (i >= aux->comprimentoCaminho || aux->caminho[i] != rootPath->caminho[i]) {
                igual = 0;
                break;
            }
        }
        if (igual == 1 && spurNode < aux->comprimentoCaminho) {
            removerAresta(grafoCopia, aux->caminho[spurNode-1], aux->caminho[spurNode]);
        }
        aux = aux->prox;
    }
}

void definirRootPath(Graph* grafo, ShortestPath* root) {
    if (grafo == NULL || root == NULL || root->comprimentoCaminho <= 0) return;
    
    for (int i = 0; i < root->comprimentoCaminho - 1; i++) {
        Edge **ptr = &(grafo->vertices[root->caminho[i]].proxima);
        while (*ptr != NULL) {
            if ((*ptr)->destino != root->caminho[i + 1]) {
                Edge* arestaRemovida = *ptr;
                *ptr = (*ptr)->proxima;
                free(arestaRemovida);
            } else {
                ptr = &(*ptr)->proxima;
            }
        }
    }
}

ShortestPaths* yen(Graph* grafo, int origem, int k) {
    ShortestPaths* caminhos = (ShortestPaths*)malloc(sizeof(ShortestPaths));
    caminhos->cabeca = NULL;
    inserirMenorCaminho(caminhos, dijkstra(grafo, origem));

    ShortestPaths *caminhosAuxiliares = alocarListaCaminhos();

    for (int i = 0; i < k - 1; i++) {
        printf("Iteração %d:\n", i);
        printf("Caminhos na Iteração %d:\n", i);
        imprimirCaminhos(caminhos);
        
        for (int j = 1; j < caminhos->cabeca->comprimentoCaminho; j++) {
            printf("Gerando caminhos auxiliares para segmento %d do caminho principal...\n", j);
            int* vetor = (int*)malloc(j * sizeof(int));
            for (int r = 0; r < j; r++) {
                vetor[r] = caminhos->cabeca->caminho[r];
            }
            Graph* grafoTemporario = criarGrafo(grafo->numVertices);
            restaurarGrafo(grafo, grafoTemporario);
            ShortestPath* rootPath = criarCaminho(vetor, j, 0);
            // Chamada modificada aqui
            // Chama definirRootPath antes de removerArestasYen
            definirRootPath(grafoTemporario, rootPath);
            removerArestasYen(grafoTemporario, caminhos, rootPath, j);
            
            printf("RootPath encontrado: ");
            imprimirCaminho(rootPath);
            imprimirGrafo(grafoTemporario);
            ShortestPath* caminho = dijkstra(grafoTemporario, origem);
            
            if (caminho != NULL && !caminhoJaExiste(caminhos, caminhosAuxiliares, caminho)) {
                printf("Novo caminho auxiliar encontrado.\n");
                imprimirCaminho(caminho);
                inserirMenorCaminho(caminhosAuxiliares, caminho);
            } else {
                printf("Caminho já existe nos caminhos principais ou auxiliares. Ignorando...\n");
                if (caminho != NULL) {
                    imprimirCaminho(caminho);
                    liberarCaminho(caminho);
                }
            }
            free(vetor);
            liberarCaminho(rootPath);
            liberarGrafo(grafoTemporario);
        }

        printf("Caminhos Auxiliares na Iteração %d:\n", i+1);
        imprimirCaminhos(caminhosAuxiliares);

        ShortestPath* minimo = NULL;
        int menorCusto = INT_MAX;
        ShortestPath* atual = caminhosAuxiliares->cabeca;
        while (atual != NULL) {
            if (atual->custo < menorCusto && !verificaCaminho(caminhos, atual)) {
                minimo = atual;
                menorCusto = atual->custo;
            }
            imprimirCaminho(atual);
            atual = atual->prox;
        }

        if (minimo != NULL) {
            printf("Inserindo o menor caminho auxiliar encontrado na lista de caminhos principais...\n");
            ShortestPath* copiaMinimo = criarCaminho(minimo->caminho, minimo->comprimentoCaminho, minimo->custo);
            inserirMenorCaminho(caminhos, copiaMinimo);
            //removerCaminhoEspecifico(caminhosAuxiliares, minimo);
        }
    }

    liberarListaDeCaminhos(caminhosAuxiliares);
    return caminhos;
}

