// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __FUNCOES_H__
#define __FUNCOES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "struct.h"
#include "exemplo.h"
#include "menu.h"

// Funções de interface
void centralizar(const char* texto);
void telaInicial(void);
void imprimir_matriz(Jogo *jogo);
void jogar(Jogo *jogo);

// Funções de manipulação do tabuleiro
int** criar_matriz(int n);
void liberar_matriz(int **matriz, int n);
int** copiar_matriz(int **matriz, int tamanho);
void abrir_pecas(Jogo *jogo);
void nova_peca(Jogo *jogo);
int movimentar(Jogo *jogo, char direcao);
void adicionar_pontos(Jogo *jogo, int valor);
int movimento_nao_guloso(Jogo *jogo, char direcao);


#endif // __FUNCOES_H__
