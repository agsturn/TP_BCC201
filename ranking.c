// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "exemplo.h"        
#include "ranking.h"       
#include <stdio.h>          
#include <stdlib.h>        
#include <string.h>         

// Função 1: Cria um arquivo de ranking vazio 
void criar_ranking(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "wb");  // Abre para escrita binária
    if (!arquivo) {  // Verifica se abriu corretamente
        printf(VERMELHO "❌ Erro ao criar arquivo %s\n" RESET, nome_arquivo);
        return;  // Sai da função em caso de erro
    }

    // Escreve zeros para as quantidades 
    int zero = 0;  // Valor zero para inicializar
    size_t written = 0;  // Contador de bytes escritos
    written += fwrite(&zero, sizeof(int), 1, arquivo);  // quantidade_4
    written += fwrite(&zero, sizeof(int), 1, arquivo);  // quantidade_5  
    written += fwrite(&zero, sizeof(int), 1, arquivo);  // quantidade_6

    fclose(arquivo);  
    
    // Verifica se todos os 3 inteiros foram escritos
    if (written == 3) {  
        printf(VERDE "✅ Arquivo %s criado com sucesso!\n" RESET, nome_arquivo);
    }
}

// Função 2: Carrega os dados do ranking 
RankingGeral carregar_ranking(const char *nome_arquivo) {
    RankingGeral ranking = {0};  // Inicializa com zeros
    
    FILE *arquivo = fopen(nome_arquivo, "rb");  // Abre para leitura binária
    if (!arquivo) {  // Verifica se abriu corretamente
        printf("Arquivo de ranking não encontrado, criando novo...\n");
        criar_ranking(nome_arquivo);  // Cria novo arquivo
        return ranking;  // Retorna ranking vazio
    }

    // 1. Ler as quantidades 
    size_t read = 0;  // Contador de leituras bem-sucedidas
    read += fread(&ranking.quantidade_4, sizeof(int), 1, arquivo);  // Lê quantidade 4x4
    read += fread(&ranking.quantidade_5, sizeof(int), 1, arquivo);  // Lê quantidade 5x5
    read += fread(&ranking.quantidade_6, sizeof(int), 1, arquivo);  // Lê quantidade 6x6

    // Verifica se leu todas as 3 quantidades
    if (read != 3) {  
        printf("❌ Erro ao ler quantidades do ranking\n");
        fclose(arquivo);  // Fecha arquivo
        return ranking;  // Retorna ranking vazio
    }

    // 2. Ler ranking 4x4 
    for (int i = 0; i < ranking.quantidade_4 && i < 10; i++) {  // Para cada entrada 4x4
        if (fread(ranking.ranking_4[i].nome, 27, 1, arquivo) != 1) break;  // Lê nome 
        ranking.ranking_4[i].nome[27] = '\0';  // Termina string 
        if (fread(&ranking.ranking_4[i].pontuacao, sizeof(int), 1, arquivo) != 1) break;  // Lê pontuação
    }

    // 3. Ler ranking 5x5
    for (int i = 0; i < ranking.quantidade_5 && i < 10; i++) {  
        if (fread(ranking.ranking_5[i].nome, 27, 1, arquivo) != 1) break;  
        ranking.ranking_5[i].nome[27] = '\0';  
        if (fread(&ranking.ranking_5[i].pontuacao, sizeof(int), 1, arquivo) != 1) break;  
    }

    // 4. Ler ranking 6x6
    for (int i = 0; i < ranking.quantidade_6 && i < 10; i++) { 
        if (fread(ranking.ranking_6[i].nome, 27, 1, arquivo) != 1) break;  
        ranking.ranking_6[i].nome[27] = '\0';  
        if (fread(&ranking.ranking_6[i].pontuacao, sizeof(int), 1, arquivo) != 1) break;  
    }

    fclose(arquivo);  
    return ranking;  // Retorna ranking preenchido
}

// Função 3: Exibe o ranking completo no terminal
void mostrar_ranking(void) {
    int result = system("clear");  // Limpa a tela
    (void)result;  // Ignora resultado

    printf(AZUL NEGRITO "══════════════════════════════════════════════════\n" RESET);
    printf(AZUL NEGRITO "                🏆 RANKING 2048 🏆\n" RESET);
    printf(AZUL NEGRITO "══════════════════════════════════════════════════\n" RESET);

    RankingGeral ranking = carregar_ranking("ranking.dat");  // Carrega dados do ranking

    // Vetor para acesso aos dados
    char *nomes_tabuleiros[] = {"4x4", "5x5", "6x6"};  // Nomes dos tamanhos
    int quantidades[] = {ranking.quantidade_4, ranking.quantidade_5, ranking.quantidade_6};  // Quantidades
    EntradaRanking *posicao[] = {ranking.ranking_4, ranking.ranking_5, ranking.ranking_6};  // Vetor de entradas

    // Loop pelos 3 tipos de tabuleiro
    for (int i = 0; i < 3; i++) {
        printf(AMARELO "\n🎮 Tabuleiro %s - %d registros\n" RESET, nomes_tabuleiros[i], quantidades[i]);
        
        if (quantidades[i] == 0) {  // Se não há registros
            printf(CIANA "   📭 (Nenhum registro)\n" RESET);
            continue;  // Pula para próximo tabuleiro
        }

        // Ordenar por pontuação (decrescente)
        for (int j = 0; j < quantidades[i] - 1; j++) {  
            for (int k = j + 1; k < quantidades[i]; k++) {  
                if (posicao[i][k].pontuacao > posicao[i][j].pontuacao) {  // Se precisa trocar
                    EntradaRanking temp = posicao[i][j];  // Armazena temporariamente
                    posicao[i][j] = posicao[i][k];  // Troca posições
                    posicao[i][k] = temp;  // Completa troca
                }
            }
        }

        // Exibe entradas ordenadas
        for (int j = 0; j < quantidades[i]; j++) {
            printf(MAGENTA "   %2d) 👤 %s - %d pontos\n" RESET, 
                   j + 1, posicao[i][j].nome, posicao[i][j].pontuacao);  // Formata linha
        }
    }

    // Rodapé
    printf(AZUL NEGRITO "\n══════════════════════════════════════════════════\n" RESET);
    printf(AMARELO "\n⏎ Pressione ENTER para voltar...\n" RESET);
    getchar();  // Limpa buffer
    getchar();  // Aguarda ENTER
}

// Função 4: Salva a estrutura de ranking no arquivo
void salvar_ranking(const char *nome_arquivo, RankingGeral *ranking) {
    FILE *arquivo = fopen(nome_arquivo, "wb");  // Abre para escrita binária
    if (!arquivo) {  // Verifica se abriu corretamente
        printf(VERMELHO "❌ Erro ao salvar ranking\n" RESET);
        return;  
    }

    // 1. Escrever quantidades 
    size_t written = 0;  // Contador de escritas
    written += fwrite(&ranking->quantidade_4, sizeof(int), 1, arquivo);  // quantidade_4
    written += fwrite(&ranking->quantidade_5, sizeof(int), 1, arquivo);  // quantidade_5
    written += fwrite(&ranking->quantidade_6, sizeof(int), 1, arquivo);  // quantidade_6

    // 2. Escrever ranking 4x4 (27 chars + 1 int para cada entrada)
    for (int i = 0; i < ranking->quantidade_4 && i < 10; i++) {  // Para cada entrada 4x4
        written += fwrite(ranking->ranking_4[i].nome, 27, 1, arquivo);  // Escreve nome
        written += fwrite(&ranking->ranking_4[i].pontuacao, sizeof(int), 1, arquivo);  // Escreve pontuação
    }

    // 3. Escrever ranking 5x5
    for (int i = 0; i < ranking->quantidade_5 && i < 10; i++) {  
        written += fwrite(ranking->ranking_5[i].nome, 27, 1, arquivo);  
        written += fwrite(&ranking->ranking_5[i].pontuacao, sizeof(int), 1, arquivo);  
    }

    // 4. Escrever ranking 6x6
    for (int i = 0; i < ranking->quantidade_6 && i < 10; i++) {  
        written += fwrite(ranking->ranking_6[i].nome, 27, 1, arquivo); 
        written += fwrite(&ranking->ranking_6[i].pontuacao, sizeof(int), 1, arquivo);  
    }

    fclose(arquivo);  
}

// Função 5: Atualiza o ranking com a pontuação do jogo atual, se merecer
void atualizar_ranking(Jogo *jogo) {
    RankingGeral ranking = carregar_ranking("ranking.dat");  // Carrega ranking atual
    
    EntradaRanking *posicao_atual;  
    int *quantidade_atual;  
    const int max_tamanho = 10;  // Tamanho máximo do ranking

    // Seleciona categoria baseada no tamanho do tabuleiro
    switch (jogo->tamanho_tabuleiro) {
        case 4:
            posicao_atual = ranking.ranking_4;  // Ranking 4x4
            quantidade_atual = &ranking.quantidade_4;  // Quantidade 4x4
            break;
        case 5:
            posicao_atual = ranking.ranking_5;  // Ranking 5x5
            quantidade_atual = &ranking.quantidade_5;  // Quantidade 5x5
            break;
        case 6:
            posicao_atual = ranking.ranking_6;  // Ranking 6x6
            quantidade_atual = &ranking.quantidade_6;  // Quantidade 6x6
            break;
        default:
            return;  // Tamanho inválido
    }

    // Verificar se merece entrar no ranking
    int merece_entrar = 0;  
    if (*quantidade_atual < max_tamanho) {  // Se ainda tem espaço
        merece_entrar = 1;  
    } else if (jogo->pontuacao > posicao_atual[max_tamanho - 1].pontuacao) {  // Se pontuação é maior que a última
        merece_entrar = 1;  // Merece entrar
    }

    if (merece_entrar) {  // Se merece entrar no ranking
        if (*quantidade_atual < max_tamanho) {  // Se ainda tem espaço
            (*quantidade_atual)++;  // Incrementa quantidade
        }

        // Adicionar nova entrada na última posição (CORRIGIDO - Opção 1)
        snprintf(posicao_atual[*quantidade_atual - 1].nome, 28, "%.27s", jogo->nome_jogador);
        
        // Ou use a Opção 2:
        // strncpy(posicao_atual[*quantidade_atual - 1].nome, jogo->nome_jogador, 27);
        // posicao_atual[*quantidade_atual - 1].nome[27] = '\0';
        
        posicao_atual[*quantidade_atual - 1].pontuacao = jogo->pontuacao;  // Define pontuação

        // Ordenar do maior para o menor (decrescente)
        for (int i = 0; i < *quantidade_atual - 1; i++) {  
            for (int j = i + 1; j < *quantidade_atual; j++) {  
                if (posicao_atual[j].pontuacao > posicao_atual[i].pontuacao) {  // Se precisa trocar
                    EntradaRanking temp = posicao_atual[i];  // Armazena temporariamente
                    posicao_atual[i] = posicao_atual[j];  // Troca posições
                    posicao_atual[j] = temp;  // Completa troca
                }
            }
        }
        
        salvar_ranking("ranking.dat", &ranking);  // Salva ranking atualizado
        printf(VERDE "📊 Ranking atualizado! Sua pontuação: %d\n" RESET, jogo->pontuacao);
    }
}