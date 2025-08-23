//Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __STRUCT_H__
#define __STRUCT_H__

// Estrutura principal do jogo
typedef struct {
    int tamanho_tab;          
    int **matriz_tab;         
    int pontos;               
    char nome[100];            
    // usar para desfazer
    int **tab_anterior;// tabuleiro anterior
    int pontos_anterior;// pontos anteriores 
    int desfazer_restantes;// quantidade de desfazer 
    // usar para trocar
    int trocas_restantes;// quantidade de trocas 
} Jogo;

// Estrutura auxiliar para ranking de um tabuleiro específico
typedef struct {
    int pontos[10];           // top 10 maiores pontuações
    int numpontos;               // quantidade de pontuações cadastradas
} Rankingtamanho;

// Ranking completo (para diferentes tamanhos de tabuleiro)
typedef struct {
    Rankingtamanho tab4;
    Rankingtamanho tab5;
    Rankingtamanho tab6;
} Ranking;

#endif // __STRUCT_H__
