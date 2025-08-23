// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "menu.h"
#include "funcoes.h"
#include "salva_jogo.h"
#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


// Função A: Funçãõ do menu
void menu(void) {
    char opcao;
    Jogo jogo_atual; // Jogo atual
    int jogo_iniciado = 0;

    // Inicializa jogo
    jogo_atual.matriz_tab = NULL;
    jogo_atual.tamanho_tab = 0;
    jogo_atual.pontos = 0;

    do {
        printf("\n===== MENU 2048 =====\n");
        printf("(N) Novo jogo\n");
        printf("(J) Continuar jogo atual\n");
        printf("(C) Carregar jogo salvo\n");
        printf("(S) Salvar jogo atual\n");
        printf("(M) Mostrar ranking\n");
        printf("(A) Ajuda\n");
        printf("(R) Sair\n");
        printf("=====================\n");
        printf("Escolha uma opcao: ");

        if (scanf(" %c", &opcao) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }

        opcao = toupper(opcao);

        switch (opcao) {
            case 'N':
                novo_jogo(&jogo_atual);
                jogo_iniciado = 1;
                break;

            case 'J':
                if (jogo_iniciado) {
                    continuar_jogo(&jogo_atual);
                } else {
                    printf("Nenhum jogo iniciado. Inicie um novo jogo primeiro.\n");
                }
                break;

            case 'C':
                if (carregar_jogo(&jogo_atual)) {
                    jogo_iniciado = 1;
                } else {
                    printf("Falha ao carregar jogo.\n");
                }
                break;

            case 'S':
                if (jogo_iniciado) {
                    salvar_jogo(&jogo_atual);
                } else {
                    printf("Nenhum jogo iniciado para salvar.\n");
                }
                break;

            case 'M':
                mostrar_ranking();
                break;

            case 'A':
                mostrar_ajuda();
                break;

            case 'R':
                sair(&jogo_atual);
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 'R');
}

// Função B: Função para sair do jogo
void sair(Jogo *jogo_atual) {
    char opcao;

    printf("\nDeseja salvar o jogo antes de sair? (S/N): ");
    scanf(" %c", &opcao);
    opcao = toupper(opcao);

    if (opcao == 'S' && jogo_atual->matriz_tab != NULL) {
        salvar_jogo(jogo_atual);
    }

    // Libera memória do tabuleiro
    if (jogo_atual->matriz_tab != NULL) {
        liberar_matriz(jogo_atual->matriz_tab, jogo_atual->tamanho_tab);
        jogo_atual->matriz_tab = NULL;
    }

    printf("Saindo do jogo. Até mais!\n");
    exit(0);
}

// Função C: Criar novo jogo
void novo_jogo(Jogo *jogo) {
    int tamanho;

    do {
        printf("Escolha o tamanho do tabuleiro (4, 5 ou 6): ");
        scanf("%d", &tamanho);
        if (tamanho < 4 || tamanho > 6) {
            printf("Tamanho invalido! Tente novamente.\n");
        }
    } while (tamanho < 4 || tamanho > 6);

    jogo->tamanho_tab = tamanho;

    printf("Digite o nome do jogador: ");
    getchar(); // limpa buffer
    fgets(jogo->nome, sizeof(jogo->nome), stdin);
    jogo->nome[strcspn(jogo->nome, "\n")] = 0;

    jogo->pontos = 0;
    jogo->desfazer_restantes = 0;
    jogo->trocas_restantes = 0;

    // Aloca e inicializa matriz
    jogo->matriz_tab = criar_matriz(tamanho);

    printf("Novo jogo criado para %s com tabuleiro %dx%d\n", jogo->nome, tamanho, tamanho);
    mostrar_tabuleiro(jogo);
}

// Função D: Continuar jogo
void continuar_jogo(Jogo *jogo_atual) {
    if (jogo_atual == NULL || jogo_atual->matriz_tab == NULL) {
        printf("Nenhum jogo em andamento.\n");
        return;
    }

    printf("Continuando jogo de %s com tabuleiro %dx%d e %d pontos.\n",
           jogo_atual->nome, jogo_atual->tamanho_tab, jogo_atual->tamanho_tab, jogo_atual->pontos);
    mostrar_tabuleiro(jogo_atual);
}

// Função E: função ajuda/instruções
void mostrar_ajuda() {
    printf("\n=== Instruções do Jogo 2048 ===\n");
    printf("Comandos durante o jogo:\n");
    printf("  a - mover para a esquerda\n");
    printf("  d - mover para a direita\n");
    printf("  w - mover para cima\n");
    printf("  s - mover para baixo\n");
    printf("  u - desfazer o último movimento\n");
    printf("  t pos1 pos2 - trocar peças\n");
    printf("  voltar - retornar ao menu\n\n");
    printf("No menu inicial, use as opções:\n");
    printf("  (R) Sair\n");
    printf("  (N) Novo jogo\n");
    printf("  (J) Continuar\n");
    printf("  (C) Carregar\n");
    printf("  (S) Salvar\n");
    printf("  (M) Mostrar Ranking\n");
    printf("  (A) Ajuda\n\n");
    printf("Pressione ENTER para voltar ao menu...\n");
    getchar();
}

// ==============================
// Função para salvar jogo
// ==============================
void salvar_jogo(Jogo *jogo) {
    if (jogo == NULL || jogo->matriz_tab == NULL) return;

    FILE *arquivo = fopen("jogo_salvo.dat", "wb");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para salvar.\n");
        return;
    }

    fwrite(&jogo->tamanho_tab, sizeof(int), 1, arquivo);
    fwrite(&jogo->pontos, sizeof(int), 1, arquivo);
    fwrite(jogo->nome, sizeof(char), 100, arquivo);
    fwrite(&jogo->desfazer_restantes, sizeof(int), 1, arquivo);
    fwrite(&jogo->trocas_restantes, sizeof(int), 1, arquivo);

    for (int i = 0; i < jogo->tamanho_tab; i++) {
        fwrite(jogo->matriz_tab[i], sizeof(int), jogo->tamanho_tab, arquivo);
    }

    fclose(arquivo);
    printf("Jogo salvo com sucesso!\n");
}

