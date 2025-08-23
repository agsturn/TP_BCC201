// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "menu.h"
#include "funcoes.h"
#include "salva_jogo.h"
#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Função A: Mostra o menu principal e trata a escolha do usuário
void menu( ) {
    char opcao;
    Jogo jogo_atual; // Estrutura do jogo atual
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

        // Lê entrada do usuário e limpa buffer se inválida
        if (scanf(" %c", &opcao) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }

        opcao = toupper(opcao);

        switch (opcao) {
            case 'N': // Novo jogo
                novo_jogo(&jogo_atual);
                jogo_iniciado = 1;
                break;

            case 'J': // Continuar jogo atual
                if (jogo_iniciado) {
                    continuar_jogo(&jogo_atual);
                } else {
                    printf("Nenhum jogo iniciado. Inicie um novo jogo primeiro.\n");
                }
                break;

            case 'C': // Carregar jogo salvo
                carregar_jogo(&jogo_atual, "jogo_salvo.dat");
                jogo_iniciado = 1;
                break;

            case 'S': // Salvar jogo atual
                if (jogo_iniciado) {
                    salvar_jogo(&jogo_atual, "jogo_salvo.txt");
                } else {
                    printf("Nenhum jogo iniciado para salvar.\n");
                }
                break;

            case 'M': // Mostrar ranking
                mostrar_ranking();
                break;

            case 'A': // Ajuda
                mostrar_ajuda();
                break;

            case 'R': // Sair
                sair(&jogo_atual);
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 'R');
}

// Função B: Pergunta ao usuário se deseja salvar antes de sair e libera memória
void sair(Jogo *jogo_atual) {
    char opcao;

    printf("\nDeseja salvar o jogo antes de sair? (S/N): ");
    scanf(" %c", &opcao);
    opcao = toupper(opcao);

    if (opcao == 'S' && jogo_atual->matriz_tab != NULL) {
        salvar_jogo(jogo_atual, "jogo_salvo.txt");
    }

    // Libera memória do tabuleiro
    if (jogo_atual->matriz_tab != NULL) {
        liberar_matriz(jogo_atual->matriz_tab, jogo_atual->tamanho_tab);
        jogo_atual->matriz_tab = NULL;
    }

    printf("Saindo do jogo. Até mais!\n");
    exit(0);
}

// Função C: Inicializa um novo jogo
void novo_jogo(Jogo *jogo) {
    int tamanho;

    // Solicita o tamanho do tabuleiro
    do {
        printf("Escolha o tamanho do tabuleiro (4, 5 ou 6): ");
        scanf("%d", &tamanho);
        if (tamanho < 4 || tamanho > 6) {
            printf("Tamanho invalido! Tente novamente.\n");
        }
    } while (tamanho < 4 || tamanho > 6);

    jogo->tamanho_tab = tamanho;

    // Solicita o nome do jogador
    printf("Digite o nome do jogador: ");
    getchar(); // limpa buffer
    fgets(jogo->nome, sizeof(jogo->nome), stdin);

    // Remove o '\n' do final do nome
    int i = 0;
    while (jogo->nome[i] != '\0') {
        if (jogo->nome[i] == '\n') {
            jogo->nome[i] = '\0';
            break;
        }
        i++;
    }

    // Inicializa variáveis do jogo
    jogo->pontos = 0;
    jogo->desfazer_restantes = 0;
    jogo->trocas_restantes = 0;

    // Aloca matriz do tabuleiro
    jogo->matriz_tab = criar_matriz(tamanho);

    printf("Novo jogo criado para %s com tabuleiro %dx%d\n", jogo->nome, tamanho, tamanho);

    // Mostra o tabuleiro inicial
    imprimir_matriz(jogo);
}

// Função D: Mostra informações do jogo atual
void continuar_jogo(Jogo *jogo_atual) {
    if (jogo_atual == NULL || jogo_atual->matriz_tab == NULL) {
        printf("Nenhum jogo em andamento.\n");
        return;
    }

    printf("Continuando jogo de %s com tabuleiro %dx%d e %d pontos.\n",
           jogo_atual->nome, jogo_atual->tamanho_tab, jogo_atual->tamanho_tab, jogo_atual->pontos);
    imprimir_matriz(jogo_atual);
}

// Função E: Mostra instruções do jogo
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