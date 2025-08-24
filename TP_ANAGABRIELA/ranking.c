// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo cores para o terminal
#define RESET   "\033[0m"
#define VERMELHO "\033[31m"
#define VERDE   "\033[32m"
#define AMARELO "\033[33m"
#define AZUL    "\033[34m"
#define MAGENTA "\033[35m"
#define CIANO   "\033[36m"
#define BRANCO  "\033[37m"
#define CIANA   "\033[36;1m"
#define NEGRITO "\033[1m"

//  Função A: Cria um arquivo binário de ranking inicial 
void criar_ranking(void) {
    // Abre o arquivo para escrita binária
    FILE *arquivo = fopen("ranking.dat", "wb");
    if (!arquivo) {
        printf(VERMELHO "❌ Erro ao criar arquivo ranking.dat\n" RESET);
        return;
    }

    // Inicializa os contadores de cada tabuleiro com zero
    int contadores[3] = {0, 0, 0}; 
    
    // Escreve os contadores no arquivo
    fwrite(contadores, sizeof(int), 3, arquivo);

    // Fecha o arquivo
    fclose(arquivo);
    printf(VERDE "✅ Arquivo ranking.dat criado com sucesso!\n" RESET);
}

// Função B: Carrega os dados do arquivo binário 
Ranking carregar_ranking(void) {
    Ranking ranking;
    
    // Inicializa todos os contadores com zero
    ranking.tab4.numpontos = 0;
    ranking.tab5.numpontos = 0;
    ranking.tab6.numpontos = 0;

    // Abre o arquivo para leitura binária
    FILE *arquivo = fopen("ranking.dat", "rb");
    if (!arquivo) {
        return ranking; // Retorna ranking vazio se arquivo não existir
    }

    // Lê os contadores de cada tabuleiro
    int contadores[3];
    fread(contadores, sizeof(int), 3, arquivo);

    // Carrega pontos do tabuleiro 4x4
    ranking.tab4.numpontos = contadores[0];
    if (contadores[0] > 0) {
        fread(ranking.tab4.pontos, sizeof(int), contadores[0], arquivo);
    }

    // Carrega pontos do tabuleiro 5x5
    ranking.tab5.numpontos = contadores[1];
    if (contadores[1] > 0) {
        fread(ranking.tab5.pontos, sizeof(int), contadores[1], arquivo);
    }

    // Carrega pontos do tabuleiro 6x6
    ranking.tab6.numpontos = contadores[2];
    if (contadores[2] > 0) {
        fread(ranking.tab6.pontos, sizeof(int), contadores[2], arquivo);
    }

    // Fecha o arquivo
    fclose(arquivo);
    return ranking;
}

// Função C: Exibe o ranking completo no terminal
void mostrar_ranking(void) {
    // Limpa a tela do terminal
    system("clear");
    
    // Cabeçalho do ranking
    printf(CIANA "╔══════════════════════════════════════════════╗\n" RESET);
    printf(CIANA "║" AMARELO NEGRITO "              🏆 RANKING 2048 🏆             " CIANA "║\n" RESET);
    printf(CIANA "╠══════════════════════════════════════════════╣\n" RESET);

    // Carrega o ranking na estrutura
    Ranking ranking = carregar_ranking();

    // Nomes dos tabuleiros para exibição
    char *nomes_tab[] = {"4x4", "5x5", "6x6"};
    
    // Array com os rankings de cada tabuleiro para facilitar acesso
    Rankingtamanho *tabs[] = {&ranking.tab4, &ranking.tab5, &ranking.tab6};
    
    // Exibe o ranking de cada tabuleiro
    for (int i = 0; i < 3; i++) {
        int quantidade = tabs[i]->numpontos;
        
        // Cabeçalho do tabuleiro
        printf(CIANA "║" RESET "   🎮 Tabuleiro %s - %d registros\n", nomes_tab[i], quantidade);

        // Ordena os pontos (do maior para o menor)
        for (int j = 0; j < quantidade-1; j++) {
            for (int k = j+1; k < quantidade; k++) {
                if (tabs[i]->pontos[k] > tabs[i]->pontos[j]) {
                    int temp = tabs[i]->pontos[j];
                    tabs[i]->pontos[j] = tabs[i]->pontos[k];
                    tabs[i]->pontos[k] = temp;
                }
            }
        }

        // Mostra os pontos ordenados
        for (int j = 0; j < quantidade; j++) {
            printf("     %2d) 👤 Jogador %d - " AMARELO "%6d pontos" RESET "\n", 
                   j+1, j+1, tabs[i]->pontos[j]);
        }
        
        // Mensagem se não houver registros
        if (quantidade == 0) {
            printf("     📭 (Nenhum registro)\n");
        }
        printf("\n");
    }

    // Rodapé do ranking
    printf(CIANA "╚══════════════════════════════════════════════╝\n" RESET);
    printf("\n" AMARELO "⏎ Pressione ENTER para voltar...\n" RESET);
    getchar();
    getchar(); // Segundo getchar para capturar o ENTER
}

// Função D: Atualiza o ranking com a pontuação do jogo atual
void atualizar_ranking(Jogo *jogo) {
    // Carrega o ranking atual
    Ranking ranking = carregar_ranking();
    
    // Determina qual tabuleiro usar baseado no tamanho
    Rankingtamanho *tab_atual;
    
    if (jogo->tamanho_tab == 4) {
        tab_atual = &ranking.tab4;
    }
    else if (jogo->tamanho_tab == 5) {
        tab_atual = &ranking.tab5;
    }
    else if (jogo->tamanho_tab == 6) {
        tab_atual = &ranking.tab6;
    }
    else {
        return; // Tamanho inválido
    }

    // Adiciona a nova pontuação se houver espaço ou se for maior que a menor
    if (tab_atual->numpontos < 10) {
        tab_atual->pontos[tab_atual->numpontos] = jogo->pontos;
        tab_atual->numpontos++;
    } else if (jogo->pontos > tab_atual->pontos[9]) {
        tab_atual->pontos[9] = jogo->pontos;
    } else {
        return; // Não atualiza se não for melhor que os 10 melhores
    }

    // Ordena os pontos do tabuleiro atual (do maior para o menor)
    for (int i = 0; i < tab_atual->numpontos-1; i++) {
        for (int j = i+1; j < tab_atual->numpontos; j++) {
            if (tab_atual->pontos[j] > tab_atual->pontos[i]) {
                int temp = tab_atual->pontos[i];
                tab_atual->pontos[i] = tab_atual->pontos[j];
                tab_atual->pontos[j] = temp;
            }
        }
    }

    // Abre o arquivo para escrita binária
    FILE *arquivo = fopen("ranking.dat", "wb");
    if (!arquivo) {
        printf(VERMELHO "❌ Erro ao abrir arquivo para atualização\n" RESET);
        return;
    }

    // Escreve os contadores atualizados
    int contadores[3] = {
        ranking.tab4.numpontos,
        ranking.tab5.numpontos,
        ranking.tab6.numpontos
    };
    fwrite(contadores, sizeof(int), 3, arquivo);

    // Escreve todos os pontos de todos os tabuleiros
    fwrite(ranking.tab4.pontos, sizeof(int), ranking.tab4.numpontos, arquivo);
    fwrite(ranking.tab5.pontos, sizeof(int), ranking.tab5.numpontos, arquivo);
    fwrite(ranking.tab6.pontos, sizeof(int), ranking.tab6.numpontos, arquivo);

    // Fecha o arquivo
    fclose(arquivo);
    printf(VERDE "📊 Ranking atualizado! " AMARELO "Sua pontuação: %d\n" RESET, jogo->pontos);
}