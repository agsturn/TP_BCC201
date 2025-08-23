// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __FUNCOES_H__
#define __FUNCOES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "struct.h"
#include "salva_jogo.h" // Necessário para salvar/carregar

// ==================== FUNÇÕES BÁSICAS ====================
// Centraliza um texto no console
void centralizar(char *texto);

// Mostra a tela inicial do jogo
void telaInicial(void);

// Limpa o buffer do teclado
void limpar_buffer(void);

// ==================== MATRIZ ====================
// Cria uma matriz n x n dinamicamente
int **criar_matriz(int n);

// Copia uma matriz existente para uma nova matriz alocada
int **copiar_matriz(int **matriz, int tamanho);

// Imprime a matriz do jogo formatada
void imprimir_matriz(Jogo *j);

// Libera a memória alocada para uma matriz
void liberar_matriz(int **matriz, int n);

// Adiciona um número aleatório (2 ou 4) em posição vazia do tabuleiro
void n_aleatorio(Jogo *jogo_atual);

// ==================== MOVIMENTAÇÃO E PONTUAÇÃO ====================
// Move as peças do jogo na direção indicada ('w','a','s','d')
// Retorna 1 se houve movimento, 0 se não
int mover(Jogo *jogo, char direcao);

// Movimenta todas as peças para a direita
int mover_direita(Jogo *jogo);

// Movimenta todas as peças para a esquerda
int mover_esquerda(Jogo *jogo);

// Movimenta todas as peças para cima
int mover_cima(Jogo *jogo);

// Movimenta todas as peças para baixo
int mover_baixo(Jogo *jogo);

// Função auxiliar que realiza o movimento e soma pontos de uma linha
void movimento_nao_guloso(int *linha, int tamanho, int *pontuacao, int *houve_movimento);

// Desfaz o último movimento realizado, se disponível
void desfazer_ultimo_movimento(Jogo *jogo);

// Troca duas peças de posição no tabuleiro, se houver trocas restantes
void trocar_pecas(Jogo *jogo);

// Adiciona pontos à pontuação atual do jogador
void adicionar_pontos(Jogo *jogo, int pontos);

// ==================== VERIFICAÇÕES ====================
// Verifica se há movimentos possíveis no tabuleiro
int verificar_movimento(Jogo *jogo);

// Verifica se o jogador venceu (chegou a 2048)
int verificar_vitoria(Jogo *jogo);

// Verifica se não há mais movimentos possíveis (derrota)
int verificar_derrota(Jogo *jogo);

#endif // __FUNCOES_H__
