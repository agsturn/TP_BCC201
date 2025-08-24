// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct {
    int tamanho_tab;          
    int **matriz_tab;         
    int pontos;               
    char nome[100];            
    int **tab_anterior;
    int pontos_anterior;
    int desfazer_restantes;
    int trocas_restantes;
} Jogo;

typedef struct {
    int pontos[10];
    int numpontos;
} Rankingtamanho;

typedef struct {
    Rankingtamanho tab4;
    Rankingtamanho tab5;
    Rankingtamanho tab6;
} Ranking;

#endif // __STRUCT_H__