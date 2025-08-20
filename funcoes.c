// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "struct.h"
#include "exemplo.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//FUNÇÕES BÁSICAS 
// Função 1: Centraliza qualquer texto dentro de uma largura definida
void centralizar(char *texto){
    int largura_terminal = 100;
    int comprimento = strlen(texto);
    int espacos = (largura_terminal - comprimento) / 2;

    if (espacos < 0) espacos = 0;

    for (int i = 0;i < espacos; i++){
        printf(" ");
    }
    printf("%s\n", texto); // <--- imprime o texto
}

// Função 2:Tela inicial         
void telaInicial() {
    system("clear"); //limpa a tela  
    
    centralizar(BOLD(YELLOW("BEM-VINDO AO JOGO")));
    centralizar(BOLD(RED("2048")));
    centralizar(GREEN("Criado por: Ana Gabriela Gomes"));
    centralizar(GREEN("Matrícula: 25.1.4119"));
    centralizar(MAGENTA("Puca lover <3!"));
    centralizar(BOLD(CYAN("\"Professor, espero que se divirta!\"")));
    printf("\n");
    centralizar(BOLD(WHITE("Pressione ENTER para iniciar o jogo...")));
    
    getchar(); // espera o enter
}

// Função 3: Criar matriz
int **criar_matriz(int n) {
    int **matriz = malloc(n * sizeof(int*));
    if (matriz == 0) 
        return NULL;

    for (int i = 0; i < n; i++) {
        matriz[i] = malloc(n * sizeof(int));
        if (matriz[i] == 0) {
            for (int k = 0; k < i; k++){
                free(matriz[k]);
            }
            free(matriz);
            return NULL;
        }
        for (int j = 0; j < n; j++) 
            matriz[i][j] = 0;
    }
    return matriz;
}

// Função 4: Imprimir matriz
void imprimir_matriz(Jogo *j) {
    int n = j->tamanho_tab;   
    char cel[8];

    printf("   ");
    for (int k = 0; k < n; k++) 
        printf(" %d  ", k + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        if (i == 0) 
            printf(TAB_TL);
        else 
            printf(TAB_ML);

        for (int k = 0; k < n; k++) {
            printf(TAB_HOR TAB_HOR TAB_HOR);
            if (k < n - 1) 
                printf(TAB_MJ);
            else if (i == 0) 
                printf(TAB_TR);
            else if (i == n - 1) 
                printf(TAB_BR);
            else 
                printf(TAB_MR);
        }
        printf("\n");

        printf("%c ", 'A' + i);

        for (int k = 0; k < n; k++) {
            int v = j->matriz_tab[i][k];

            if (v == 0) 
                sprintf(cel, "   ");
            else 
                sprintf(cel, "%2d ", v);

            printf(TAB_VER);

            if (v == 2) 
                printf("%s%s%s", ANSI_COLOR_RED, cel, ANSI_RESET);
            else if (v == 4) 
                printf("%s%s%s", ANSI_COLOR_GREEN, cel, ANSI_RESET);
            else if (v == 8) 
                printf("%s%s%s", ANSI_COLOR_YELLOW, cel, ANSI_RESET);
            else if (v == 16) 
                printf("%s%s%s", ANSI_COLOR_BLUE, cel, ANSI_RESET);
            else if (v == 32) 
                printf("%s%s%s", ANSI_COLOR_MAGENTA, cel, ANSI_RESET);
            else if (v == 64) 
                printf("%s%s%s", ANSI_COLOR_CYAN, cel, ANSI_RESET);
            else 
                printf("%s", cel);
        }
        printf(TAB_VER "\n");

        if (i == n - 1) {
            printf(TAB_BL);
            for (int k = 0; k < n; k++) {
                printf(TAB_HOR TAB_HOR TAB_HOR);
                if (k < n - 1) 
                    printf(TAB_BJ);
                else 
                    printf(TAB_BR);
            }
            printf("\n");
        }
    }
}

// Função 5: Liberar matriz
void liberar_matriz(int **matriz, int n) {
    for (int i = 0; i < n; i++) 
        free(matriz[i]);

    free(matriz);
}

//Função 6:Coloca o numero aleatorio
void n_aleatorio(Jogo *jogo_atual) {
    int n = jogo_atual->tamanho_tab;
    int pecas;

    if (n == 6) pecas = 2;
    else pecas = 1;

    for (int p = 0; p < pecas; p++) {
        int linha, coluna;
        int valor;
        int probabilidade = rand() % 100;

        if (n == 4) {
            if (probabilidade < 90) 
                valor = 2;
            else valor = 4;
        } else if (n == 5) {
            if (probabilidade < 85) 
                valor = 2;
            else valor = 4;
        } else {
            if (probabilidade < 80) valor = 2;
            else valor = 4;
        }

        do {
            linha = rand() % n;
            coluna = rand() % n;
        } while (jogo_atual->matriz_tab[linha][coluna] != 0);

        jogo_atual->matriz_tab[linha][coluna] = valor;
    }
}

// Função 7: Copiar matriz
int **copiar_matriz(int **matriz, int tamanho) {
    int **copia = criar_matriz(tamanho);
    for (int i = 0; i < tamanho; i++){
        for (int j = 0; j < tamanho; j++){
            copia[i][j] = matriz[i][j];
        }
    }
    return copia;
}

//MOVIMENTO E PONTUAÇÃO
int mover(Jogo *jogo,char direcao){
    switch(direcao){
        case 'a': return mover_esquerda(jogo);
        case 'd': return mover_direita(jogo);
        case 'w': return mover_cima(jogo);
        case 's': return mover_baixo(jogo);
        default: return 0;
    }
}

int mover_direita(Jogo *jogo){
    int n = jogo->tamanho_tab;
    int movimento = 0;

    for(int i = 0; i < n; i++){
        int houve_movimento = 0;
        int linha[n];

        for(int j = 0; j < n; j++)
            linha[j] = jogo->matriz_tab[i][n-1-j];

        movimento_nao_guloso(linha, n, &(jogo->pontos), &houve_movimento);

        for(int j = 0; j < n; j++)
            jogo->matriz_tab[i][n-1-j] = linha[j];

        if(houve_movimento) movimento = 1;
    }

    return movimento;
}

int mover_esquerda(Jogo *jogo){
    int n = jogo->tamanho_tab;
    int movimento = 0;

    for(int i = 0; i < n; i++){
        int houve_movimento = 0;
        int linha[n];

        for(int j = 0; j < n; j++)
            linha[j] = jogo->matriz_tab[i][j];

        movimento_nao_guloso(linha, n, &(jogo->pontos), &houve_movimento);

        for(int j = 0; j < n; j++)
            jogo->matriz_tab[i][j] = linha[j];

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

        for(int i = 0; i < n; i++)
            coluna[i] = jogo->matriz_tab[i][j];

        movimento_nao_guloso(coluna, n, &(jogo->pontos), &houve_movimento);

        for(int i = 0; i < n; i++)
            jogo->matriz_tab[i][j] = coluna[i];

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

        for(int i = 0; i < n; i++)
            coluna[i] = jogo->matriz_tab[n-1-i][j];

        movimento_nao_guloso(coluna, n, &(jogo->pontos), &houve_movimento);

        for(int i = 0; i < n; i++)
            jogo->matriz_tab[n-1-i][j] = coluna[i];

        if(houve_movimento) movimento = 1;
    }

    return movimento;
}

// Função 13:Adicionar pontos
void adicionar_pontos(Jogo *jogo, int pontos) {
    jogo->pontos += pontos;
}

//Função 14:Movimento não guloso
void movimento_nao_guloso(int *linha, int tamanho, int *pontuacao, int *houve_movimento) {
    int temporario[tamanho];
    int combinada[tamanho];
    for (int i = 0; i < tamanho; i++) {
        temporario[i] = linha[i];
        combinada[i] = 0;
    }

    for (int i = 0; i < tamanho; i++) {
        if (linha[i] == 0) continue;
        int k = i;
        while (k > 0 && linha[k-1] == 0) {
            linha[k-1] = linha[k];
            linha[k] = 0;
            k--;
        }
    }

    for (int i = 0; i < tamanho - 1; i++) {
        if (linha[i] != 0 && linha[i] == linha[i+1] && !combinada[i] && !combinada[i+1]) {
            linha[i] *= 2;
            linha[i+1] = 0;
            combinada[i] = 1;
            *pontuacao += linha[i];
        }
    }

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
        if (linha[i] != temporario[i]) {
            *houve_movimento = 1;
            break;
        }
    }
}

// Função 15: Verificar movimento, vitória ou derrota
int verificar_jogo(Jogo *jogo){
    int n = jogo->tamanho_tab;
    int movimento_possivel = 0;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(jogo->matriz_tab[i][j] == 0){
                movimento_possivel = 1; // tem espaço vazio
            }

            // verifica se pode combinar com o de baixo
            if(i < n-1 && jogo->matriz_tab[i][j] == jogo->matriz_tab[i+1][j]){
                movimento_possivel = 1;
            }

            // verifica se pode combinar com o da direita
            if(j < n-1 && jogo->matriz_tab[i][j] == jogo->matriz_tab[i][j+1]){
                movimento_possivel = 1;
            }
        }
    }

    return movimento_possivel; // 1 = ainda dá pra jogar, 0 = acabou
}