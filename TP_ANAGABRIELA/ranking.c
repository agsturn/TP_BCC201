// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>

void criar_ranking(void) {
    FILE *arquivoRanking = fopen("ranking.dat", "wb");
    if (!arquivoRanking) {
        printf("Erro ao criar arquivo ranking.dat\n");
        return;
    }

    int totalTabuleiro[3] = {0, 0, 0}; 
    fwrite(totalTabuleiro, sizeof(int), 3, arquivoRanking);

    fclose(arquivoRanking);
    printf("Arquivo ranking.dat criado com sucesso!\n");
}

void mostrar_ranking() {
    system("clear");
    
    printf(CIANA "╔══════════════════════════════════════════════╗\n" RESET);
    printf(CIANA "║" AMARELO "              🏆 RANKING 2048 🏆             " CIANA "║\n" RESET);
    printf(CIANA "╠══════════════════════════════════════════════╣\n" RESET);
    
    printf(CIANA "║" VERDE "         Em desenvolvimento...           " CIANA "║\n" RESET);
    printf(CIANA "║" AZUL "    Sistema de ranking em breve!       " CIANA "║\n" RESET);
    
    printf(CIANA "╚══════════════════════════════════════════════╝\n" RESET);
    
    printf("\n");
    printf(AMARELO "        Pressione ENTER para voltar...\n" RESET);
    getchar();
}

void atualizar_ranking(Jogo *jogo) {
    FILE *arquivoRanking = fopen("ranking.dat", "rb+");
    if (!arquivoRanking) {
        printf("Arquivo ranking.dat não encontrado!\n");
        return;
    }

    int totalTabuleiro[3];
    fread(totalTabuleiro, sizeof(int), 3, arquivoRanking);

    int indiceTab = jogo->tamanho_tab - 4;
    if (indiceTab < 0 || indiceTab > 2) {
        fclose(arquivoRanking);
        return;
    }

    long posicaoInicial = sizeof(int) * 3; 
    for (int i = 0; i < indiceTab; i++) {
        posicaoInicial += totalTabuleiro[i] * (sizeof(char)*100 + sizeof(int));
    }

    fseek(arquivoRanking, posicaoInicial, SEEK_SET);

    char nomes[10][100];
    int pontos[10];
    int movimentosEspeciais[10];
    int totalAtual = totalTabuleiro[indiceTab];

    for (int i = 0; i < totalAtual; i++) {
        fread(nomes[i], sizeof(char), 100, arquivoRanking);
        fread(&pontos[i], sizeof(int), 1, arquivoRanking);
        movimentosEspeciais[i] = 0;
    }

    int desempateJogador = jogo->trocas_restantes + jogo->desfazer_restantes;

    if (totalAtual < 10) {
        strcpy(nomes[totalAtual], jogo->nome);
        pontos[totalAtual] = jogo->pontos;
        movimentosEspeciais[totalAtual] = desempateJogador;
        totalAtual++;
    } else if (jogo->pontos > pontos[9] || 
              (jogo->pontos == pontos[9] && desempateJogador < movimentosEspeciais[9])) {
        strcpy(nomes[9], jogo->nome);
        pontos[9] = jogo->pontos;
        movimentosEspeciais[9] = desempateJogador;
    } else {
        fclose(arquivoRanking);
        return;
    }

    for (int i = 0; i < totalAtual-1; i++) {
        for (int j = i+1; j < totalAtual; j++) {
            if (pontos[j] > pontos[i] ||
                (pontos[j] == pontos[i] && movimentosEspeciais[j] < movimentosEspeciais[i])) {
                
                int pontosTemporario = pontos[i];
                pontos[i] = pontos[j];
                pontos[j] = pontosTemporario;

                char nomeTemporario[100];
                strcpy(nomeTemporario, nomes[i]);
                strcpy(nomes[i], nomes[j]);
                strcpy(nomes[j], nomeTemporario);

                int movimentosTemporario = movimentosEspeciais[i];
                movimentosEspeciais[i] = movimentosEspeciais[j];
                movimentosEspeciais[j] = movimentosTemporario;
            }
        }
    }

    fseek(arquivoRanking, posicaoInicial, SEEK_SET);
    for (int i = 0; i < totalAtual; i++) {
        fwrite(nomes[i], sizeof(char), 100, arquivoRanking);
        fwrite(&pontos[i], sizeof(int), 1, arquivoRanking);
    }

    totalTabuleiro[indiceTab] = totalAtual;
    fseek(arquivoRanking, 0, SEEK_SET);
    fwrite(totalTabuleiro, sizeof(int), 3, arquivoRanking);

    fclose(arquivoRanking);
    printf("Ranking atualizado!\n");
}