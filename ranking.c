// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>

// Função 1: Função para criar o arquivo de ranking inicial
void criar_ranking(void) {
    FILE *arqRanking = fopen("ranking.dat", "wb");
    if (!arqRanking) {
        printf("Erro ao criar arquivo ranking.dat\n");
        return;
    }

    int num4 = 0;
    int num5 = 0;
    int num6 = 0;

    // Grava os três números no início do arquivo
    // Eles indicam quantas pontuações existem para cada tamanho
    fwrite(&num4, sizeof(int), 1, arqRanking);
    fwrite(&num5, sizeof(int), 1, arqRanking);
    fwrite(&num6, sizeof(int), 1, arqRanking);

    fclose(arqRanking);
    printf("Arquivo ranking.dat criado com sucesso!\n");
}

// Função 2: Mostrar ranking
void mostrar_ranking(void) {
    FILE *arqRanking = fopen("ranking.dat", "rb");// Abre o arquivo binário de ranking
    if (!arqRanking) {
        printf("Arquivo ranking.dat não encontrado!\n");
        return;
    }

    // Lê a quantidade de pontuações cadastradas para cada tamanho de tabuleiro
    int num4, num5, num6;
    fread(&num4, sizeof(int), 1, arqRanking);
    fread(&num5, sizeof(int), 1, arqRanking);
    fread(&num6, sizeof(int), 1, arqRanking);

    char nome_jogador[100];
    int pontos;              

    printf("===== Ranking 4x4 =====\n");
    for(int i = 0; i < num4; i++) {
        fread(nome_jogador, sizeof(char), 100, arqRanking); 
        fread(&pontos, sizeof(int), 1, arqRanking);         
        printf("%2d. %-27s %d\n", i+1, nome_jogador, pontos);
    }

    printf("\n===== Ranking 5x5 =====\n");
    for(int i = 0; i < num5; i++) {
        fread(nome_jogador, sizeof(char), 100, arqRanking);
        fread(&pontos, sizeof(int), 1, arqRanking);
        printf("%2d. %-27s %d\n", i+1, nome_jogador, pontos);
    }

    printf("\n===== Ranking 6x6 =====\n");
    for(int i = 0; i < num6; i++) {
        fread(nome_jogador, sizeof(char), 100, arqRanking);
        fread(&pontos, sizeof(int), 1, arqRanking);
        printf("%2d. %-27s %d\n", i+1, nome_jogador, pontos);
    }

    // Fecha o arquivo
    fclose(arqRanking);
}

// Função 3: Função para atualizar o ranking 
void atualizar_ranking(Jogo *jogo) {
    FILE *arqRanking = fopen("ranking.dat", "rb+");
    if (!arqRanking) {
        printf("Arquivo ranking.dat não encontrado!\n");
        return;
    }

    // Lê a quantidade de pontuações cadastradas para cada tamanho de tabuleiro
    int num4, num5, num6;
    fread(&num4, sizeof(int), 1, arqRanking);
    fread(&num5, sizeof(int), 1, arqRanking);
    fread(&num6, sizeof(int), 1, arqRanking);

    // Buffers para armazenar nomes e pontuações
    char nomes[10][100];
    int pontos[10];
    int *num_jogadores;
    long pos_inicial;

    // Determina qual ranking atualizar com base no tamanho do tabuleiro
    if(jogo->tamanho_tab == 4) {
        pos_inicial = sizeof(int)*3;
        fseek(arqRanking, pos_inicial, SEEK_SET);
        for(int i = 0; i < num4; i++) {
            fread(nomes[i], sizeof(char), 100, arqRanking);
            fread(&pontos[i], sizeof(int), 1, arqRanking);
        }
        num_jogadores = &num4;
    } else if(jogo->tamanho_tab == 5) {
        pos_inicial = sizeof(int)*3 + (sizeof(char)*100 + sizeof(int))*num4;
        fseek(arqRanking, pos_inicial, SEEK_SET);
        for(int i = 0; i < num5; i++) {
            fread(nomes[i], sizeof(char), 100, arqRanking);
            fread(&pontos[i], sizeof(int), 1, arqRanking);
        }
        num_jogadores = &num5;
    } else if(jogo->tamanho_tab == 6) {
        pos_inicial = sizeof(int)*3 + (sizeof(char)*100 + sizeof(int))*(num4 + num5);
        fseek(arqRanking, pos_inicial, SEEK_SET);
        for(int i = 0; i < num6; i++) {
            fread(nomes[i], sizeof(char), 100, arqRanking);
            fread(&pontos[i], sizeof(int), 1, arqRanking);
        }
        num_jogadores = &num6;
    } else {
        fclose(arqRanking);
        return; // Tamanho de tabuleiro inválido
    }

    // Adiciona novo jogador ao ranking
    if(*num_jogadores < 10) { // se ainda houver espaço
        strncpy(nomes[*num_jogadores], jogo->nome, 100);
        nomes[*num_jogadores][99] = '\0'; // garante terminação
        pontos[*num_jogadores] = jogo->pontos;
        (*num_jogadores)++;
    } else if(jogo->pontos > pontos[9]) { // substitui o menor se maior que o último
        strncpy(nomes[9], jogo->nome, 100);
        nomes[9][99] = '\0';
        pontos[9] = jogo->pontos;
    } else {
        fclose(arqRanking);
        return; // pontuação muito baixa para entrar no ranking
    }

    // Ordena do maior para o menor usando Bubble Sort
    for(int i = 0; i < *num_jogadores - 1; i++) {
        for(int j = i + 1; j < *num_jogadores; j++) {
            if(pontos[j] > pontos[i]) {
                // Troca pontuação
                int tmp_p = pontos[i];
                pontos[i] = pontos[j];
                pontos[j] = tmp_p;
                // Troca nome correspondente
                char tmp_n[100];
                strcpy(tmp_n, nomes[i]);
                strcpy(nomes[i], nomes[j]);
                strcpy(nomes[j], tmp_n);
            }
        }
    }

    // Grava de volta no arquivo
    fseek(arqRanking, pos_inicial, SEEK_SET);
    for(int i = 0; i < *num_jogadores; i++) {
        fwrite(nomes[i], sizeof(char), 100, arqRanking);
        fwrite(&pontos[i], sizeof(int), 1, arqRanking);
    }

    // Atualiza os contadores iniciais
    fseek(arqRanking, 0, SEEK_SET);
    fwrite(&num4, sizeof(int), 1, arqRanking);
    fwrite(&num5, sizeof(int), 1, arqRanking);
    fwrite(&num6, sizeof(int), 1, arqRanking);

    fclose(arqRanking);
    printf("Ranking atualizado com sucesso!\n");
}

