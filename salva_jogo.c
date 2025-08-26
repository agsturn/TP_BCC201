// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "exemplo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "salva_jogo.h"

// Função 1: Salva o jogo em arquivo de texto
void salvar_jogo(Jogo *jogo, const char *arquivo) {
    if (!jogo) {
        printf(VERMELHO "❌ Erro: ponteiro do jogo inválido.\n" RESET);
        return;
    }

    FILE *arquivo_jogo = fopen(arquivo, "w");
    if (!arquivo_jogo) {
        printf(VERMELHO "❌ Erro ao abrir o arquivo para salvar.\n" RESET);
        return;
    }

    fprintf(arquivo_jogo, "%d %d %d\n", jogo->tamanho_tabuleiro, 
            jogo->desfazer_disponiveis, jogo->trocas_disponiveis);
    fprintf(arquivo_jogo, "%d %s\n", jogo->pontuacao, jogo->nome_jogador);

    for (int i = 0; i < jogo->tamanho_tabuleiro; i++) {
        for (int j = 0; j < jogo->tamanho_tabuleiro; j++)
            fprintf(arquivo_jogo, "%d ", jogo->matriz_atual[i][j]);
        fprintf(arquivo_jogo, "\n");
    }

    if (jogo->matriz_anterior) {
        for (int i = 0; i < jogo->tamanho_tabuleiro; i++) {
            for (int j = 0; j < jogo->tamanho_tabuleiro; j++)
                fprintf(arquivo_jogo, "%d ", jogo->matriz_anterior[i][j]);
            fprintf(arquivo_jogo, "\n");
        }
    } else {
        for (int i = 0; i < jogo->tamanho_tabuleiro; i++) {
            for (int j = 0; j < jogo->tamanho_tabuleiro; j++)
                fprintf(arquivo_jogo, "0 ");
            fprintf(arquivo_jogo, "\n");
        }
    }

    fclose(arquivo_jogo);
    printf(VERDE "💾 Jogo salvo com sucesso em %s!\n" RESET, arquivo);
}

// Função 2: Carrega o jogo de arquivo
void carregar_jogo(Jogo *jogo, const char *arquivo) {
    FILE *arquivo_jogo = fopen(arquivo, "r");
    if (!arquivo_jogo) {
        printf(VERMELHO "❌ Erro ao abrir o arquivo para carregar.\n" RESET);
        return;
    }

    // Ler dados básicos com verificação
    if (fscanf(arquivo_jogo, "%d %d %d", &jogo->tamanho_tabuleiro, 
               &jogo->desfazer_disponiveis, &jogo->trocas_disponiveis) != 3) {
        printf("❌ Erro na leitura dos dados básicos\n");
        fclose(arquivo_jogo);
        return;
    }

    // Validar tamanho
    if (jogo->tamanho_tabuleiro < 4 || jogo->tamanho_tabuleiro > 6) {
        printf("❌ Tamanho inválido no arquivo: %d\n", jogo->tamanho_tabuleiro);
        fclose(arquivo_jogo);
        return;
    }

    // Ler pontuação
    if (fscanf(arquivo_jogo, "%d", &jogo->pontuacao) != 1) {
        printf("❌ Erro ao ler pontuação\n");
        fclose(arquivo_jogo);
        return;
    }

    // Ler nome
    if (fscanf(arquivo_jogo, " %27[^\n]", jogo->nome_jogador) != 1) {
        printf("❌ Erro ao ler nome do jogador\n");
        fclose(arquivo_jogo);
        return;
    }

    int n = jogo->tamanho_tabuleiro;
    
    // Alocar e ler matriz atual
    jogo->matriz_atual = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        jogo->matriz_atual[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            if (fscanf(arquivo_jogo, "%d", &jogo->matriz_atual[i][j]) != 1) {
                printf("❌ Erro ao ler matriz atual[%d][%d]\n", i, j);
                fclose(arquivo_jogo);
                return;
            }
        }
    }

    // Alocar e ler matriz anterior
    jogo->matriz_anterior = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        jogo->matriz_anterior[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            if (fscanf(arquivo_jogo, "%d", &jogo->matriz_anterior[i][j]) != 1) {
                printf("❌ Erro ao ler matriz anterior[%d][%d]\n", i, j);
                fclose(arquivo_jogo);
                return;
            }
        }
    }

    fclose(arquivo_jogo);
    printf(VERDE "📂 Jogo carregado com sucesso de %s!\n" RESET, arquivo);
}
// Função 3: Lê um jogo diretamente do arquivo e mostra
void ler_jogo(const char *arq) {
    FILE *arquivo_jogo = fopen(arq, "r");
    if (!arquivo_jogo) {
        printf(VERMELHO "❌ Erro ao abrir o arquivo %s.\n" RESET, arq);
        return;
    }

    int tamanho, desfazer, trocar, pontos;
    char nome[28]; 

    fscanf(arquivo_jogo, "%d %d %d", &tamanho, &desfazer, &trocar);
    fscanf(arquivo_jogo, "%d", &pontos);
    fscanf(arquivo_jogo, " %27c", nome);

    printf(AZUL NEGRITO "📊 Informações do Jogo:\n" RESET);
    printf(AMARELO "Tamanho: %d | Desfazer: %d | Trocar: %d\n" RESET, tamanho, desfazer, trocar);
    printf(VERDE "Pontuação: %d | Jogador: %s\n" RESET, pontos, nome);

    int valor;
    printf(CIANA "\nMatriz atual:\n" RESET);
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fscanf(arquivo_jogo, "%d", &valor);
            printf("%4d", valor);
        }
        printf("\n");
    }

    printf(MAGENTA "\nMatriz última jogada:\n" RESET);
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fscanf(arquivo_jogo, "%d", &valor);
            printf("%4d", valor);
        }
        printf("\n");
    }

    fclose(arquivo_jogo);
}
