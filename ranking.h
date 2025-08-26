// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#ifndef __RANKING_H__
#define __RANKING_H__

#include "exemplo.h"
#include "struct.h"

void criar_ranking(const char *nome_arquivo);
void mostrar_ranking(void);
void atualizar_ranking(Jogo *jogo);
RankingGeral carregar_ranking(const char *nome_arquivo);

#endif // __RANKING_H__