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

void centralizar(char *texto);
void telaInicial();
int **criar_matriz(int n);
void imprimir_matriz(Jogo *j);
void liberar_matriz(int **matriz, int n);
void n_aleatorio(Jogo *jogo_atual); 
int **copiar_matriz(int **matriz, int tamanho);
int mover(Jogo *jogo,char direcao);
int mover_direita(Jogo *jogo);
int mover_esquerda(Jogo *jogo);
int mover_cima(Jogo *jogo);
int mover_baixo(Jogo *jogo);
void adicionar_pontos(Jogo *jogo, int pontos);
void movimento_nao_guloso(int *linha, int tamanho, int *pontuacao, int *houve_movimento);
int verificar_jogo(Jogo *jogo);

#endif // __FUNCOES_H__
