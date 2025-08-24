// Ana Gabriela Gomes Lopes Pereira - 25.1.4119
#ifndef __SALVA_JOGO_H__
#define __SALVA_JOGO_H__

#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

void salvar_jogo(Jogo *jogo, const char *arquivo);
void carregar_jogo(Jogo *jogo, const char *arquivo);
void ler_jogo(const char *arq);

#endif // __SALVA_JOGO_H__