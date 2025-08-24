// Ana Gabriela Gomes Lopes Pereira - 25.1.4119
#include "funcoes.h"
#include "menu.h"
#include "salva_jogo.h"
#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void menu() {
    char opcao;
    Jogo jogo_atual;
    int jogo_iniciado = 0;

    // Inicializa o jogo
    jogo_atual.matriz_tab = NULL;
    jogo_atual.tab_anterior = NULL;
    jogo_atual.tamanho_tab = 0;
    jogo_atual.pontos = 0;
    jogo_atual.desfazer_restantes = 0;
    jogo_atual.trocas_restantes = 0;

    do {
        system("clear");
        
        // Menu CENTRALIZADO
        printf("\n");
        centralizar(CIANA "╔══════════════════════════════════════════════╗" RESET, 60);
        centralizar(CIANA "║" AMARELO "               🎮 MENU 2048 🎮              " CIANA "║" RESET, 60);
        centralizar(CIANA "╠══════════════════════════════════════════════╣" RESET, 60);
        centralizar(CIANA "║" VERDE " (N) Novo jogo                         " CIANA "║" RESET, 60);
        centralizar(CIANA "║" VERDE " (J) Continuar jogo atual              " CIANA "║" RESET, 60);
        centralizar(CIANA "║" VERDE " (C) Carregar jogo salvo               " CIANA "║" RESET, 60);
        centralizar(CIANA "║" VERDE " (S) Salvar jogo atual                 " CIANA "║" RESET, 60);
        centralizar(CIANA "║" VERDE " (M) Mostrar ranking                   " CIANA "║" RESET, 60);
        centralizar(CIANA "║" VERDE " (A) Ajuda                             " CIANA "║" RESET, 60);
        centralizar(CIANA "║" VERDE " (R) Sair                              " CIANA "║" RESET, 60);
        centralizar(CIANA "╚══════════════════════════════════════════════╝" RESET, 60);
        
        printf("\n");
        printf(AMARELO " Escolha uma opção (N, J, C, S, M, A, R): " RESET);

        // Tratamento seguro da entrada
        char entrada[10];
        if (scanf("%9s", entrada) != 1) {
            limpar_buffer();
            printf(VERMELHO "\n Entrada inválida! Use apenas as opções do menu.\n" RESET);
            printf(AMARELO " Pressione ENTER para continuar..." RESET);
            getchar();
            continue;
        }
        
        opcao = toupper(entrada[0]);
        
        // Verifica se é uma opção válida
        if (strlen(entrada) > 1 || (opcao != 'N' && opcao != 'J' && opcao != 'C' && 
            opcao != 'S' && opcao != 'M' && opcao != 'A' && opcao != 'R')) {
            printf(VERMELHO "\n Opção inválida! Use apenas: N, J, C, S, M, A, R\n" RESET);
            printf(AMARELO " Pressione ENTER para continuar..." RESET);
            getchar();
            continue;
        }

        switch (opcao) {
            case 'R':
                printf(AMARELO "\n Deseja realmente sair? (S/N): " RESET);
                char resposta;
                if (scanf(" %c", &resposta) != 1) {
                    limpar_buffer();
                    printf(VERMELHO "\n Entrada inválida!\n" RESET);
                    break;
                }
                limpar_buffer();
                if (toupper(resposta) == 'S') {
                    sair(&jogo_atual);
                }
                break;

            case 'N':
                novo_jogo(&jogo_atual);
                jogo_iniciado = 1;
                break;

            case 'J':
                if (jogo_iniciado) {
                    continuar_jogo(&jogo_atual);
                } else {
                    printf(VERMELHO "\n Nenhum jogo em andamento!\n" RESET);
                }
                break;

            case 'C':
                printf(AMARELO "\n Nome do arquivo: " RESET);
                char nome_arquivo[100];
                if (scanf("%99s", nome_arquivo) != 1) {
                    limpar_buffer();
                    printf(VERMELHO "\n Entrada inválida!\n" RESET);
                    break;
                }
                limpar_buffer();
                carregar_jogo(&jogo_atual, nome_arquivo);
                jogo_iniciado = 1;
                continuar_jogo(&jogo_atual);
                break;

            case 'S':
                if (jogo_iniciado) {
                    printf(AMARELO "\n Nome do arquivo: " RESET);
                    char nome_salvar[100];
                    if (scanf("%99s", nome_salvar) != 1) {
                        limpar_buffer();
                        printf(VERMELHO "\n Entrada inválida!\n" RESET);
                        break;
                    }
                    limpar_buffer();
                    salvar_jogo(&jogo_atual, nome_salvar);
                } else {
                    printf(VERMELHO "\n Nenhum jogo para salvar!\n" RESET);
                }
                break;

            case 'M':
                mostrar_ranking();
                break;

            case 'A':
                mostrar_ajuda();
                break;
        }

        printf(AMARELO "\n Pressione ENTER para continuar..." RESET);
        getchar();

    } while (opcao != 'R');
}

void sair(Jogo *jogo) {
    char opcao;

    printf(AMARELO "\n Deseja salvar antes de sair? (S/N): " RESET);
    scanf(" %c", &opcao);
    opcao = toupper(opcao);

    if (opcao == 'S' && jogo->matriz_tab != NULL) {
        printf(AMARELO " Nome do arquivo: " RESET);
        char nome_arquivo[100];
        scanf("%s", nome_arquivo);
        salvar_jogo(jogo, nome_arquivo);
    }

    // Libera a memória
    if (jogo->matriz_tab != NULL) {
        liberar_matriz(jogo->matriz_tab, jogo->tamanho_tab);
    }
    if (jogo->tab_anterior != NULL) {
        liberar_matriz(jogo->tab_anterior, jogo->tamanho_tab);
    }

    printf(VERDE "\n Obrigado por jogar! Até mais! 👋\n" RESET);
    exit(0);
}

void novo_jogo(Jogo *jogo) {
    int tamanho;

    printf(AMARELO "\n Escolha o tamanho do tabuleiro (4, 5 ou 6): " RESET);
    scanf("%d", &tamanho);
    
    while (tamanho < 4 || tamanho > 6) {
        printf(VERMELHO " Tamanho inválido! Escolha 4, 5 ou 6: " RESET);
        scanf("%d", &tamanho);
    }

    jogo->tamanho_tab = tamanho;

    printf(AMARELO " Digite seu nome: " RESET);
    getchar();
    fgets(jogo->nome, sizeof(jogo->nome), stdin);
    
    // Remove o enter do final do nome
    for (int i = 0; jogo->nome[i] != '\0'; i++) {
        if (jogo->nome[i] == '\n') {
            jogo->nome[i] = '\0';
            break;
        }
    }

    // Inicializa valores
    jogo->pontos = 0;
    jogo->desfazer_restantes = 3;
    jogo->trocas_restantes = 3;

    // Cria o tabuleiro
    jogo->matriz_tab = criar_matriz(tamanho);
    jogo->tab_anterior = NULL;

    // Adiciona números iniciais
    n_aleatorio(jogo);
    n_aleatorio(jogo);

    printf(VERDE "\n Jogo criado com sucesso! Boa sorte! 🍀\n" RESET);
    printf(AMARELO " Pressione ENTER para começar..." RESET);
    getchar();

    continuar_jogo(jogo);
}

void continuar_jogo(Jogo *jogo) {
    if (jogo == NULL || jogo->matriz_tab == NULL) {
        printf(VERMELHO "\n⚠️  Nenhum jogo em andamento.\n" RESET);
        printf("\nPressione ENTER para continuar...");
        getchar();
        return;
    }

    char comando[20];
    int movimento_valido;
    
    while (1) {
        system("clear");
        imprimir_matriz(jogo);
        
        printf(AMARELO "Digite comando: " RESET);
        scanf("%s", comando);
        limpar_buffer();
        
        if (strcmp(comando, "voltar") == 0) {
            printf(VERDE "\nVoltando ao menu principal...\n" RESET);
            break;
        }
        
        movimento_valido = 0;
        
        // Salva estado atual para poder desfazer
        if (jogo->tab_anterior) {
            liberar_matriz(jogo->tab_anterior, jogo->tamanho_tab);
        }
        jogo->tab_anterior = copiar_matriz(jogo->matriz_tab, jogo->tamanho_tab);
        jogo->pontos_anterior = jogo->pontos;
        
        // Processa o comando do jogador
        switch (comando[0]) {
            case 'a': case 'A':
                movimento_valido = mover_esquerda(jogo);
                break;
            case 'd': case 'D':
                movimento_valido = mover_direita(jogo);
                break;
            case 'w': case 'W':
                movimento_valido = mover_cima(jogo);
                break;
            case 's': case 'S':
                movimento_valido = mover_baixo(jogo);
                break;
            case 'u': case 'U':
                desfazer_movimento(jogo);
                movimento_valido = 1;
                break;
            case 't': case 'T':
                trocar_pecas(jogo);
                movimento_valido = 1;
                break;
            default:
                printf(VERMELHO "\nComando inválido! Tente novamente.\n" RESET);
                printf("Pressione ENTER para continuar...");
                getchar();
                continue;
        }
        
        // Se foi um movimento válido, adiciona número novo
        if (movimento_valido && comando[0] != 'u' && comando[0] != 't') {
            n_aleatorio(jogo);
        }
        
        // Verifica se ganhou ou perdeu
        int resultado = verificar_vitoria(jogo);
        if (resultado == 0) {
            break;
        }
        
        resultado = verificar_derrota(jogo);
        if (resultado == 0) {
            break;
        }
    }
}

void mostrar_ajuda() {
    system("clear");
    
    printf(CIANA "╔══════════════════════════════════════════════╗\n" RESET);
    printf(CIANA "║" AMARELO "               📚 AJUDA 2048 📚              " CIANA "║\n" RESET);
    printf(CIANA "╠══════════════════════════════════════════════╣\n" RESET);
    
    printf(CIANA "║" VERDE "          Como Jogar:                   " CIANA "║\n" RESET);
    printf(CIANA "║" MAGENTA " Use W, A, S, D para mover as peças       " CIANA "║\n" RESET);
    printf(CIANA "║" MAGENTA " Combine números iguais para somar        " CIANA "║\n" RESET);
    printf(CIANA "║" MAGENTA " Tente chegar no 2048!                   " CIANA "║\n" RESET);
    
    printf(CIANA "╠══════════════════════════════════════════════╣\n" RESET);
    
    printf(CIANA "║" VERDE "          Comandos:                      " CIANA "║\n" RESET);
    printf(CIANA "║" AZUL " W - Mover para cima ↗️                 " CIANA "║\n" RESET);
    printf(CIANA "║" AZUL " S - Mover para baixo ↙️                " CIANA "║\n" RESET);
    printf(CIANA "║" AZUL " A - Mover para esquerda ↪️             " CIANA "║\n" RESET);
    printf(CIANA "║" AZUL " D - Mover para direita ↩️              " CIANA "║\n" RESET);
    printf(CIANA "║" AZUL " U - Desfazer movimento ↩️              " CIANA "║\n" RESET);
    printf(CIANA "║" AZUL " T - Trocar duas peças 🔄               " CIANA "║\n" RESET);
    printf(CIANA "║" AZUL " VOLTAR - Voltar ao menu 🏠             " CIANA "║\n" RESET);
    
    printf(CIANA "╚══════════════════════════════════════════════╝\n" RESET);
    
    printf("\n");
    printf(AMARELO "        Pressione ENTER para voltar...\n" RESET);
    getchar();
}