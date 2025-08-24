//  Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "salva_jogo.h"
#include "exemplo.h"

// Função 1: Retorna a cor do valor do tabuleiro
const char* cor_valor(int valor) {
    if (valor == 2) return VERMELHO;
    if (valor == 4) return VERDE;
    if (valor == 8) return AMARELO;
    if (valor == 16) return AZUL;
    if (valor == 32) return MAGENTA;
    if (valor == 64) return CIANA;
    return NEGRITO; // valores maiores
}

// Função 2: Mostra o tabuleiro colorido e com cabeçalho
void imprimir_matriz(Jogo *jogo) {
    int n = jogo->tamanho_tab;

    // Cabeçalho
    printf("\n");
    printf(CIANA "╔══════════════════════════════════════════════╗\n" RESET);
    printf(CIANA "║" AMARELO "               🎮 JOGO 2048 🎮               " CIANA "║\n" RESET);
    printf(CIANA "╠══════════════════════════════════════════════╣\n" RESET);
    
    // Informações do jogador
    printf(CIANA "║" VERDE " Jogador: %-30s " CIANA "║\n" RESET, jogo->nome);
    printf(CIANA "║" AZUL " Pontos: %-4d " MAGENTA "Trocas: %-2d " VERMELHO "Desfazer: %-2d " CIANA "║\n" RESET,
           jogo->pontos, jogo->trocas_restantes, jogo->desfazer_restantes);
    printf(CIANA "╚══════════════════════════════════════════════╝\n" RESET);
    printf("\n");

    // Cabeçalho das colunas
    printf("    ");
    for (int j = 0; j < n; j++)
        printf(NEGRITO "%4d " RESET, j + 1);
    printf("\n");

    // Imprime linhas do tabuleiro
    for (int i = 0; i < n; i++) {
        printf(NEGRITO "%c " RESET, 'A' + i); // letra da linha
        printf("|");
        for (int j = 0; j < n; j++) {
            int val = jogo->matriz_tab[i][j];
            if (val == 0) printf("   . ");
            else printf("%s%4d " RESET, cor_valor(val), val);
        }
        printf("|\n");
    }
    printf("\n");
}

// Função 3: Salva o jogo em arquivo de texto
void salvar_jogo(Jogo *jogo, const char *arquivo) {
    if (!jogo) {
        printf(VERMELHO "❌ Ponteiro do jogo inválido!\n" RESET);
        return;
    }

    FILE *arquivo_jogo = fopen(arquivo, "w");
    if (!arquivo_jogo) {
        printf(VERMELHO "❌ Erro ao abrir o arquivo para salvar.\n" RESET);
        return;
    }

    // Primeira linha: tamanho do tabuleiro, desfazer e trocas
    fprintf(arquivo_jogo, "%d %d %d\n", jogo->tamanho_tab, jogo->desfazer_restantes, jogo->trocas_restantes);

    // Segunda linha: pontos e nome do jogador
    fprintf(arquivo_jogo, "%d %s\n", jogo->pontos, jogo->nome);

    int n = jogo->tamanho_tab;

    // Salva matriz atual
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            fprintf(arquivo_jogo, "%d ", jogo->matriz_tab[i][j]);
        fprintf(arquivo_jogo, "\n");
    }

    // Salva matriz anterior (se existir)
    if (jogo->tab_anterior) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                fprintf(arquivo_jogo, "%d ", jogo->tab_anterior[i][j]);
            fprintf(arquivo_jogo, "\n");
        }
    } else {
        // Se não existir, preenche com zeros
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                fprintf(arquivo_jogo, "0 ");
            fprintf(arquivo_jogo, "\n");
        }
    }

    fclose(arquivo_jogo);
    printf(VERDE "💾 Jogo salvo com sucesso em %s!\n" RESET, arquivo);
}

// Função 4: Carrega o jogo de arquivo e exibe o tabuleiro colorido
void carregar_jogo(Jogo *jogo, const char *arquivo) {
    FILE *arquivo_jogo = fopen(arquivo, "r");
    if (!arquivo_jogo) {
        printf(VERMELHO "❌ Erro ao abrir o arquivo para carregar.\n" RESET);
        return;
    }

    // Lê tamanho, desfazer e trocas restantes
    fscanf(arquivo_jogo, "%d %d %d", &jogo->tamanho_tab, &jogo->desfazer_restantes, &jogo->trocas_restantes);

    // Lê pontuação
    fscanf(arquivo_jogo, "%d", &jogo->pontos);

    // Lê nome completo do jogador
    int c;
    while ((c = fgetc(arquivo_jogo)) == ' ');
    ungetc(c, arquivo_jogo);
    fgets(jogo->nome, sizeof(jogo->nome), arquivo_jogo);
    jogo->nome[strcspn(jogo->nome, "\n")] = '\0';

    int n = jogo->tamanho_tab;

    // Aloca matrizes
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

    printf(VERDE "📂 Jogo carregado com sucesso de %s!\n" RESET, arquivo);

    // Mostra o tabuleiro colorido
    imprimir_matriz(jogo);
}

// Função 5: Apenas lê um arquivo e mostra matrizes coloridas sem criar Jogo completo
void ler_jogo(const char *arq) {
    FILE *arquivo_jogo = fopen(arq, "r");
    if (!arquivo_jogo) {
        printf(VERMELHO "❌ Erro ao abrir o arquivo %s.\n" RESET, arq);
        return;
    }

    int tamanho, ndesfazer, ntrocar, pontos;
    char nome[100];

    fscanf(arquivo_jogo, "%d %d %d", &tamanho, &ndesfazer, &ntrocar);
    fscanf(arquivo_jogo, "%d", &pontos);

    int c;
    while ((c = fgetc(arquivo_jogo)) == ' ');
    ungetc(c, arquivo_jogo);

    fgets(nome, sizeof(nome), arquivo_jogo);
    nome[strcspn(nome, "\n")] = '\0';

    printf(CIANA "\n📋 Dados do Jogo:\n" RESET);
    printf(AMARELO "Tamanho: %d | Desfazer: %d | Trocas: %d\n" RESET, tamanho, ndesfazer, ntrocar);
    printf(AMARELO "Pontuação: %d | Jogador: %s\n" RESET, pontos, nome);

    int **matriz = (int **)malloc(tamanho * sizeof(int *));
    for (int i = 0; i < tamanho; i++)
        matriz[i] = (int *)malloc(tamanho * sizeof(int));

    printf(MAGENTA "\nMatriz atual:\n" RESET);
    for (int i = 0; i < tamanho; i++)
        for (int j = 0; j < tamanho; j++)
            fscanf(arquivo_jogo, "%d", &matriz[i][j]);

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            int val = matriz[i][j];
            if (val == 0) printf("   . ");
            else printf("%s%4d " RESET, cor_valor(val), val);
        }
        printf("\n");
    }

    printf(MAGENTA "\nMatriz última jogada:\n" RESET);
    for (int i = 0; i < tamanho; i++)
        for (int j = 0; j < tamanho; j++)
            fscanf(arquivo_jogo, "%d", &matriz[i][j]);

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            int val = matriz[i][j];
            if (val == 0) printf("   . ");
            else printf("%s%4d " RESET, cor_valor(val), val);
        }
        printf("\n");
    }

    for (int i = 0; i < tamanho; i++)
        free(matriz[i]);
    free(matriz);

    fclose(arquivo_jogo);
    printf(VERDE "\n✅ Leitura do jogo concluída!\n" RESET);
}
