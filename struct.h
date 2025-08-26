// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct {
    int tamanho_tabuleiro;          
    int **matriz_atual;         
    int pontuacao;               
    char nome_jogador[28]; // 27 caracteres + '\0'            
    int **matriz_anterior;
    int pontuacao_anterior;
    int desfazer_disponiveis;
    int trocas_disponiveis;
} Jogo;

typedef struct {
    char nome[28]; // 27 caracteres + '\0'
    int pontuacao;
} EntradaRanking;

typedef struct {
    EntradaRanking ranking_4[10];
    EntradaRanking ranking_5[10];
    EntradaRanking ranking_6[10];
    int quantidade_4;
    int quantidade_5;
    int quantidade_6;
} RankingGeral;

#endif // __STRUCT_H__