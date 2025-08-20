//Ana Gabriela - 25.1.4119

#ifndef __STRUCT_H__
#define __STRUCT_H__

// Struct para guardar as informações do jogo
typedef struct {
    int tamanho_tab;     // tamanho do tabuleiro
    int **matriz_tab;    // ponteiro de matriz para malloc
    int pontos;          // pontos atuais
    char nome[50];       // nome do jogador
    int desfaz;          // chances de desfazer
    int movimentos_desfazer; // quantidade de movimentos que ainda podem ser desfeitos
    int **tab_anterior;  // tabuleiro anterior (para desfazer)
    int pontos_anterior; // pontos anteriores (para desfazer)
} Jogo;


#endif // __STRUCT_H__