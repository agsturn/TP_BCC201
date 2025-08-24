// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __RANKING_H__
#define __RANKING_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "struct.h"
#include "funcoes.h"
#include "exemplo.h"
#include "menu.h"
#include "salva_jogo.h"

void criar_ranking(void); // Cria o arquivo binário de ranking inicial
void mostrar_ranking(void);//Exibe o ranking completo no terminal
void atualizar_ranking(Jogo *jogo); // Atualiza o ranking com os dados do jogo atual
Ranking carregar_ranking(void); //Carrega os dados do ranking na estrutura Ranking

#endif // __RANKING_H__