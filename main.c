// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "menu.h"
#include "funcoes.h"
#include "struct.h"
#include "salva_jogo.h"
#include "ranking.h"
#include "exemplo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int main() {
    srand(time(NULL)); // inicializa números aleatórios

    // 1. Tela inicial
    telaInicial();

    // 2. Configurações do jogo
    int tamanho = 4;
    Jogo jogo;
    strcpy(jogo.nome, "Ana Gabriela");
    jogo.pontos = 0;
    jogo.trocar_pecas = 0;
    jogo.desfazer = 0;
    jogo.tamanho_tab = tamanho;

    // 3. Cria a matriz do tabuleiro
    jogo.matriz_tab = criar_matriz(tamanho);
    if (!jogo.matriz_tab) {
        printf("Erro ao alocar matriz!\n");
        return 1;
    }

    // 4. Adiciona duas peças aleatórias iniciais
    n_aleatorio(&jogo);
    n_aleatorio(&jogo);

    // 5. Imprime o tabuleiro colorido
    imprimir_matriz(&jogo);

    // 6. Libera memória
    liberar_matriz(jogo.matriz_tab, tamanho);

    return 0;
}