// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "funcoes.h"
#include "exemplo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ==================== FUNÇÕES BÁSICAS ====================
// Função 1: Centraliza horizontalmente um texto na tela
void centralizar(char *texto) {
    int largura_terminal = 80; 
    int comprimento = strlen(texto);
    int espacos = (largura_terminal - comprimento) / 2;
    if (espacos < 0) espacos = 0;
    for (int i = 0; i < espacos; i++)
        printf(" ");
    printf("%s\n", texto);
}

// Função 2: Tela inicial do jogo, centralizada horizontal e verticalmente
void telaInicial() {
    system("clear"); 
    int largura_terminal = 100;
    int altura_terminal = 24; 
    char *texto[] = {
        "============================================",
        "        BEM-VINDO AO JOGO        ",
        "             2048               ",
        "Criado por: Ana Gabriela Gomes      ",
        "Matrícula: 25.1.4119           ",
        "          Puca lover <3!             ",
        "\"Professor, espero que se divirta!\"",
        "============================================",
        "Pressione ENTER para iniciar o jogo..."
    };
    int linhas_texto = sizeof(texto) / sizeof(texto[0]);
    int margem_superior = (altura_terminal - linhas_texto) / 2;
    if (margem_superior < 0) margem_superior = 0;
    for (int i = 0; i < margem_superior; i++) printf("\n"); // espaço vertical
    for (int i = 0; i < linhas_texto; i++) {
        int espacos = (largura_terminal - strlen(texto[i])) / 2;
        if (espacos < 0) espacos = 0;
        for (int j = 0; j < espacos; j++) printf(" ");
        printf("%s\n", texto[i]);
    }
    limpar_buffer();
}
// Função 3: Limpa o buffer do teclado
void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ==================== MATRIZ ====================
// Função 4: Cria uma matriz NxN usando alocação dinâmica
int **criar_matriz(int n) {
    int **matriz = malloc(n * sizeof(int *));
    if (!matriz) return NULL;
    for (int i = 0; i < n; i++) {
        matriz[i] = malloc(n * sizeof(int));
        if (!matriz[i]) {
            for (int k = 0; k < i; k++) free(matriz[k]);
            free(matriz);
            return NULL;
        }
        for (int j = 0; j < n; j++) matriz[i][j] = 0;
    }
    return matriz;
}

// Função 5: Imprime a matriz colorida do jogo
void imprimir_matriz(Jogo *jogo) {
    int n = jogo->tamanho_tab;
    char cel[8];

    printf("Jogador: %s | Pontos: %d\n", jogo->nome, jogo->pontos);
    printf("Chances de trocar: %d | Desfazer: %d\n", jogo->trocas_restantes, jogo->desfazer_restantes);

    // Cabeçalho das colunas
    printf("   ");
    for (int k = 0; k < n; k++) printf(" %d  ", k + 1);
    printf("\n");

    // Imprime linhas
    for (int i = 0; i < n; i++) {
        printf("%c ", 'A' + i);
        for (int j = 0; j < n; j++) {
            int v = jogo->matriz_tab[i][j];
            sprintf(cel, v == 0 ? "   " : "%2d ", v);
            printf("%s", cel);
        }
        printf("\n");
    }
}

// Função 6: Libera memória da matriz
void liberar_matriz(int **matriz, int n) {
    if (!matriz) return;
    for (int i = 0; i < n; i++) free(matriz[i]);
    free(matriz);
}

// Função 7: Copia uma matriz NxN para outra
int **copiar_matriz(int **matriz, int tamanho) {
    if (!matriz || tamanho <= 0) return NULL;
    int **nova = malloc(tamanho * sizeof(int *));
    if (!nova) return NULL;
    for (int i = 0; i < tamanho; i++) {
        nova[i] = malloc(tamanho * sizeof(int));
        if (!nova[i]) {
            for (int j = 0; j < i; j++) free(nova[j]);
            free(nova);
            return NULL;
        }
        for (int j = 0; j < tamanho; j++) nova[i][j] = matriz[i][j];
    }
    return nova;
}

// Função 8: Insere números aleatórios no tabuleiro
void n_aleatorio(Jogo *jogo_atual) {
    int n = jogo_atual->tamanho_tab;
    int pecas = (n == 6) ? 2 : 1;

    for (int p = 0; p < pecas; p++) {
        int linha, coluna, valor;
        int prob = rand() % 100;

        if (n == 4) valor = (prob < 90) ? 2 : 4;
        else if (n == 5) valor = (prob < 85) ? 2 : 4;
        else valor = (prob < 80) ? 2 : 4;

        do {
            linha = rand() % n;
            coluna = rand() % n;
        } while (jogo_atual->matriz_tab[linha][coluna] != 0);

        jogo_atual->matriz_tab[linha][coluna] = valor;
    }
}

// ==================== MOVIMENTAÇÃO ====================

// Função 9: Processa linha ou coluna para empurrar e combinar números iguais
void movimento_nao_guloso(int *linha, int tamanho, int *pontuacao, int *houve_movimento) {
    int original[tamanho], combinada[tamanho];
    for (int i = 0; i < tamanho; i++) {
        original[i] = linha[i];
        combinada[i] = 0;
    }

    // Empurra números para frente
    for (int i = 0; i < tamanho; i++) {
        if (linha[i] == 0) continue;
        int k = i;
        while (k > 0 && linha[k-1] == 0) {
            linha[k-1] = linha[k];
            linha[k] = 0;
            k--;
        }
    }

    // Combina números iguais
    for (int i = 0; i < tamanho - 1; i++) {
        if (linha[i] != 0 && linha[i] == linha[i+1] && !combinada[i] && !combinada[i+1]) {
            linha[i] *= 2;
            linha[i+1] = 0;
            combinada[i] = 1;
            *pontuacao += linha[i];
        }
    }

    // Empurra novamente após combinações
    for (int i = 0; i < tamanho; i++) {
        if (linha[i] == 0) continue;
        int k = i;
        while (k > 0 && linha[k-1] == 0) {
            linha[k-1] = linha[k];
            linha[k] = 0;
            k--;
        }
    }

    *houve_movimento = 0;
    for (int i = 0; i < tamanho; i++) {
        if (linha[i] != original[i]) {
            *houve_movimento = 1;
            break;
        }
    }
}

// Funções de movimentação: esquerda, direita, cima e baixo
int mover_esquerda(Jogo *jogo){
    int n = jogo->tamanho_tab;
    int movimento = 0;

    for(int i = 0; i < n; i++){
        int houve_movimento = 0;
        int linha[n];
        for(int j = 0; j < n; j++) linha[j] = jogo->matriz_tab[i][j];

        movimento_nao_guloso(linha, n, &(jogo->pontos), &houve_movimento);

        for(int j = 0; j < n; j++) jogo->matriz_tab[i][j] = linha[j];
        if(houve_movimento) movimento = 1;
    }
    return movimento;
}

int mover_direita(Jogo *jogo){
    int n = jogo->tamanho_tab;
    int movimento = 0;

    for(int i = 0; i < n; i++){
        int houve_movimento = 0;
        int linha[n];
        for(int j = 0; j < n; j++) linha[j] = jogo->matriz_tab[i][n-1-j];

        movimento_nao_guloso(linha, n, &(jogo->pontos), &houve_movimento);

        for(int j = 0; j < n; j++) jogo->matriz_tab[i][n-1-j] = linha[j];
        if(houve_movimento) movimento = 1;
    }
    return movimento;
}

int mover_cima(Jogo *jogo){
    int n = jogo->tamanho_tab;
    int movimento = 0;

    for(int j = 0; j < n; j++){
        int houve_movimento = 0;
        int coluna[n];
        for(int i = 0; i < n; i++) coluna[i] = jogo->matriz_tab[i][j];

        movimento_nao_guloso(coluna, n, &(jogo->pontos), &houve_movimento);

        for(int i = 0; i < n; i++) jogo->matriz_tab[i][j] = coluna[i];
        if(houve_movimento) movimento = 1;
    }
    return movimento;
}

int mover_baixo(Jogo *jogo){
    int n = jogo->tamanho_tab;
    int movimento = 0;

    for(int j = 0; j < n; j++){
        int houve_movimento = 0;
        int coluna[n];
        for(int i = 0; i < n; i++) coluna[i] = jogo->matriz_tab[n-1-i][j];

        movimento_nao_guloso(coluna, n, &(jogo->pontos), &houve_movimento);

        for(int i = 0; i < n; i++) jogo->matriz_tab[n-1-i][j] = coluna[i];
        if(houve_movimento) movimento = 1;
    }
    return movimento;
}

// ==================== DESFAZER / TROCAR ====================

void desfazer_movimento(Jogo *jogo) {
    if (jogo->desfazer_restantes <= 0) {
        printf("Sem chances de desfazer!\n");
        return;
    }
    if (!jogo->tab_anterior) return;

    for (int i = 0; i < jogo->tamanho_tab; i++)
        for (int j = 0; j < jogo->tamanho_tab; j++)
            jogo->matriz_tab[i][j] = jogo->tab_anterior[i][j];

    jogo->pontos = jogo->pontos_anterior;
    jogo->desfazer_restantes--;
    printf("Movimento desfeito. Restam %d desfazer(s)\n", jogo->desfazer_restantes);
}

void trocar_pecas(Jogo *jogo) {
    if (jogo->trocas_restantes <= 0) {
        printf("Sem chances de trocar peças!\n");
        return;
    }

    char pos1[3], pos2[3];
    int l1, c1, l2, c2;

    printf("Digite a primeira posição (Ex: A1): ");
    scanf("%2s", pos1);
    printf("Digite a segunda posição (Ex: B3): ");
    scanf("%2s", pos2);

    l1 = toupper(pos1[0]) - 'A';
    c1 = pos1[1] - '1';
    l2 = toupper(pos2[0]) - 'A';
    c2 = pos2[1] - '1';

    if (l1 < 0 || l1 >= jogo->tamanho_tab || l2 < 0 || l2 >= jogo->tamanho_tab ||
        c1 < 0 || c1 >= jogo->tamanho_tab || c2 < 0 || c2 >= jogo->tamanho_tab) {
        printf("Posições inválidas!\n");
        return;
    }

    int temp = jogo->matriz_tab[l1][c1];
    jogo->matriz_tab[l1][c1] = jogo->matriz_tab[l2][c2];
    jogo->matriz_tab[l2][c2] = temp;

    jogo->trocas_restantes--;
    printf("Peças trocadas. Restam %d trocas\n", jogo->trocas_restantes);
}

// ==================== VERIFICAÇÕES ====================

int verificar_jogo(Jogo *jogo) {
    int n = jogo->tamanho_tab;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (jogo->matriz_tab[i][j] == 0) return 1;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i < n-1 && jogo->matriz_tab[i][j] == jogo->matriz_tab[i+1][j]) return 1;
            if (j < n-1 && jogo->matriz_tab[i][j] == jogo->matriz_tab[i][j+1]) return 1;
        }
    return 0;
}

int verificar_vitoria(Jogo *jogo) {
    int n = jogo->tamanho_tab;
    char opcao;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (jogo->matriz_tab[i][j] == 2048) {
                printf("\nParabéns! Você atingiu 2048!\n");
                do {
                    printf("Deseja continuar jogando? (s/n): ");
                    scanf(" %c", &opcao);
                    limpar_buffer();
                    opcao = tolower(opcao);
                } while (opcao != 's' && opcao != 'n');
                return (opcao == 's') ? 1 : 0;
            }
    return 2;
}

int verificar_derrota(Jogo *jogo) {
    if (verificar_jogo(jogo)) return 1;

    printf("\nGame Over! Não existem mais movimentos possíveis.\n");
    if (jogo->desfazer_restantes > 0) {
        char opcao;
        do {
            printf("Deseja usar DESFAZER? (s/n): ");
            scanf(" %c", &opcao);
            limpar_buffer();
            opcao = tolower(opcao);
        } while (opcao != 's' && opcao != 'n');

        if (opcao == 's') {
            jogo->desfazer_restantes--;
            return 2;
        }
    }
    printf("Voltando ao menu principal...\n");
    return 0;
}
