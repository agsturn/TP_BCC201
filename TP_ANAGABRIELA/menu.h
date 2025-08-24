// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __MENU_H__
#define __MENU_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "struct.h"      
#include "funcoes.h"     
#include "exemplo.h"     
#include "ranking.h"     
#include "salva_jogo.h"  

// Protótipos do menu
void menu(void);
void sair(Jogo *jogo_atual);
void novo_jogo(Jogo *jogo_atual);
void continuar_jogo(Jogo *jogo_atual);
void mostrar_ajuda(void);

#endif // __MENU_H__