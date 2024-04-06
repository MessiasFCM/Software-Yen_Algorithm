#include "../include/file_io.h"
#include "../include/yen.h"
#include "../include/paths.h"
#include <stdio.h>

int processInputOutput(const char *inputFileName, const char *outputFileName) {
    // Abre e lê o arquivo de entrada
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return 0; // Falha
    }

    int numCidades, numVoos, k, cidadeOrigem = 1;
    if (fscanf(inputFile, "%d %d %d", &numCidades, &numVoos, &k) != 3) {
        fprintf(stderr, "Formato de arquivo de entrada inválido.\n");
        fclose(inputFile);
        return 0; // Falha
    }

    // Verificações de limites para n, m e k
    if (numCidades < 2 || numCidades > 100000 || numVoos < 1 || numVoos > 200000 || k < 1 || k > 10) {
        fprintf(stderr, "Os valores de n, m ou k estão fora dos limites permitidos.\n");
        fclose(inputFile);
        return 0; // Falha
    }

    Graph* grafo = criarGrafo(numCidades);
    for (int i = 0; i < numVoos; i++) {
        int cidadeA, cidadeB, preco;
        if (fscanf(inputFile, "%d %d %d", &cidadeA, &cidadeB, &preco) != 3) {
            fprintf(stderr, "Erro ao ler os dados da aresta.\n");
            fclose(inputFile);
            liberarGrafo(grafo);
            return 0; // Falha
        }

        if (cidadeA < 1 || cidadeA > numCidades || cidadeB < 1 || cidadeB > numCidades || preco < 1 || preco > 1000000000) {
            fprintf(stderr, "Os valores de a, b ou c estão fora dos limites permitidos.\n");
            fclose(inputFile);
            liberarGrafo(grafo);
            return 0; // Falha
        }
        adicionarAresta(grafo, cidadeA - 1, cidadeB - 1, preco);
    }

    fclose(inputFile);

    imprimirGrafo(grafo);

    // Processamento do algoritmo de Yen
    ShortestPaths* caminhos = yen(grafo, cidadeOrigem - 1, k);
    if (!caminhos || !caminhos->cabeca) {
        fprintf(stderr, "Erro ao executar o algoritmo de Yen.\n");
        liberarGrafo(grafo);
        return 0; // Falha
    }

    // Imprime os custos dos caminhos na tela e escreve no arquivo de saída
    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        liberarGrafo(grafo);
        liberarListaDeCaminhos(caminhos);
        return 0; // Falha
    }

    inverterListaCaminhos(caminhos);

    ShortestPath* atual = caminhos->cabeca;
    while (atual != NULL) {
        fprintf(outputFile, "%d ", atual->custo);
        atual = atual->prox;
    }

    fclose(outputFile);

    // Liberação de recursos
    liberarGrafo(grafo);
    liberarListaDeCaminhos(caminhos);

    return 1; // Sucesso
}
