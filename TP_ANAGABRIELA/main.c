// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h"
#include "menu.h"
#include "salva_jogo.h"
#include "ranking.h"

int main(void) {
    srand(time(NULL)); // Inicia o gerador de números aleatórios
    
    telaInicial();// Mostra tela inicial
    menu();// Inicia o menu principal

    return 0;
}