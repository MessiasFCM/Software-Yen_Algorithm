#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Estrutura de uma aresta do grafo
typedef struct Edge {
    int destino;
    int peso;
    struct Edge* proxima;
} Edge;

// Estrutura de um vértice do grafo
typedef struct Vertex {
    int id;
    Edge* proxima;
    int custo;
    int noAnterior;
} Vertex;

// Estrutura do grafo
typedef struct Graph {
    int numVertices;
    Vertex* vertices;
} Graph;

Graph* criarGrafo(int numVertices);
void adicionarAresta(Graph* grafo, int origem, int destino, int peso);
void liberarGrafo(Graph* grafo);
void removerAresta(Graph *grafo, int src, int dest);
void imprimirGrafo(Graph* grafo);
void copiarGrafo(Graph* grafoOrigem, Graph* grafoDestino);
void restaurarGrafo(Graph* grafo, Graph* grafoTemporario);

#endif
