// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>

// Função 1: Criar o arquivo de ranking inicial
void criar_ranking(void) {
    FILE *arquivoRanking = fopen("ranking.dat", "wb");
    if (!arquivoRanking) {
        printf("Erro ao criar arquivo ranking.dat\n");
        return;
    }

    // Inicializa os contadores para cada tamanho de tabuleiro
    int totalTabuleiro[3] = {0, 0, 0}; 
    fwrite(totalTabuleiro, sizeof(int), 3, arquivoRanking);

    fclose(arquivoRanking);
    printf("Arquivo ranking.dat criado com sucesso!\n");
}

// Função 2: Mostra ranking completo
void mostrar_ranking(void) {
    FILE *arquivoRanking = fopen("ranking.dat", "rb");
    if (!arquivoRanking) {
        printf("Arquivo ranking.dat não encontrado!\n");
        return;
    }

    int totalTabuleiro[3];
    fread(totalTabuleiro, sizeof(int), 3, arquivoRanking);

    char nome[100];
    int pontos;

    // Mostra ranking para cada tabuleiro
    for (int tab = 0; tab < 3; tab++) {
        printf("===== Ranking Tabuleiro %d =====\n", tab + 1);
        for (int i = 0; i < totalTabuleiro[tab]; i++) {
            fread(nome, sizeof(char), 100, arquivoRanking);
            fread(&pontos, sizeof(int), 1, arquivoRanking);
            printf("%2d. %-27s %d\n", i+1, nome, pontos);
        }
        printf("\n");
    }

    fclose(arquivoRanking);
}

// Função 3: Atualiza ranking considerando desempate por movimentos especiais
void atualizar_ranking(Jogo *jogo) {
    FILE *arquivoRanking = fopen("ranking.dat", "rb+");
    if (!arquivoRanking) {
        printf("Arquivo ranking.dat não encontrado!\n");
        return;
    }

    // Lê os contadores de cada tabuleiro
    int totalTabuleiro[3];
    fread(totalTabuleiro, sizeof(int), 3, arquivoRanking);

    // Determina índice do tabuleiro baseado no tamanho
    int indiceTab = jogo->tamanho_tab - 4; // Ex: 4x4=0, 5x5=1, 6x6=2
    if (indiceTab < 0 || indiceTab > 2) {
        fclose(arquivoRanking);
        return;
    }

    // Calcula posição inicial do ranking no arquivo
    long posicaoInicial = sizeof(int) * 3; 
    for (int i = 0; i < indiceTab; i++) {
        posicaoInicial += totalTabuleiro[i] * (sizeof(char)*100 + sizeof(int));
    }

    fseek(arquivoRanking, posicaoInicial, SEEK_SET);

    // Lê ranking existente
    char nomes[10][100];
    int pontos[10];
    int movimentosEspeciais[10]; // Para desempate
    int totalAtual = totalTabuleiro[indiceTab];

    for (int i = 0; i < totalAtual; i++) {
        fread(nomes[i], sizeof(char), 100, arquivoRanking);
        fread(&pontos[i], sizeof(int), 1, arquivoRanking);
        movimentosEspeciais[i] = 0; // Inicialmente 0
    }

    // Desempate: menos movimentos especiais usados é melhor
    int desempateJogador = jogo->trocas_restantes + jogo->desfazer_restantes;

    // Adiciona novo jogador ou substitui pior considerando desempate
    if (totalAtual < 10) {
        strcpy(nomes[totalAtual], jogo->nome);
        nomes[totalAtual][99] = '\0';  // 
        pontos[totalAtual] = jogo->pontos;
        movimentosEspeciais[totalAtual] = desempateJogador;
        totalAtual++;
    } else if (jogo->pontos > pontos[9] || 
              (jogo->pontos == pontos[9] && desempateJogador < movimentosEspeciais[9])) {
        strcpy(nomes[9], jogo->nome);
        nomes[9][99] = '\0';
        pontos[9] = jogo->pontos;
        movimentosEspeciais[9] = desempateJogador;
    } else {
        fclose(arquivoRanking);
        return;
    }

    // Ordena ranking: maior pontuação primeiro, desempate por menos movimentos usados
    for (int i = 0; i < totalAtual-1; i++) {
        for (int j = i+1; j < totalAtual; j++) {
            if (pontos[j] > pontos[i] ||
                (pontos[j] == pontos[i] && movimentosEspeciais[j] < movimentosEspeciais[i])) {
                
                // Troca pontuação
                int pontosTemporario = pontos[i];
                pontos[i] = pontos[j];
                pontos[j] = pontosTemporario;

                // Troca nome
                char nomeTemporario[100];
                strcpy(nomeTemporario, nomes[i]);
                strcpy(nomes[i], nomes[j]);
                strcpy(nomes[j], nomeTemporario);

                // Troca valor de desempate
                int movimentosTemporario = movimentosEspeciais[i];
                movimentosEspeciais[i] = movimentosEspeciais[j];
                movimentosEspeciais[j] = movimentosTemporario;
            }
        }
    }

    // Grava ranking atualizado
    fseek(arquivoRanking, posicaoInicial, SEEK_SET);
    for (int i = 0; i < totalAtual; i++) {
        fwrite(nomes[i], sizeof(char), 100, arquivoRanking);
        fwrite(&pontos[i], sizeof(int), 1, arquivoRanking);
    }

    // Atualiza contadores no início do arquivo
    totalTabuleiro[indiceTab] = totalAtual;
    fseek(arquivoRanking, 0, SEEK_SET);
    fwrite(totalTabuleiro, sizeof(int), 3, arquivoRanking);

    fclose(arquivoRanking);
    printf("Ranking atualizado considerando desempate por movimentos especiais!\n");
}