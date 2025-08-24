// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __FUNCOES_H__
#define __FUNCOES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "struct.h"
#include "salva_jogo.h"

// ==================== FUNÇÕES BÁSICAS ====================
void centralizar(const char *texto, int largura_total); 
void telaInicial(void);                                 
void limpar_buffer(void);                             

// ==================== MATRIZ ====================
int **criar_matriz(int n);                              
void liberar_matriz(int **matriz, int n);             
int **copiar_matriz(int **matriz, int tamanho);        
const char* cor_valor(int valor);                     
void imprimir_matriz(Jogo *j);                         
void n_aleatorio(Jogo *jogo_atual);

// ==================== MOVIMENTAÇÃO ====================
void movimento_nao_guloso(int *linha, int tamanho, int *pontuacao, int *houve_movimento); 
int mover_esquerda(Jogo *jogo);                         
int mover_direita(Jogo *jogo);                          
int mover_cima(Jogo *jogo);                             
int mover_baixo(Jogo *jogo);                            

// ==================== DESFAZER / TROCAR ====================
void desfazer_movimento(Jogo *jogo);                   
void trocar_pecas(Jogo *jogo);                         
void verificar_ganho_movimentos_especiais(Jogo *jogo);

// ==================== VERIFICAÇÕES ====================
int verificar_jogo(Jogo *jogo);                        
int verificar_vitoria(Jogo *jogo);                     
int verificar_derrota(Jogo *jogo);                     

#endif