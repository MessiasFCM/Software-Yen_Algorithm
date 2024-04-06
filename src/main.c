#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include "../include/file_io.h"
#include "../include/graph.h"
#include "../include/paths.h"
#include "../include/yen.h"

int main(int argc, char **argv) {
    struct timeval start, end;
    struct rusage usageStart, usageEnd;
    char *inputFileName = NULL, *outputFileName = NULL;
    int opt;

    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i': inputFileName = optarg; break;
            case 'o': outputFileName = optarg; break;
            default:
                fprintf(stderr, "Uso: %s -i <arquivo de entrada> -o <arquivo de saída>\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (!inputFileName || !outputFileName) {
        fprintf(stderr, "Arquivos de entrada e/ou saída não especificados!\n");
        return EXIT_FAILURE;
    }

    gettimeofday(&start, NULL);
    getrusage(RUSAGE_SELF, &usageStart);

    if (!processInputOutput(inputFileName, outputFileName)) {
        fprintf(stderr, "Falha no processamento dos arquivos.\n");
        return EXIT_FAILURE;
    }

    gettimeofday(&end, NULL);
    getrusage(RUSAGE_SELF, &usageEnd);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    double userTime = (usageEnd.ru_utime.tv_sec - usageStart.ru_utime.tv_sec) + 
                      (usageEnd.ru_utime.tv_usec - usageStart.ru_utime.tv_usec) / 1e6;
    double sysTime = (usageEnd.ru_stime.tv_sec - usageStart.ru_stime.tv_sec) + 
                     (usageEnd.ru_stime.tv_usec - usageStart.ru_stime.tv_usec) / 1e6;

    printf("\n-> Tempo de execução...\n");
    printf("# Tempo total de execução: %.3f segundos.\n", elapsed);
    printf("# Tempo de usuário: %.3f segundos.\n", userTime);
    printf("# Tempo de sistema: %.3f segundos.\n", sysTime);
    
    return EXIT_SUCCESS;
}
