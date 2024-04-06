#include "../include/paths.h"
#include "../include/graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

ShortestPaths* alocarListaCaminhos() {
    // Aloca espaço para a estrutura ShortestPaths
    ShortestPaths* listaCaminhos = (ShortestPaths*)malloc(sizeof(ShortestPaths));
    if (listaCaminhos == NULL) {
        // Tratamento de erro se a alocação falhar
        printf("Erro ao alocar memória para a lista de MenoresCaminhos.\n");
        exit(EXIT_FAILURE);
    }

    listaCaminhos->cabeca = NULL; // Inicializa a cabeça da lista como NULL

    return listaCaminhos;
}


void liberarListaDeCaminhos(ShortestPaths* lista) {
    ShortestPath* atual = lista->cabeca;
    while (atual != NULL) {
        ShortestPath* proximo = atual->prox;
        free(atual->caminho);
        free(atual);
        atual = proximo;
    }
    free(lista);
}

void liberarCaminho(ShortestPath* caminho) {
    if (caminho != NULL) {
        free(caminho->caminho);
        free(caminho);
    }
}

ShortestPath* criarCaminho(int *caminho, int tamanhoCaminho, int custo) {
    int* copia = (int*)malloc(tamanhoCaminho * sizeof(int));
    if (!copia) return NULL; // Falha na alocação de memória

    memcpy(copia, caminho, tamanhoCaminho * sizeof(int));
    ShortestPath* novo = (ShortestPath*)malloc(sizeof(ShortestPath));
    if (!novo) {
        free(copia); // Liberar a memória do novo caminho em caso de falha na alocação
        return NULL;
    }

    novo->caminho = copia;
    novo->comprimentoCaminho = tamanhoCaminho;
    novo->custo = custo;
    novo->prox = NULL;

    return novo;
}

ShortestPath* buscaMenorCustoNaLista(ShortestPaths* caminhos) {
    ShortestPath* atual = caminhos->cabeca;
    ShortestPath* menorCusto = atual;
    while (atual != NULL) {
        if (atual->custo < menorCusto->custo) {
            menorCusto = atual;
        }
        atual = atual->prox;
    }
    return menorCusto;
}

void inserirMenorCaminho(ShortestPaths* listaCaminhos, ShortestPath* novoCaminho) {
    if (listaCaminhos == NULL || novoCaminho == NULL) {
        printf("Erro: Lista de Menores Caminhos ou novo caminho inválidos.\n");
        return;
    }

    novoCaminho->prox = listaCaminhos->cabeca; 
    listaCaminhos->cabeca = novoCaminho; 
}

void removerCaminhoEspecifico(ShortestPaths* lista, ShortestPath* caminhoParaRemover) {
    ShortestPath* atual = lista->cabeca;
    ShortestPath* anterior = NULL;

    while (atual != NULL) {
        if (atual == caminhoParaRemover) { // Encontramos o caminho a ser removido
            if (anterior == NULL) {
                // O caminho a ser removido é o primeiro da lista
                lista->cabeca = atual->prox;
            } else {
                // O caminho a ser removido está no meio ou no final da lista
                anterior->prox = atual->prox;
            }
            // Não liberamos o caminho aqui pois ele será utilizado em outro lugar
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

void removerProximaArestaCaminhoPrincipal(Graph* grafoTemporario, ShortestPaths* caminhos, int i) {
    if (i >= caminhos->cabeca->comprimentoCaminho - 1) {
        // Não há aresta "próxima" para remover se estivermos no último vértice do caminho
        return;
    }
    
    int origem = caminhos->cabeca->caminho[i];
    int destino = caminhos->cabeca->caminho[i + 1];
    
    removerAresta(grafoTemporario, origem, destino);
}

bool verificaCaminho(ShortestPaths* caminhos, ShortestPath* caminhoTeste) {
    ShortestPath* atual = caminhos->cabeca;
    while (atual != NULL) {
        // Comparar os comprimentos dos caminhos
        if (atual->comprimentoCaminho == caminhoTeste->comprimentoCaminho) {
            // Verificar se os caminhos são idênticos
            bool iguais = true;
            for (int i = 0; i < atual->comprimentoCaminho; i++) {
                if (atual->caminho[i] != caminhoTeste->caminho[i]) {
                    iguais = false;
                    break;
                }
            }
            // Se os caminhos são idênticos, retornar true
            if (iguais) return true;
        }
        atual = atual->prox;
    }
    // Se nenhum caminho idêntico foi encontrado, retornar false
    return false;
}


void removerArestasNaoPertencentesAoCaminho(Graph* grafo, ShortestPath* caminho) {
    if (grafo == NULL || caminho == NULL || caminho->comprimentoCaminho <= 0) return;
    // Percorre cada vértice no caminho, exceto o último
    int indiceVertice = 0;
    while (indiceVertice < caminho->comprimentoCaminho - 1) {
        int idOrigem = caminho->caminho[indiceVertice];
        int idDestino = caminho->caminho[indiceVertice + 1];
        
        // Procura a aresta que conecta o vértice de origem ao vértice de destino
        Edge **ponteiroAresta = &grafo->vertices[idOrigem].proxima;
        while (*ponteiroAresta != NULL) {
            if ((*ponteiroAresta)->destino == idDestino) {
                ponteiroAresta = &(*ponteiroAresta)->proxima;
            } else {
                // Remove a aresta que não faz parte do caminho
                Edge* arestaRemovida = *ponteiroAresta;
                *ponteiroAresta = (*ponteiroAresta)->proxima; // Atualiza o ponteiro para pular a aresta removida
                free(arestaRemovida);
                break; // Sai do loop após remover a aresta
            }
        }
        indiceVertice++;
    }
}

void inverterListaCaminhos(ShortestPaths* caminhos) {
    ShortestPath* prev = NULL;
    ShortestPath* current = caminhos->cabeca;
    ShortestPath* next = NULL;

    while (current != NULL) {
        next = current->prox;  // Salva o próximo
        current->prox = prev;  // Inverte o ponteiro do atual
        prev = current;        // Move prev para frente
        current = next;        // Move current para frente
    }
    caminhos->cabeca = prev;  // Atualiza a cabeça da lista para o novo primeiro elemento
}

void imprimirCaminhos(ShortestPaths* caminhos) {
    if (caminhos == NULL || caminhos->cabeca == NULL) {
        printf("Lista de caminhos vazia.\n");
        return;
    }

    ShortestPath* atual = caminhos->cabeca;
    int index = 1;

    while (atual != NULL) {
        printf("Caminho %d: Comprimento = %d, Custo = %d\n", index, atual->comprimentoCaminho, atual->custo);
        printf("Caminho:");
        for (int i = 0; i < atual->comprimentoCaminho; i++) {
            printf(" %d", atual->caminho[i]);
        }
        printf("\n");

        atual = atual->prox;
        index++;
    }
}

void removerCaminho(ShortestPaths* caminhos, ShortestPath* caminho) {
    if (caminhos == NULL || caminho == NULL) {
        printf("Erro: Lista de caminhos ou caminho inválido.\n");
        return;
    }

    ShortestPath* atual = caminhos->cabeca;
    ShortestPath* anterior = NULL;

    while (atual != NULL) {
        if (atual == caminho) { // Encontramos o caminho a ser removido
            if (anterior == NULL) {
                // O caminho a ser removido é o primeiro da lista
                caminhos->cabeca = atual->prox;
            } else {
                // O caminho a ser removido está no meio ou no final da lista
                anterior->prox = atual->prox;
            }
            // Libera a memória alocada para o caminho removido
            liberarCaminho(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }

    printf("Erro: Caminho não encontrado na lista.\n");
}

bool caminhoJaExiste(ShortestPaths* caminhosPrincipais, ShortestPaths* caminhosAuxiliares, ShortestPath* caminhoEncontrado) {
    // Verifica se o caminho já existe na lista principal
    if (verificaCaminho(caminhosPrincipais, caminhoEncontrado)) {
        return true;
    }
    // Verifica se o caminho já existe na lista auxiliar
    if (verificaCaminho(caminhosAuxiliares, caminhoEncontrado)) {
        return true;
    }

    return false;
}


