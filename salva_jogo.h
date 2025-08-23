// Ana Gabriela Gomes Lopes Pereira - 25.1.4119
#ifndef __SALVA_JOGO_H__
#define __SALVA_JOGO_H__

#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

// ==================== PROTÓTIPOS ====================
void salvar_jogo(Jogo *jogo, const char *arquivo);// Salva o estado atual do jogo em um arquivo texto
void carregar_jogo(Jogo *jogo, const char *arquivo);// Carrega um jogo salvo de um arquivo texto
void ler_jogo(const char *arq);// Mostra o conteúdo de um arquivo de jogo sem alterar o estado atual

#endif // __SALVA_JOGO_H__
