// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "salva_jogo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNÇÃO A:Salva o jogo atual em arquivo texto
void salvar_jogo(Jogo *jogo, const char *arquivo) {
    FILE *arquivo_jogo = fopen(arquivo, "w");
    if (!arquivo_jogo) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }

    // Primeira linha: tamanho do tabuleiro, desfazer e trocas restantes
    fprintf(arquivo_jogo, "%d %d %d\n", jogo->tamanho_tab, jogo->desfazer_restantes, jogo->trocas_restantes);

    // Segunda linha: pontuação e nome do jogador
    fprintf(arquivo_jogo, "%d %s\n", jogo->pontos, jogo->nome);

    // Matriz atual
    for (int i = 0; i < jogo->tamanho_tab; i++) {
        for (int j = 0; j < jogo->tamanho_tab; j++) {
            fprintf(arquivo_jogo, "%d ", jogo->matriz_tab[i][j]);
        }
        fprintf(arquivo_jogo, "\n");
    }

    // Matriz da última jogada (tabuleiro anterior)
    for (int i = 0; i < jogo->tamanho_tab; i++) {
        for (int j = 0; j < jogo->tamanho_tab; j++) {
            fprintf(arquivo_jogo, "%d ", jogo->tab_anterior[i][j]);
        }
        fprintf(arquivo_jogo, "\n");
    }

    if (fclose(arquivo_jogo) == EOF) {
        printf("Erro ao fechar o arquivo! Jogo pode não ter sido salvo.\n");
    } else {
        printf("Jogo salvo com sucesso em %s!\n", arquivo);
    }
}

// FUNÇÃO B: Carrega um jogo salvo de um arquivo texto
void carregar_jogo(Jogo *jogo, const char *arquivo) {
    FILE *arquivo_jogo = fopen(arquivo, "r");
    if (!arquivo_jogo) {
        printf("Erro ao abrir o arquivo para carregar.\n");
        return;
    }

    // Lê informações iniciais
    fscanf(arquivo_jogo, "%d %d %d", &jogo->tamanho_tab, &jogo->desfazer_restantes, &jogo->trocas_restantes);
    fscanf(arquivo_jogo, "%d %s", &jogo->pontos, jogo->nome);

    int n = jogo->tamanho_tab;

    // Aloca matrizes se não estiverem alocadas
    jogo->matriz_tab = (int **)malloc(n * sizeof(int *));
    jogo->tab_anterior = (int **)malloc(n * sizeof(int *));
    if (!jogo->matriz_tab || !jogo->tab_anterior) {
        printf("Erro de alocação de memória.\n");
        fclose(arquivo_jogo);
        return;
    }

    for (int i = 0; i < n; i++) {
        jogo->matriz_tab[i] = (int *)malloc(n * sizeof(int));
        jogo->tab_anterior[i] = (int *)malloc(n * sizeof(int));
        if (!jogo->matriz_tab[i] || !jogo->tab_anterior[i]) {
            printf("Erro de alocação de memória.\n");
            fclose(arquivo_jogo);
            return;
        }
    }

    // Lê a matriz atual
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(arquivo_jogo, "%d", &jogo->matriz_tab[i][j]);
        }
    }

    // Lê a matriz da última jogada
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(arquivo_jogo, "%d", &jogo->tab_anterior[i][j]);
        }
    }

    if (fclose(arquivo_jogo) == EOF) {
        printf("Erro ao fechar o arquivo!\n");
    } else {
        printf("Jogo carregado com sucesso de %s!\n", arquivo);
    }
}

// FUNÇÃO C: Mostra o conteúdo de um arquivo de jogo sem alterar o estado atual
void ler_jogo(const char *arq) {
    FILE *arquivo_jogo = fopen(arq, "r");
    if (!arquivo_jogo) {
        printf("Erro ao abrir o arquivo %s.\n", arq);
        return;
    }

    int tamanho, ndesfazer, ntrocar, pontos;
    char nome[50];

    fscanf(arquivo_jogo, "%d %d %d", &tamanho, &ndesfazer, &ntrocar);
    fscanf(arquivo_jogo, "%d %s", &pontos, nome);

    printf("Tamanho: %d | Desfazer: %d | Trocar: %d\n", tamanho, ndesfazer, ntrocar);
    printf("Pontuação: %d | Jogador: %s\n", pontos, nome);

    printf("\nMatriz atual:\n");
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            int val;
            fscanf(arquivo_jogo, "%d", &val);
            printf("%4d", val);
        }
        printf("\n");
    }

    printf("\nMatriz última jogada:\n");
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            int val;
            fscanf(arquivo_jogo, "%d", &val);
            printf("%4d", val);
        }
        printf("\n");
    }

    if (fclose(arquivo_jogo) == EOF) {
        printf("Erro ao fechar o arquivo.\n");
    }
}
