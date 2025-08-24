//  Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "salva_jogo.h"
#include "exemplo.h"

// Função 1: Salva o jogo em arquivo de texto
void salvar_jogo(Jogo *jogo, const char *arquivo) {
    if (!jogo) {
        printf(VERMELHO "Erro: ponteiro do jogo inválido.\n" RESET);
        return;
    }

    FILE *arquivo_jogo = fopen(arquivo, "w");
    if (!arquivo_jogo) {
        printf(VERMELHO "Erro ao abrir o arquivo para salvar.\n" RESET);
        return;
    }

    // Primeiro linha: tamanho do tabuleiro, desfazer, trocar
    fprintf(arquivo_jogo, "%d %d %d\n", jogo->tamanho_tab, jogo->desfazer_restantes, jogo->trocas_restantes);

    // Segunda linha: pontos e nome
    fprintf(arquivo_jogo, "%d %s\n", jogo->pontos, jogo->nome);

    // Matrizes do jogo e última jogada
    for (int i = 0; i < jogo->tamanho_tab; i++) {
        for (int j = 0; j < jogo->tamanho_tab; j++)
            fprintf(arquivo_jogo, "%d ", jogo->matriz_tab[i][j]);
        fprintf(arquivo_jogo, "\n");
    }

    for (int i = 0; i < jogo->tamanho_tab; i++) {
        for (int j = 0; j < jogo->tamanho_tab; j++)
            fprintf(arquivo_jogo, "%d ", jogo->tab_anterior[i][j]);
        fprintf(arquivo_jogo, "\n");
    }

    fclose(arquivo_jogo);
    printf(VERDE "Jogo salvo com sucesso em %s!\n" RESET, arquivo);
}

// Função 2: Carrega o jogo de arquivo e exibe o tabuleiro colorido
void carregar_jogo(Jogo *jogo, const char *arquivo) {
    FILE *arquivo_jogo = fopen(arquivo, "r");
    if (!arquivo_jogo) {
        printf(VERMELHO "Erro ao abrir o arquivo para carregar.\n" RESET);
        return;
    }

    // Lê tamanho, desfazer e trocas
    fscanf(arquivo_jogo, "%d %d %d", &jogo->tamanho_tab, &jogo->desfazer_restantes, &jogo->trocas_restantes);

    // Lê a linha completa (pontos + nome)
    char linha[200];
    fgets(linha, sizeof(linha), arquivo_jogo);
    linha[strcspn(linha, "\n")] = '\0';

    // Separar pontos do nome
    int i = 0;
    while (linha[i] >= '0' && linha[i] <= '9') i++;
    linha[i] = '\0';
    jogo->pontos = atoi(linha);
    strcpy(jogo->nome, linha + i + 1); // copia o nome

    // Aloca matrizes
    int n = jogo->tamanho_tab;
    jogo->matriz_tab = (int **)malloc(n * sizeof(int *));
    jogo->tab_anterior = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        jogo->matriz_tab[i] = (int *)malloc(n * sizeof(int));
        jogo->tab_anterior[i] = (int *)malloc(n * sizeof(int));
    }

    // Lê matriz atual
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            fscanf(arquivo_jogo, "%d", &jogo->matriz_tab[i][j]);

    // Lê matriz anterior
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            fscanf(arquivo_jogo, "%d", &jogo->tab_anterior[i][j]);

    fclose(arquivo_jogo);
    printf(AZUL "Jogo carregado com sucesso de %s!\n" RESET, arquivo);
}

//Função 3: Lê um jogo diretamente do arquivo e mostra 
void ler_jogo(const char *arq) {
    FILE *arquivo_jogo = fopen(arq, "r");
    if (!arquivo_jogo) {
        printf(VERMELHO "Erro ao abrir o arquivo %s.\n" RESET, arq);
        return;
    }

    int tamanho, ndesfazer, ntrocar, pontos;
    char nome[100];

    // Lê tamanho, desfazer e trocas
    fscanf(arquivo_jogo, "%d %d %d", &tamanho, &ndesfazer, &ntrocar);

    // Lê linha completa com pontos + nome
    char linha[200];
    fgets(linha, sizeof(linha), arquivo_jogo);
    linha[strcspn(linha, "\n")] = '\0';

    int i = 0;
    while (linha[i] >= '0' && linha[i] <= '9') i++;
    linha[i] = '\0';
    pontos = atoi(linha);
    strcpy(nome, linha + i + 1);

    printf(CIANA "Tamanho: %d | Desfazer: %d | Trocar: %d\n" RESET, tamanho, ndesfazer, ntrocar);
    printf(VERDE "Pontuação: %d | Jogador: %s\n" RESET, pontos, nome);

    int valor;
    printf("\nMatriz atual:\n");
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fscanf(arquivo_jogo, "%d", &valor);
            printf("%4d", valor);
        }
        printf("\n");
    }

    printf("\nMatriz última jogada:\n");
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fscanf(arquivo_jogo, "%d", &valor);
            printf("%4d", valor);
        }
        printf("\n");
    }

    fclose(arquivo_jogo);
}