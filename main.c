// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "struct.h"
#include "exemplo.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int main() {
    srand(time(NULL));

    Jogo jogo;
    jogo.tamanho_tab = 4;
    jogo.pontos = 0;
    jogo.matriz_tab = criar_matriz(jogo.tamanho_tab);

    // Inicializa duas peças aleatórias
    n_aleatorio(&jogo);
    n_aleatorio(&jogo);

    char comando;
    int jogo_continua = 1;

    while(jogo_continua) {
        system("clear");
        imprimir_matriz(&jogo);
        printf("\nPontos: %d\n", jogo.pontos);
        printf("Digite w/a/s/d para mover ou q para sair: ");
        scanf(" %c", &comando);
        comando = tolower(comando);

        if(comando == 'q') break;

        int houve_movimento = mover(&jogo, comando);
        if(houve_movimento) {
            n_aleatorio(&jogo); // adiciona nova peça
        }

        int estado = verificar_jogo(&jogo); // 1 se ainda tem movimento, 0 se não
        if(estado == 0) {
            system("clear");
            imprimir_matriz(&jogo);
            printf("\nFim de jogo! Nenhum movimento possível.\n");
            break;
        }
    }

    liberar_matriz(jogo.matriz_tab, jogo.tamanho_tab);
    printf("Obrigado por jogar!\n");
    return 0;
}

