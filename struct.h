//Ana Gabriela - 25.1.4119

#ifndef __STRUCT_H__
#define __STRUCT_H__

// Struct para guardar as informações do jogo
typedef struct {
    int tamanho_tab;     // tamanho do tabuleiro
    int **matriz_tab;    // ponteiro de matriz para malloc
    int pontos;          // pontos atuais
    char nome[50];       // nome do jogador
    int desfaz;          // chances de desfazer (não usado ainda)
    int troca;           // chances de trocar (não usado ainda)
    int **tab_anterior;  // tabuleiro anterior (não usado ainda) 
    int pontos_anterior; // pontos anteriores (não usado ainda)
    int temsalvo;        // flag para desfazer (não usado ainda)
} Jogo;

#endif // __STRUCT_H__