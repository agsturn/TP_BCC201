// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __FUNCOES_H__
#define __FUNCOES_H__

#include "exemplo.h"
#include "struct.h"

// ==================== FUNÇÕES BÁSICAS ====================
void centralizar(const char *texto, int largura_total); 
void tela_inicial(void);                                 
void limpar_buffer(void);                              
int processar_posicao(const char *entrada, int *linha, int *coluna);

// ==================== MATRIZ ====================
int **criar_matriz(int tamanho);                              
void liberar_matriz(int **matriz, int tamanho);             
int **copiar_matriz(int **matriz, int tamanho);        
const char* obter_cor_valor(int valor);                     
void imprimir_tabuleiro(Jogo *jogo);                         
void adicionar_numero_aleatorio(Jogo *jogo);

// ==================== MOVIMENTAÇÃO ====================
void movimento_nao_guloso(int *linha, int tamanho, int *pontuacao, int *houve_movimento); 
int mover_esquerda(Jogo *jogo);                         
int mover_direita(Jogo *jogo);                          
int mover_cima(Jogo *jogo);                             
int mover_baixo(Jogo *jogo);                            

// ==================== DESFAZER / TROCAR ====================
void desfazer_movimento(Jogo *jogo);                   
void trocar_pecas(Jogo *jogo);                        

// ==================== VERIFICAÇÕES ====================
int verificar_jogo(Jogo *jogo);                        
int verificar_vitoria(Jogo *jogo);                     
int verificar_derrota(Jogo *jogo);                     
int ha_movimentos_possiveis(Jogo *jogo);

#endif