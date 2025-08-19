// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "menu.h"
#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Função A:Função para executar o menu e direcionar a função do comando escolhido
void menu(void) {
    char opcao;
    Jogo jogo_atual;//Declara o jogo atual
    int jogo_iniciado = 0;//Variavel para saber se já tem jogo ativo

    // Inicializa o tabuleiro com NULL para evitar acessar lixo
    jogo_atual.matriz_tab = NULL;
    jogo_atual.tamanho_tab = 0;
    jogo_atual.pontos = 0;

    do {
        printf(CYAN("\n===== MENU 2048 =====\n"));
        printf(BOLD(YELLOW("(N) Novo jogo\n")));
        printf(BOLD(RED("(J) Continuar jogo atual\n")));
        printf(BOLD(GREEN("(C) Carregar jogo salvo\n")));
        printf(BOLD(MAGENTA("(S) Salvar jogo atual\n")));
        printf(BOLD(BLUE("(M) Mostrar ranking\n")));
        printf(BOLD(YELLOW("(A) Ajuda\n")));
        printf(BOLD(RED("(R) Sair\n")));
        printf(CYAN("=====================\n"));
        printf(BOLD("Escolha uma opcao: "));
        if (scanf(" %c", &opcao) != 1) {
            // limpa entrada em caso de erro
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
                    printf("Nenhum jogo iniciado. Por favor, inicie um novo jogo primeiro.\n");
                 }
                break;
            case 'C':
                printf("Carregando jogo salvo...\n");
                // TODO: carregar jogo
                break;
            case 'S':
                printf("Salvando jogo atual...\n");
                // TODO: salvar jogo
                break;
            case 'M':
                printf("Mostrando ranking...\n");
                // TODO: mostrar ranking
                break;
            case 'A':
                mostrar_ajuda();
                break;
            case 'R':
                sair();
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 'R');
}

/*Função B:Função para sair do jogo
void sair() {
    char opcao;

    printf("\nDeseja salvar o jogo antes de sair? (S/N): ");
    scanf(" %c", &opcao);
    opcao = toupper(opcao);

    if (opcao == 'S') {
        //salvar_jogo(); 
        printf("Criar função para salvar o jogo");
    }
    else {
        printf("\nRetornando ao menu...\n");
        menu();
    }

}

//Função C:Função para criar o tabuleiros
void novo_jogo() {
    Jogo jogo;
    int tamanho;

    do {
        printf("Escolha o tamanho do tabuleiro (4 - 4x4, 5 - 5x5, 6 - 6x6): "); //corrigir oara nao colocar letra
        scanf("%d", &tamanho);
        if (tamanho != 4 && tamanho != 5 && tamanho != 6) {
            printf("Tamanho invalido! Tente novamente.\n");
        }
    } while (tamanho != 4 && tamanho != 5 && tamanho != 6);

    jogo.tamanho_tab = tamanho;

    printf("Digite o nome do jogador: ");
    getchar(); // limpa \n do buffer
    fgets(jogo.nome, sizeof(jogo.nome), stdin);
    jogo.nome[strcspn(jogo.nome, "\n")] = 0; // remove \n

    jogo.pontos = 0;

    // Aloca e inicializa a matriz
    jogo.matriz_tab = criar_matriz(tamanho);

  

    printf("Novo jogo criado para %s com tabuleiro %dx%d\n", jogo.nome, tamanho, tamanho);
    mostrar_tabuleiro(&jogo);

    // Aqui normalmente o jogo começaria a rodar (loop principal)
    // Após o uso, libera a matriz
    liberar_matriz(jogo.matriz_tab, tamanho);
}

//Função D:Continuar um jogo
void continuar_jogo(Jogo *jogo_atual) {
    if (jogo_atual == NULL) {
        printf("Nenhum jogo em andamento. Volte ao menu e comece um novo jogo.\n");
        return;
    }
    printf("Continuando jogo de %s com tabuleiro %dx%d e %d pontos.\n",
        jogo_atual->nome, jogo_atual->tamanho_tab, jogo_atual->tamanho_tab, jogo_atual->pontos);
    mostrar_tabuleiro(jogo_atual);
    
}

//Função H:
//Função I:
//Função J:
//Função K:Ajuda com as instru¸c˜oes de como jogar
void mostrar_ajuda() {
    printf(BOLD(CYAN("\n=== Instruções do Jogo 2048 ===\n")));
    printf(BOLD(WHITE("Comandos durante o jogo:\n")));
    printf("  a - mover as peças para a esquerda\n");
    printf("  d - mover as peças para a direita\n");
    printf("  w - mover as peças para cima\n");
    printf("  s - mover as peças para baixo\n");
    printf("  u - desfazer o último movimento\n");
    printf("  t pos1 pos2 - trocar as peças nas posições pos1 e pos2\n");
    printf("  voltar - retornar ao menu inicial\n\n");
    printf(BOLD(WHITE("No menu inicial, use as opções:\n")));
    printf("  (R) Sair - sair do jogo\n");
    printf("  (N) Novo jogo - iniciar um novo jogo\n");
    printf("  (J) Continuar - continuar o jogo atual\n");
    printf("  (C) Carregar - carregar um jogo salvo\n");
    printf("  (S) Salvar - salvar o jogo atual\n");
    printf("  (M) Mostrar Ranking - exibir as 10 melhores pontuações\n");
    printf("  (A) Ajuda - mostrar estas instruções\n\n");
    printf(BOLD(WHITE("Pressione ENTER para voltar ao menu...\n")));
    getchar();
}*\