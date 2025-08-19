// Ana Gabriela Gomes Lopes Pereira - 25.1.4119
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "struct.h"
#include "funcoes.h"

#define TAM 4  // tamanho do tabuleiro 4x4

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    void menu(void);
    // Criação do Jogo de teste
    Jogo jogo_teste;
    jogo_teste.tamanho_tab = TAM;
    jogo_teste.pontos = 0;
    jogo_teste.matriz_tab = criar_matriz(TAM);

    // Preenchendo o tabuleiro com algumas peças iniciais
    jogo_teste.matriz_tab[0][0] = 2;
    jogo_teste.matriz_tab[0][1] = 2;
    jogo_teste.matriz_tab[0][2] = 2;
    jogo_teste.matriz_tab[0][3] = 2;

    printf("Tabuleiro inicial:\n");
    imprimir_matriz(&jogo_teste);

    char direcao;
    printf("\nDigite a direção do movimento (w/a/s/d): ");
    scanf(" %c", &direcao);

    // Executa o movimento não-guloso
    if (movimento_nao_guloso(&jogo_teste, direcao)) {
        printf("\nTabuleiro após o movimento '%c':\n", direcao);
        imprimir_matriz(&jogo_teste);
    } else {
        printf("\nMovimento inválido ou nenhuma peça se moveu.\n");
    }

    // Libera memória do tabuleiro
    liberar_matriz(jogo_teste.matriz_tab, TAM);

    return 0;
}
