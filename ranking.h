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

// ==================== FUNÇÕES DE RANKING ====================
void criar_ranking();// Cria o ranking inicial 
void mostrar_ranking();// Mostra o ranking completo na tela
void atualizar_ranking(Jogo *jogo);// Atualiza o ranking 

#endif // __RANKING_H__
