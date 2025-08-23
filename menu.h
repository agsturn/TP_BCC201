// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __MENU_H__
#define __MENU_H__

// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Bibliotecas do projeto
#include "struct.h"      
#include "funcoes.h"     // Funções gerais do jogo (movimentos, tabuleiro, etc.)
#include "exemplo.h"     // Decorações 
#include "ranking.h"     // Funções de ranking
#include "salva_jogo.h"  // Funções para salvar/continuar jogo


// Protótipos do menu
void menu();// Exibe o menu principal e gerencia opções

// Protótipos de controle de jogo
void sair(Jogo *jogo_atual); // Encerra o jogo e libera recursos
void novo_jogo(Jogo *jogo_atual); // Inicializa um novo jogo
void continuar_jogo(Jogo *jogo_atual); // Continua jogo salvo

// Protótipos auxiliares
void mostrar_ajuda(void); // Mostra instruções

#endif // __MENU_H__
