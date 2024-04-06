#include "../include/graph.h"

// Função para criar um novo grafo
Graph* criarGrafo(int numVertices) {
    Graph* grafo = (Graph*)malloc(sizeof(Graph));
    grafo->numVertices = numVertices;
    grafo->vertices = (Vertex*)malloc(numVertices * sizeof(Vertex));

    for (int i = 0; i < numVertices; i++) {
        grafo->vertices[i].id = i;
        grafo->vertices[i].proxima = NULL;
    }

    return grafo;
}

// Função para adicionar uma aresta ao grafo
void adicionarAresta(Graph* grafo, int origem, int destino, int peso) {
    Edge* novaAresta = (Edge*)malloc(sizeof(Edge));
    novaAresta->destino = destino;
    novaAresta->peso = peso;
    novaAresta->proxima = grafo->vertices[origem].proxima;
    grafo->vertices[origem].proxima = novaAresta;
}

// Função para gerar o arquivo DOT para o Graphviz
void gerarArquivoDot(Graph* grafo) {
    FILE* arquivoDot = fopen("grafo.dot", "w");

    if (arquivoDot == NULL) {
        printf("Erro ao criar o arquivo DOT.\n");
        return;
    }

    fprintf(arquivoDot, "digraph G {\n");

    for (int i = 0; i < grafo->numVertices; i++) {
        Edge* arestaAtual = grafo->vertices[i].proxima;
        while (arestaAtual != NULL) {
            fprintf(arquivoDot, "\t%d -> %d [label=\"%d\"];\n", i + 1, arestaAtual->destino + 1, arestaAtual->peso);
            arestaAtual = arestaAtual->proxima;
        }
    }
    system("dot -Tpng grafo.dot -o grafo.png");
    fprintf(arquivoDot, "}\n");
    fclose(arquivoDot);
}

void liberarGrafo(Graph* grafo) {
    if (grafo == NULL) return;

    for (int i = 0; i < grafo->numVertices; i++) {
        Edge* aresta = grafo->vertices[i].proxima;
        while (aresta != NULL) {
            Edge* tmp = aresta;
            aresta = aresta->proxima;
            free(tmp); // Libera cada aresta individualmente
        }
    }
    free(grafo->vertices); // Libera o array de vértices
    free(grafo); // Libera o grafo
}

void removerAresta(Graph* grafo, int origem, int destino) {
    Edge **ptr = &(grafo->vertices[origem].proxima), *temp;
    while (*ptr != NULL) {
        if ((*ptr)->destino == destino) {
            temp = *ptr;
            *ptr = (*ptr)->proxima;
            free(temp); // Libera a aresta removida corretamente
            break; // Para a execução após remover a aresta
        }
        ptr = &((*ptr)->proxima);
    }
}

void imprimirGrafo(Graph* grafo) {
    printf("Estado atual do grafo com os pesos de cada aresta:\n");

    for (int v = 0; v < grafo->numVertices; v++) {
        printf("Vértice %d:", v);
        Edge* arestaAtual = grafo->vertices[v].proxima;
        while (arestaAtual != NULL) {
            printf(" -> %d (%d)", arestaAtual->destino, arestaAtual->peso);
            arestaAtual = arestaAtual->proxima;
        }
        printf("\n");
    }
}

void copiarGrafo(Graph* origem, Graph* destino) {
    // Certifica-se de que a memória atualmente alocada para destino->vertices seja liberada, se existir
    if (destino->vertices != NULL) {
        for (int i = 0; i < destino->numVertices; i++) {
            Edge* atual = destino->vertices[i].proxima;
            while (atual != NULL) {
                Edge* paraRemover = atual;
                atual = atual->proxima;
                free(paraRemover); // Libera a aresta atual
            }
        }
        free(destino->vertices); // Libera os vértices
    }
    
    // Continua com a operação de cópia
    destino->numVertices = origem->numVertices;
    destino->vertices = (Vertex*)calloc(origem->numVertices, sizeof(Vertex)); // Usando calloc para inicializar tudo com NULL
    if (destino->vertices == NULL) {
        fprintf(stderr, "Falha ao alocar memória para os vértices do grafo de destino.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < origem->numVertices; i++) {
        destino->vertices[i].proxima = NULL;
        Edge* atual = origem->vertices[i].proxima;
        Edge** ptrDestino = &(destino->vertices[i].proxima);

        while (atual != NULL) {
            *ptrDestino = (Edge*)malloc(sizeof(Edge));
            if (*ptrDestino == NULL) {
                // Falha ao alocar memória para uma aresta do grafo de destino
                fprintf(stderr, "Falha ao alocar memória para uma aresta do grafo de destino.\n");
                exit(EXIT_FAILURE);
            }
            (*ptrDestino)->destino = atual->destino;
            (*ptrDestino)->peso = atual->peso;
            (*ptrDestino)->proxima = NULL;
            ptrDestino = &((*ptrDestino)->proxima);
            atual = atual->proxima;
        }
    }
}

void restaurarGrafo(Graph* grafo, Graph* grafoTemporario) {
    int numVertices = grafo->numVertices;

    for (int i = 0; i < numVertices; i++) {
        grafoTemporario->vertices[i].custo = grafo->vertices[i].custo;
        grafoTemporario->vertices[i].noAnterior = grafo->vertices[i].noAnterior;

        // Remove todas as arestas do grafo temporário
        Edge* atualTemp = grafoTemporario->vertices[i].proxima;
        while (atualTemp != NULL) {
            Edge* temp = atualTemp;
            atualTemp = atualTemp->proxima;
            free(temp);
        }
        grafoTemporario->vertices[i].proxima = NULL;

        // Copia as arestas do grafo original de volta para o grafo temporário
        Edge* atualOriginal = grafo->vertices[i].proxima;
        Edge* anterior = NULL;
        while (atualOriginal != NULL) {
            Edge* novaAresta = (Edge*)malloc(sizeof(Edge));
            novaAresta->destino = atualOriginal->destino;
            novaAresta->peso = atualOriginal->peso;
            novaAresta->proxima = NULL;

            // Insere a nova aresta no grafo temporário
            if (anterior == NULL) {
                grafoTemporario->vertices[i].proxima = novaAresta;
            } else {
                anterior->proxima = novaAresta;
            }

            anterior = novaAresta;
            atualOriginal = atualOriginal->proxima;
        }
    }
}
