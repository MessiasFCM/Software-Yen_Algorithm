#ifndef YEN_H
#define YEN_H

#include "dijkstra.h"
#include "paths.h"

ShortestPaths* yen(Graph* grafo, int origem, int k);
void removerArestasYen(Graph* grafoCopia, ShortestPaths* caminhos, ShortestPath* rootPath, int spurNode);
void definirRootPath(Graph* grafo, ShortestPath* root);
void imprimirCaminho(ShortestPath* caminho);

#endif
