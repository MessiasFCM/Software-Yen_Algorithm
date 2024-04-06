#include "../include/dijkstra.h"
#include "../include/paths.h"
#include <stdio.h>
#include <limits.h>

void relaxar(NoHeap* heap, int* tamHeap, InfoCaminho* caminhos, int src, int dest, int peso) {
    int novoCustoCaminho = caminhos[src].custo + peso;

    // Atualiza se encontrar um caminho mais curto
    if (novoCustoCaminho < caminhos[dest].custo || caminhos[dest].custo == -1) {
        caminhos[dest].custo = novoCustoCaminho;
        caminhos[dest].noAnterior = src;

        // Verifica se o nó destino já está no heap
        if (!estaNoHeap(heap, *tamHeap, dest)) {
            inserirNoHeap(heap, tamHeap, dest, novoCustoCaminho);
        }
    }
}
int estaNoHeap(NoHeap* heap, int tamHeap, int no) {
    for (int i = 0; i < tamHeap; i++) {
        if (heap[i].no == no) {
            return 1;
        }
    }
    return 0;
}

void inserirNoHeap(NoHeap* heap, int* tamHeap, int no, int custo) {
    heap[*tamHeap].no = no;
    heap[*tamHeap].custo = custo;
    (*tamHeap)++;
}

NoHeap extrairMin(NoHeap* heap, int* tamHeap) {
    NoHeap min = heap[0];
    heap[0] = heap[*tamHeap - 1];
    (*tamHeap)--;
    heapMinimo(heap, 0, *tamHeap);
    return min;
}

void heapMinimo(NoHeap* heap, int i, int tamHeap) {
    int menor = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < tamHeap && heap[esquerda].custo < heap[menor].custo) {
        menor = esquerda;
    }
    if (direita < tamHeap && heap[direita].custo < heap[menor].custo) {
        menor = direita;
    }

    if (menor != i) {
        NoHeap temp = heap[i];
        heap[i] = heap[menor];
        heap[menor] = temp;
        heapMinimo(heap, menor, tamHeap);
    }
}

ShortestPath* dijkstra(Graph* grafo, int origem) {
    int numVertices = grafo->numVertices;
    InfoCaminho* caminhos = (InfoCaminho*)malloc(numVertices * sizeof(InfoCaminho));
    NoHeap* heap = (NoHeap*)malloc(numVertices * sizeof(NoHeap));
    int tamHeap = 0;
    // Inicializa os caminhos
    for (int i = 0; i < numVertices; i++) {
        caminhos[i].custo = INT_MAX;
        caminhos[i].noAnterior = -1;
    }
    caminhos[origem].custo = 0;
    inserirNoHeap(heap, &tamHeap, origem, 0);
    while (tamHeap > 0) {
        NoHeap minNo = extrairMin(heap, &tamHeap);
        int noAtual = minNo.no;

        Edge* arestaAtual = grafo->vertices[noAtual].proxima;
        while (arestaAtual != NULL) {
            relaxar(heap, &tamHeap, caminhos, noAtual, arestaAtual->destino, arestaAtual->peso);
            arestaAtual = arestaAtual->proxima;
        }
    }

    // Verificar se o destino foi alcançado
    if (caminhos[numVertices - 1].custo == INT_MAX) {
        printf("Destino não pode ser alcançado a partir da origem %d.\n", origem);
        free(caminhos);
        free(heap);
        return NULL;
    }

    // Criar e preencher a estrutura MenorCaminho*
    ShortestPath* shortestPath = (ShortestPath*)malloc(sizeof(ShortestPath));
    shortestPath->caminho = (int*)malloc(numVertices * sizeof(int));
    shortestPath->comprimentoCaminho = 0;
    shortestPath->custo = caminhos[numVertices - 1].custo; // Custo do menor caminho até o último vértice
    // Preencher o caminho
    int temp = numVertices - 1;
    while (temp != -1 && temp != origem) {
        // Verificar se o comprimentoCaminho está dentro dos limites do vetor
        if (shortestPath->comprimentoCaminho < numVertices) {
            shortestPath->caminho[shortestPath->comprimentoCaminho++] = temp;
        } else {
            printf("Erro: comprimentoCaminho excede o tamanho do vetor caminho\n");
            break;
        }
        temp = caminhos[temp].noAnterior;
    }
    
    // Verificar se o índice final foi adicionado corretamente
    if (shortestPath->comprimentoCaminho < numVertices) {
        shortestPath->caminho[shortestPath->comprimentoCaminho++] = origem;
    } else {
        printf("Erro: comprimentoCaminho excede o tamanho do vetor caminho\n");
    }

    // Inverter o vetor caminho para ficar na ordem correta
    for (int i = 0; i < shortestPath->comprimentoCaminho / 2; i++) {
        int temp = shortestPath->caminho[i];
        shortestPath->caminho[i] = shortestPath->caminho[shortestPath->comprimentoCaminho - i - 1];
        shortestPath->caminho[shortestPath->comprimentoCaminho - i - 1] = temp;
    }

    printf("Caminho invertido:\n");
    for (int i = 0; i < shortestPath->comprimentoCaminho; i++) {
        printf("%d ", shortestPath->caminho[i]);
    }
    printf("Custo: %d", shortestPath->custo);
    printf("\n");

    free(caminhos);
    free(heap);

    return shortestPath;
}
