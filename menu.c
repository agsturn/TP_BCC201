// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "exemplo.h"
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
    jogo_atual.matriz_atual = NULL;
    jogo_atual.matriz_anterior = NULL;
    jogo_atual.tamanho_tabuleiro = 0;
    jogo_atual.pontuacao = 0;
    jogo_atual.desfazer_disponiveis = 0;
    jogo_atual.trocas_disponiveis = 0;
    jogo_atual.pontuacao_anterior = 0;

    do {
        int result = system("clear");
        (void)result; // Descarta o resultado intencionalmente
        
        // Menu
        printf("\n\n");
        printf(CIANA "════════════════════════════════════════════════\n" RESET);
        printf(AMARELO NEGRITO "                🎮 MENU 2048 🎮\n" RESET);
        printf(CIANA "════════════════════════════════════════════════\n" RESET);
        printf(VERDE "(N) Novo jogo\n");
        printf(AZUL "(J) Continuar jogo atual\n");
        printf(MAGENTA "(C) Carregar jogo salvo\n");
        printf(CIANA "(S) Salvar jogo atual\n");
        printf(AMARELO "(M) Mostrar ranking\n");
        printf(VERMELHO "(A) Ajuda\n");
        printf(NEGRITO "(R) Sair\n" RESET);
        printf(CIANA "════════════════════════════════════════════════\n" RESET);
        printf("\n");
        printf(AMARELO "Escolha uma opção (N, J, C, S, M, A, R): " RESET);

        // Tratamento seguro da entrada
        char entrada[10];
        if (scanf("%9s", entrada) != 1) {
            limpar_buffer();
            printf(VERMELHO "\nEntrada inválida! Use apenas las opções do menu.\n" RESET);
            printf(AMARELO "Pressione ENTER para continuar..." RESET);
            getchar();
            continue;
        }
        
        opcao = toupper(entrada[0]);
        
        // Verifica se é uma opção válida
        if (strlen(entrada) > 1 || (opcao != 'N' && opcao != 'J' && opcao != 'C' && 
            opcao != 'S' && opcao != 'M' && opcao != 'A' && opcao != 'R')) {
            printf(VERMELHO "\nOpção inválida! Use apenas: N, J, C, S, M, A, R\n" RESET);
            printf(AMARELO "Pressione ENTER para continuar..." RESET);
            getchar();
            continue;
        }

        switch (opcao) {
            case 'R':
                printf(AMARELO "\nDeseja realmente sair? (S/N): " RESET);
                char resposta;
                if (scanf(" %c", &resposta) != 1) {
                    limpar_buffer();
                    printf(VERMELHO "\nEntrada inválida!\n" RESET);
                    break;
                }
                limpar_buffer();
                if (toupper(resposta) == 'S') {
                    sair(&jogo_atual);
                    return;
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
                    printf(VERMELHO "\nNenhum jogo em andamento!\n" RESET);
                }
                break;

            case 'C':
                printf(CIANA "\n📂 Nome do arquivo: " RESET);
                char nome_arquivo[100];
                if (scanf("%99s", nome_arquivo) != 1) {
                    limpar_buffer();
                    printf(VERMELHO "\nEntrada inválida!\n" RESET);
                    break;
                }
                limpar_buffer();
                carregar_jogo(&jogo_atual, nome_arquivo);
                jogo_iniciado = 1;
                continuar_jogo(&jogo_atual);
                break;

            case 'S':
                if (jogo_iniciado) {
                    printf(CIANA "\n💾 Nome do arquivo: " RESET);
                    char nome_salvar[100];
                    if (scanf("%99s", nome_salvar) != 1) {
                        limpar_buffer();
                        printf(VERMELHO "\nEntrada inválida!\n" RESET);
                        break;
                    }
                    limpar_buffer();
                    salvar_jogo(&jogo_atual, nome_salvar);
                } else {
                    printf(VERMELHO "\nNenhum jogo para salvar!\n" RESET);
                }
                break;

            case 'M':
                mostrar_ranking();
                break;

            case 'A':
                mostrar_ajuda();
                break;
        }

        printf(AMARELO "\nPressione ENTER para continuar..." RESET);
        getchar();

    } while (opcao != 'R');
}

void sair(Jogo *jogo) {
    char opcao;

    printf(AMARELO "\nDeseja salvar antes de sair? (S/N): " RESET);
    if (scanf(" %c", &opcao) != 1) {
        limpar_buffer();
        printf(VERMELHO "\nEntrada inválida!\n" RESET);
        return;
    }
    opcao = toupper(opcao);

    if (opcao == 'S' && jogo->matriz_atual != NULL) {
        printf(CIANA "Nome do arquivo: " RESET);
        char nome_arquivo[100];
        if (scanf("%s", nome_arquivo) != 1) {
            limpar_buffer();
            printf(VERMELHO "\nEntrada inválida!\n" RESET);
            return;
        }
        salvar_jogo(jogo, nome_arquivo);
    }

    // Libera a memória
    if (jogo->matriz_atual != NULL) {
        liberar_matriz(jogo->matriz_atual, jogo->tamanho_tabuleiro);
    }
    if (jogo->matriz_anterior != NULL) {
        liberar_matriz(jogo->matriz_anterior, jogo->tamanho_tabuleiro);
    }

    printf(VERDE "\nObrigado por jogar! Até mais! 👋\n" RESET);
}

void novo_jogo(Jogo *jogo) {
    int tamanho;

    printf(AMARELO "\nEscolha o tamanho do tabuleiro (4, 5 ou 6): " RESET);
    if (scanf("%d", &tamanho) != 1) {
        limpar_buffer();
        printf(VERMELHO "\nEntrada inválida!\n" RESET);
        return;
    }
    
    while (tamanho < 4 || tamanho > 6) {
        printf(VERMELHO "Tamanho inválido! Escolha 4, 5 ou 6: " RESET);
        if (scanf("%d", &tamanho) != 1) {
            limpar_buffer();
            printf(VERMELHO "\nEntrada inválida!\n" RESET);
            return;
        }
    }

    jogo->tamanho_tabuleiro = tamanho;

    printf(AMARELO "Digite seu nome: " RESET);
    getchar();
    if (fgets(jogo->nome_jogador, sizeof(jogo->nome_jogador), stdin) == NULL) {
        printf(VERMELHO "\nErro ao ler nome!\n" RESET);
        return;
    }
    
    // Remove o enter do final do nome
    for (int i = 0; jogo->nome_jogador[i] != '\0'; i++) {
        if (jogo->nome_jogador[i] == '\n') {
            jogo->nome_jogador[i] = '\0';
            break;
        }
    }

    // Inicializa valores corretamente
    jogo->pontuacao = 0;
    jogo->desfazer_disponiveis = 0;
    jogo->trocas_disponiveis = 0;
    jogo->pontuacao_anterior = 0;

    // Cria o tabuleiro
    jogo->matriz_atual = criar_matriz(tamanho);
    jogo->matriz_anterior = NULL;

    // Adiciona números iniciais
    adicionar_numero_aleatorio(jogo);
    adicionar_numero_aleatorio(jogo);

    printf(VERDE "\nJogo criado com sucesso! Boa sorte! 🍀\n" RESET);
    printf(AMARELO "Pressione ENTER para começar..." RESET);
    getchar();

    continuar_jogo(jogo);
}

void continuar_jogo(Jogo *jogo) {
    if (jogo == NULL || jogo->matriz_atual == NULL) {
        printf(VERMELHO "\n⚠️  Nenhum jogo em andamento.\n" RESET);
        printf(AMARELO "\nPressione ENTER para continuar..." RESET);
        getchar();
        return;
    }

    char comando[20];
    int movimento_valido;
    
    while (1) {
        int result = system("clear");
        (void)result; // Descarta o resultado intencionalmente
        
        imprimir_tabuleiro(jogo);
        
        printf(AMARELO "Digite comando: " RESET);
        if (scanf("%s", comando) != 1) {
            limpar_buffer();
            printf(VERMELHO "\nEntrada inválida!\n" RESET);
            continue;
        }
        limpar_buffer();
        
        if (strcmp(comando, "voltar") == 0) {
            printf(VERDE "\nVoltando ao menu principal...\n" RESET);
            break;
        }
        
        movimento_valido = 0;
        
        // Salva estado atual para poder desfazer
        if (jogo->matriz_anterior) {
            liberar_matriz(jogo->matriz_anterior, jogo->tamanho_tabuleiro);
        }
        jogo->matriz_anterior = copiar_matriz(jogo->matriz_atual, jogo->tamanho_tabuleiro);
        jogo->pontuacao_anterior = jogo->pontuacao;
        
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
                trocar_pecas(jogo); // CORREÇÃO: trocar_pecas_interativo → trocar_pecas
                movimento_valido = 1;
                break;
            default:
                printf(VERMELHO "\nComando inválido! Tente novamente.\n" RESET);
                printf(AMARELO "Pressione ENTER para continuar..." RESET);
                getchar();
                continue;
        }
        
        // Se foi um movimento válido, adiciona número novo
        if (movimento_valido && comando[0] != 'u' && comando[0] != 't') {
            adicionar_numero_aleatorio(jogo);
        }
        
        // Verifica se ganhou
        int status_vitoria = verificar_vitoria(jogo);
        if (status_vitoria == 0) {
            break; // Jogador venceu e quer sair
        }
        
        // Verifica se perdeu (apenas se foi um movimento ou troca válida)
        if (movimento_valido) {
            int resultado_derrota = verificar_derrota(jogo);
            if (resultado_derrota == 0) {
                break; // Fim de jogo definitivo
            }
            // Se resultado_derrota == 2, continua o jogo (usou desfazer)
        }
    }
}

void mostrar_ajuda() {
    int result = system("clear");
    (void)result; // Descarta o resultado intencionalmente
    
    printf(CIANA "══════════════════════════════════════════════════\n" RESET);
    printf(AMARELO NEGRITO "                📚 AJUDA 2048 📚\n" RESET);
    printf(CIANA "══════════════════════════════════════════════════\n" RESET);

    printf("\n");
    printf(VERDE "Como Jogar:\n" RESET);
    printf("Use W, A, S, D para mover as peças\n");
    printf("Combine números iguais para somar\n");
    printf("Tente chegar no 2048!\n");

    printf("\n");
    printf(CIANA "══════════════════════════════════════════════════\n" RESET);

    printf("\n");
    printf(AMARELO "Comandos:\n" RESET);
    printf(VERDE "  W - Mover para cima ↗️\n");
    printf(AZUL "  S - Mover para baixo ↙️\n");
    printf(MAGENTA "  A - Mover para esquerda ↪️\n");
    printf(CIANA "  D - Mover para direita ↩️\n");
    printf(AMARELO "  U - Desfazer movimento ↩️\n");
    printf(VERMELHO "  T - Trocar duas peças 🔄\n");
    printf(NEGRITO "  VOLTAR - Voltar ao menu 🏠\n" RESET);

    printf("\n");
    printf(CIANA "══════════════════════════════════════════════════\n" RESET);
    
    printf("\n");
    printf(AMARELO "Pressione ENTER para voltar...\n" RESET);
    getchar();
}