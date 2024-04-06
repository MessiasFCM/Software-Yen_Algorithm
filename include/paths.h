#ifndef PATHS_H
#define PATHS_H
#include <stdbool.h>
#include "graph.h"

typedef struct ShortestPath{
    int* caminho;
    int comprimentoCaminho;
    int custo;
    struct ShortestPath* prox;
} ShortestPath;

typedef struct ShortestPaths{
    ShortestPath* cabeca;
} ShortestPaths;

ShortestPaths* alocarListaCaminhos();
void liberarListaDeCaminhos(ShortestPaths* lista);
void liberarCaminho(ShortestPath* caminho);
ShortestPath* criarCaminho(int *caminho, int tamanhoCaminho, int custo);
void inserirMenorCaminho(ShortestPaths* listaCaminhos, ShortestPath* novoCaminho);
void removerCaminhoEspecifico(ShortestPaths* caminhos, ShortestPath* caminho);
void removerProximaArestaCaminhoPrincipal(Graph* grafoTemporario, ShortestPaths* caminhos, int i);
bool verificaCaminho(ShortestPaths* caminhos, ShortestPath* caminhoTeste);
void removerArestasNaoPertencentesAoCaminho(Graph* grafo, ShortestPath* root);
ShortestPath* buscaMenorCustoNaLista(ShortestPaths* caminhos);
void inverterListaCaminhos(ShortestPaths* caminhos);
void imprimirCaminhos(ShortestPaths* caminhos);
void removerCaminho(ShortestPaths* caminhos, ShortestPath* caminho);
bool caminhoJaExiste(ShortestPaths* caminhosPrincipais, ShortestPaths* caminhosAuxiliares, ShortestPath* caminhoEncontrado);

#endif