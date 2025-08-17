// Ana Gabriela Gomes Lopes Pereira - 25.1.4119
// Arquivo com as implementações das funções utilizadas no jogo

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
void centralizar(const char* texto) {
    int largura_terminal = 100;
    int comprimento = (int)strlen(texto); //calcula o tamanho do texto
    int espacos = (largura_terminal - comprimento) / 2;

    if (espacos < 0){ //caso for maior que o tamanho do terminal,ele não vai imprimrir 
        espacos = 0;
    }

    for (int i = 0; i < espacos; i++){ //impressão dos espaços
         printf(" ");
    }

    printf("%s\n", texto); //impressão do texto e \n
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
int** criar_matriz(int n) {
    int **matriz = malloc(n * sizeof(int *)); // Aloca uma matriz de n linhas
    if (!matriz) 
        return NULL; // Verifica se a alocação falhou

    for (int i = 0; i < n; i++) {
        matriz[i] = malloc(n * sizeof(int)); // Aloca cada linha com n inteiros
        if (!matriz[i]) { // Se falhar, libera o que já tinha sido alocado
            for (int k = 0; k < i; k++) free(matriz[k]);
            free(matriz);
            return NULL;
        }
        for (int j = 0; j < n; j++) 
            matriz[i][j] = 0; // Inicializa com 0
    }
    return matriz; // Retorna a matriz criada
}


// Função 4: Imprimir matriz
void imprimir_matriz(Jogo *j) {
    int n = j->tamanho_tab;   
    char cel[8];// armazenar a string de cada célula

    // Imprime o cabeçalho das colunas com números
    printf("   ");
    for (int k = 0; k < n; k++) 
        printf(" %d  ", k + 1);
    printf("\n");

    // Percorre todas as linhas do tabuleiro
    for (int i = 0; i < n; i++) {
        // Desenha a linha horizontal superior ou do meio, dependendo da linha
        if (i == 0) 
            printf(TAB_TL);   // Canto superior esquerdo
        else 
            printf(TAB_ML);   // Linha do meio à esquerda

        // Desenha as divisões horizontais das células
        for (int k = 0; k < n; k++) {
            printf(TAB_HOR TAB_HOR TAB_HOR);  // Linha horizontal da célula
            if (k < n - 1) 
                printf(TAB_MJ);   // Junção entre células
            else if (i == 0) 
                printf(TAB_TR);   // Canto superior direito
            else if (i == n - 1) 
                printf(TAB_BR);   // Canto inferior direito (será refeito depois)
            else 
                printf(TAB_MR);   // Linha do meio à direita
        }
        printf("\n");

        // Imprime a letra da linha (A, B, C, ...)
        printf("%c ", 'A' + i);

        // Percorre todas as colunas da linha atual
        for (int k = 0; k < n; k++) {
            int v = j->matriz_tab[i][k];  // Valor da célula atual

            // Formata o valor da célula
            if (v == 0) 
                sprintf(cel, "   ");      // Espaço vazio
            else 
                sprintf(cel, "%2d ", v);  // Número preenchido

            printf(TAB_VER);  // Desenha a borda vertical da célula

            // Aplica cores diferentes de acordo com o valor da célula
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
                printf("%s", cel);  // Sem cor para valores maiores
        }
        printf(TAB_VER "\n");  // Fecha a linha com a borda direita

        // Se for a última linha, desenha a linha inferior do tabuleiro
        if (i == n - 1) {
            printf(TAB_BL);  // Canto inferior esquerdo
            for (int k = 0; k < n; k++) {
                printf(TAB_HOR TAB_HOR TAB_HOR);  // Linha horizontal
                if (k < n - 1) 
                    printf(TAB_BJ);   // Junção inferior entre células
                else 
                    printf(TAB_BR);   // Canto inferior direito
            }
            printf("\n");
        }
    }
}

// Função 5: Liberar matriz
void liberar_matriz(int **matriz, int n) {
    // Percorre cada linha da matriz e libera a memória alocada
    for (int i = 0; i < n; i++) 
        free(matriz[i]);  // Libera cada linha

    free(matriz);// Libera o ponteiro da matriz em si
}

// Função 6: Colocar números aleatórios

// Função 7: Copiar matriz
int** copiar_matriz(int **matriz, int tamanho) {
    int **copia = criar_matriz(tamanho);

    // Copia cada elemento da matriz original para a nova matriz
    for (int i = 0; i < tamanho; i++)
        for (int j = 0; j < tamanho; j++)
            copia[i][j] = matriz[i][j];

    // Retorna a matriz copiada
    return copia;
}

//MOVIMENTO E PONTUAÇÃO
// Função 8:Movimenta o tabuleiro 
int mover(Jogo *jogo, char direcao) {
    int **tab = jogo->matriz_tab; 
    int n = jogo->tamanho_tab;    
    int movimento = 0; // indica se houve movimento

    if (direcao == 'a') { // mover para a esquerda
        for (int i = 0; i < n; i++) {       
            for (int j = 1; j < n; j++) {   // percorre cada coluna a partir da segunda
                if (tab[i][j] != 0) {       
                    int k = j;
                    // move a peça enquanto a célula à esquerda estiver vazia
                    while (k > 0 && tab[i][k-1] == 0) {
                        tab[i][k-1] = tab[i][k];
                        tab[i][k] = 0;
                        k--;
                        movimento = 1; // marcou que houve movimento
                    }
                    // combina com a peça à esquerda se tiver o mesmo valor
                    if (k > 0 && tab[i][k-1] == tab[i][k]) {
                        tab[i][k-1] *= 2; // dobra o valor
                        tab[i][k] = 0;    // limpa a célula original
                        adicionar_pontos(jogo, tab[i][k-1]); // soma pontos
                        movimento = 1;
                    }
                }
            }
        }
    } 
    else if (direcao == 'd') { // mover para a direita
        for (int i = 0; i < n; i++) {        
            for (int j = n-2; j >= 0; j--) {  // percorre da penúltima coluna para a esquerda
                if (tab[i][j] != 0) {
                    int k = j;
                    // move a peça enquanto a célula à direita estiver vazia
                    while (k < n-1 && tab[i][k+1] == 0) {
                        tab[i][k+1] = tab[i][k];
                        tab[i][k] = 0;
                        k++;
                        movimento = 1;
                    }
                    // combina com a peça à direita se tiver o mesmo valor
                    if (k < n-1 && tab[i][k+1] == tab[i][k]) {
                        tab[i][k+1] *= 2;
                        tab[i][k] = 0;
                        adicionar_pontos(jogo, tab[i][k+1]);
                        movimento = 1;
                    }
                }
            }
        }
    } 
    else if (direcao == 'w') { // mover para cima
        for (int j = 0; j < n; j++) {          
            for (int i = 1; i < n; i++) { // percorre cada linha a partir da segunda
                if (tab[i][j] != 0) {
                    int k = i;
                    // move a peça enquanto a célula acima estiver vazia
                    while (k > 0 && tab[k-1][j] == 0) {
                        tab[k-1][j] = tab[k][j];
                        tab[k][j] = 0;
                        k--;
                        movimento = 1;
                    }
                    // combina com a peça acima se tiver o mesmo valor
                    if (k > 0 && tab[k-1][j] == tab[k][j]) {
                        tab[k-1][j] *= 2;
                        tab[k][j] = 0;
                        adicionar_pontos(jogo, tab[k-1][j]);
                        movimento = 1;
                    }
                }
            }
        }
    } 
    else if (direcao == 's') { // mover para baixo
        for (int j = 0; j < n; j++) {
            for (int i = n-2; i >= 0; i--) {  // percorre da penúltima linha para cima
                if (tab[i][j] != 0) {
                    int k = i;
                    // move a peça enquanto a célula abaixo estiver vazia
                    while (k < n-1 && tab[k+1][j] == 0) {
                        tab[k+1][j] = tab[k][j];
                        tab[k][j] = 0;
                        k++;
                        movimento = 1;
                    }
                    // combina com a peça abaixo se tiver o mesmo valor
                    if (k < n-1 && tab[k+1][j] == tab[k][j]) {
                        tab[k+1][j] *= 2;
                        tab[k][j] = 0;
                        adicionar_pontos(jogo, tab[k+1][j]);
                        movimento = 1;
                    }
                }
            }
        }
    }

    return movimento; // retorna se houve ou não movimento
}

// Função 9: Adicionar nova peça aleatória
void nova_peca(Jogo *jogo) {
    int vazios[36][2], qt_vazios = 0;
    for (int i = 0; i < jogo->tamanho_tab; i++) {
        for (int j = 0; j < jogo->tamanho_tab; j++) {
            if (jogo->matriz_tab[i][j] == 0) {
                vazios[qt_vazios][0] = i;
                vazios[qt_vazios][1] = j;
                qt_vazios++;
            }
        }
    }
    if (qt_vazios == 0) return;

    int pos = rand() % qt_vazios;
    int valor = (rand() % 100 < 90) ? 2 : 4;
    jogo->matriz_tab[vazios[pos][0]][vazios[pos][1]] = valor;
}

//Função 10:Atualiza a pontuação somando o valor 
void adicionar_pontos(Jogo *jogo, int valor) {
    jogo->pontos += valor;
}

//Função 11: Movimento “não-guloso”
int movimento_nao_guloso(Jogo *jogo, char direcao) {
    int **matriz = jogo->matriz_tab;  
    int tamanho = jogo->tamanho_tab;  
    int movimento_valido = 0;        

    // Matriz auxiliar para marcar peças já combinadas nesta jogada
    int combinada[tamanho][tamanho];
    for (int i = 0; i < tamanho; i++)
        for (int j = 0; j < tamanho; j++)
            combinada[i][j] = 0;  // nenhuma peça foi combinada ainda

    // Movimentar para a esquerda
    if (direcao == 'a') {
        for (int i = 0; i < tamanho; i++) {
            for (int j = 1; j < tamanho; j++) { // percorre da 2ª até a última coluna
                if (matriz[i][j] != 0) {        // se a célula não estiver vazia
                    int k = j;

                    // Move a peça enquanto houver espaço vazio à esquerda
                    while (k > 0 && matriz[i][k-1] == 0) {
                        matriz[i][k-1] = matriz[i][k];  // move a peça
                        matriz[i][k] = 0;               // limpa posição antiga
                        k--;
                        movimento_valido = 1;           // movimento aconteceu
                    }

                    // Combina peças iguais não combinadas ainda
                    if (k > 0 && matriz[i][k-1] == matriz[i][k] &&
                        !combinada[i][k-1] && !combinada[i][k]) {
                        matriz[i][k-1] *= 2;         // combina
                        matriz[i][k] = 0;            // limpa posição antiga
                        combinada[i][k-1] = 1;       // marca como combinada
                        movimento_valido = 1;        // movimento/combinação ocorreu
                    }
                }
            }
        }
    }

    // Movimentar para a direita
    else if (direcao == 'd') {
        for (int i = 0; i < tamanho; i++) {
            for (int j = tamanho-2; j >= 0; j--) { // percorre da penúltima coluna até a primeira
                if (matriz[i][j] != 0) {
                    int k = j;

                    // Move a peça enquanto houver espaço à direita
                    while (k < tamanho-1 && matriz[i][k+1] == 0) {
                        matriz[i][k+1] = matriz[i][k];
                        matriz[i][k] = 0;
                        k++;
                        movimento_valido = 1;
                    }

                    // Combina peças iguais não combinadas
                    if (k < tamanho-1 && matriz[i][k+1] == matriz[i][k] &&
                        !combinada[i][k+1] && !combinada[i][k]) {
                        matriz[i][k+1] *= 2;
                        matriz[i][k] = 0;
                        combinada[i][k+1] = 1;
                        movimento_valido = 1;
                    }
                }
            }
        }
    }

    // Movimentar para cima
    else if (direcao == 'w') {
        for (int j = 0; j < tamanho; j++) {
            for (int i = 1; i < tamanho; i++) { // percorre linhas de cima para baixo
                if (matriz[i][j] != 0) {
                    int k = i;

                    // Move a peça enquanto houver espaço acima
                    while (k > 0 && matriz[k-1][j] == 0) {
                        matriz[k-1][j] = matriz[k][j];
                        matriz[k][j] = 0;
                        k--;
                        movimento_valido = 1;
                    }

                    // Combina peças iguais não combinadas
                    if (k > 0 && matriz[k-1][j] == matriz[k][j] &&
                        !combinada[k-1][j] && !combinada[k][j]) {
                        matriz[k-1][j] *= 2;
                        matriz[k][j] = 0;
                        combinada[k-1][j] = 1;
                        movimento_valido = 1;
                    }
                }
            }
        }
    }

    // Movimentar para baixo
    else if (direcao == 's') {
        for (int j = 0; j < tamanho; j++) {
            for (int i = tamanho-2; i >= 0; i--) { // percorre da penúltima linha até a primeira
                if (matriz[i][j] != 0) {
                    int k = i;

                    // Move a peça enquanto houver espaço abaixo
                    while (k < tamanho-1 && matriz[k+1][j] == 0) {
                        matriz[k+1][j] = matriz[k][j];
                        matriz[k][j] = 0;
                        k++;
                        movimento_valido = 1;
                    }

                    // Combina peças iguais não combinadas
                    if (k < tamanho-1 && matriz[k+1][j] == matriz[k][j] &&
                        !combinada[k+1][j] && !combinada[k][j]) {
                        matriz[k+1][j] *= 2;
                        matriz[k][j] = 0;
                        combinada[k+1][j] = 1;
                        movimento_valido = 1;
                    }
                }
            }
        }
    }

    return movimento_valido; // retorna se houve movimento ou combinação
}
