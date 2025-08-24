// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "funcoes.h"
#include "exemplo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ==================== FUNÇÕES BÁSICAS ====================
void centralizar(const char *texto, int largura_total) {
    // Calcula comprimento real ignorando códigos de cor
    int comprimento_real = 0;
    int dentro_codigo = 0;
    
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == '\x1b') { // Início do código ANSI
            dentro_codigo = 1;
            continue;
        }
        if (dentro_codigo) {
            if (texto[i] == 'm') { // Fim do código ANSI
                dentro_codigo = 0;
            }
            continue;
        }
        comprimento_real++;
    }
    
    int espacos = (largura_total - comprimento_real) / 2;
    if (espacos < 0) espacos = 0;
    
    for (int i = 0; i < espacos; i++) {
        printf(" ");
    }
    printf("%s\n", texto);
}

void telaInicial() {
    system("clear");
    
    printf(CIANA "╔══════════════════════════════════════════════╗\n" RESET);
    printf(CIANA "║" AMARELO "            🎮 BEM-VINDO AO 2048 🎮            " CIANA "║\n" RESET);
    printf(CIANA "╠══════════════════════════════════════════════╣\n" RESET);
    printf(CIANA "║" AZUL "         Criado por: Ana Gabriela         " CIANA "║\n" RESET);
    printf(CIANA "║" AZUL "           Matrícula: 25.1.4119           " CIANA "║\n" RESET);
    printf(CIANA "║" VERMELHO "             ♥ Puca lover ♥              " CIANA "║\n" RESET);
    printf(CIANA "║" VERDE "     \"Professor, espero que se divirta!\"    " CIANA "║\n" RESET);
    printf(CIANA "╚══════════════════════════════════════════════╝\n" RESET);
    
    printf("\n");
    printf(AMARELO "        Pressione ENTER para iniciar o jogo...\n" RESET);
    
    getchar();
}

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ==================== MATRIZ ====================
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

const char* cor_valor(int valor) {
    if (valor == 2) return VERMELHO;
    if (valor == 4) return VERDE;
    if (valor == 8) return AMARELO;
    if (valor == 16) return AZUL;
    if (valor == 32) return MAGENTA;
    if (valor == 64) return CIANA;
    return NEGRITO;
}

void imprimir_matriz(Jogo *jogo) {
    int tamanho = jogo->tamanho_tab;
    int largura_total = 60;

    // Cabeçalho simples
    printf("\n");
    printf(CIANA "╔══════════════════════════════════════════════╗\n" RESET);
    printf(CIANA "║" AMARELO "               🎮 JOGO 2048 🎮               " CIANA "║\n" RESET);
    printf(CIANA "╠══════════════════════════════════════════════╣\n" RESET);
    
    // Informações em linhas separadas
    printf(CIANA "║" VERDE " Jogador: %-30s " CIANA "║\n" RESET, jogo->nome);
    printf(CIANA "║" AZUL " Pontos: %-4d " MAGENTA "Trocas: %-2d " VERMELHO "Desfazer: %-2d " CIANA "║\n" RESET,
           jogo->pontos, jogo->trocas_restantes, jogo->desfazer_restantes);
    
    printf(CIANA "╚══════════════════════════════════════════════╝\n" RESET);
    printf("\n");

    // ... o resto do código do tabuleiro permanece igual ...
    // [insira aqui o código do tabuleiro que já estava funcionando]
}

void n_aleatorio(Jogo *jogo_atual) {
    int n = jogo_atual->tamanho_tab;
    int pecas;

    if (n == 6) pecas = 2;
    else pecas = 1;

    for (int p = 0; p < pecas; p++) {
        int valor;
        int prob = rand() % 100;

        if (n == 4) {
            if (prob < 90) valor = 2;
            else valor = 4;
        } else if (n == 5) {
            if (prob < 85) valor = 2;
            else valor = 4;
        } else {
            if (prob < 80) valor = 2;
            else valor = 4;
        }

        int posicoes_vazias = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (jogo_atual->matriz_tab[i][j] == 0) {
                    posicoes_vazias++;
                }
            }
        }
        
        if (posicoes_vazias == 0) return;
        
        int pos_aleatoria = rand() % posicoes_vazias;
        int contador = 0;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (jogo_atual->matriz_tab[i][j] == 0) {
                    if (contador == pos_aleatoria) {
                        jogo_atual->matriz_tab[i][j] = valor;
                        break;
                    }
                    contador++;
                }
            }
        }
    }
}

void liberar_matriz(int **matriz, int n) {
    if (!matriz) return;
    for (int i = 0; i < n; i++) free(matriz[i]);
    free(matriz);
}

int **copiar_matriz(int **matriz, int tamanho) {
    if (!matriz) return NULL;
    
    int **copia = criar_matriz(tamanho);
    if (!copia) return NULL;
    
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            copia[i][j] = matriz[i][j];
        }
    }
    return copia;
}

// ==================== MOVIMENTAÇÃO ====================
void movimento_nao_guloso(int *linha, int tamanho, int *pontuacao, int *houve_movimento) {
    int combinada[tamanho];
    
    for (int i = 0; i < tamanho; i++) {
        combinada[i] = 0;
    }
    
    *houve_movimento = 0;
    
    // Primeiro empurra
    for (int i = 0; i < tamanho; i++) {
        if (linha[i] != 0) {
            int k = i;
            while (k > 0 && linha[k-1] == 0) {
                linha[k-1] = linha[k];
                linha[k] = 0;
                k--;
                *houve_movimento = 1;
            }
        }
    }
    
    // Combina números iguais (não-guloso)
    for (int i = 0; i < tamanho - 1; i++) {
        if (linha[i] != 0 && linha[i] == linha[i+1] && !combinada[i]) {
            linha[i] *= 2;
            linha[i+1] = 0;
            *pontuacao += linha[i];
            combinada[i] = 1;
            *houve_movimento = 1;
            combinada[i+1] = 1;
        }
    }
    
    // Segundo empurra
    for (int i = 0; i < tamanho; i++) {
        if (linha[i] != 0) {
            int k = i;
            while (k > 0 && linha[k-1] == 0) {
                linha[k-1] = linha[k];
                linha[k] = 0;
                k--;
                *houve_movimento = 1;
            }
        }
    }
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

// ==================== DESFAZER / TROCAR ====================
void desfazer_movimento(Jogo *jogo) {
    if (jogo->desfazer_restantes <= 0) {
        printf(VERMELHO "\n Sem chances de desfazer! ❌\n" RESET);
        return;
    }

    if (!jogo->tab_anterior) {
        printf(VERMELHO "\n Nada para desfazer! ❌\n" RESET);
        return;
    }

    // Restaura o tabuleiro anterior
    for (int linha = 0; linha < jogo->tamanho_tab; linha++) {
        for (int coluna = 0; coluna < jogo->tamanho_tab; coluna++) {
            jogo->matriz_tab[linha][coluna] = jogo->tab_anterior[linha][coluna];
        }
    }

    jogo->pontos = jogo->pontos_anterior;
    jogo->desfazer_restantes--;
    
    printf(VERDE "\n Movimento desfeito! ↩️\n" RESET);
    printf(AZUL " Chances restantes: %d\n" RESET, jogo->desfazer_restantes);
}

void trocar_pecas(Jogo *jogo) {
    if (jogo->trocas_restantes <= 0) {
        printf(VERMELHO "\n Sem chances de trocar! ❌\n" RESET);
        return;
    }

    char posicao1[10], posicao2[10];
    int linha1, coluna1, linha2, coluna2;

    printf(AMARELO "\n Primeira posição (ex: A1): " RESET);
    if (scanf("%9s", posicao1) != 1) {
        limpar_buffer();
        printf(VERMELHO "\n Entrada inválida!\n" RESET);
        return;
    }

    printf(AMARELO " Segunda posição (ex: B2): " RESET);
    if (scanf("%9s", posicao2) != 1) {
        limpar_buffer();
        printf(VERMELHO "\n Entrada inválida!\n" RESET);
        return;
    }
    limpar_buffer();

    // Valida formato das posições
    if (strlen(posicao1) != 2 || strlen(posicao2) != 2 ||
        !isalpha(posicao1[0]) || !isdigit(posicao1[1]) ||
        !isalpha(posicao2[0]) || !isdigit(posicao2[1])) {
        printf(VERMELHO "\n Formato inválido! Use formato LetraNúmero (ex: A1)\n" RESET);
        return;
    }

    // Converte letras para números
    linha1 = toupper(posicao1[0]) - 'A';
    coluna1 = posicao1[1] - '1';
    linha2 = toupper(posicao2[0]) - 'A';
    coluna2 = posicao2[1] - '1';

    // Verifica se as posições são válidas
    if (linha1 < 0 || linha1 >= jogo->tamanho_tab || linha2 < 0 || linha2 >= jogo->tamanho_tab ||
        coluna1 < 0 || coluna1 >= jogo->tamanho_tab || coluna2 < 0 || coluna2 >= jogo->tamanho_tab) {
        printf(VERMELHO "\n Posições fora do tabuleiro! ❌\n" RESET);
        return;
    }

    // Verifica se as posições não estão vazias
    if (jogo->matriz_tab[linha1][coluna1] == 0 || jogo->matriz_tab[linha2][coluna2] == 0) {
        printf(VERMELHO "\n Não é possível trocar posições vazias! ❌\n" RESET);
        return;
    }

    // Troca as peças
    int temp = jogo->matriz_tab[linha1][coluna1];
    jogo->matriz_tab[linha1][coluna1] = jogo->matriz_tab[linha2][coluna2];
    jogo->matriz_tab[linha2][coluna2] = temp;

    jogo->trocas_restantes--;
    
    printf(VERDE "\n Peças trocadas com sucesso! 🔄\n" RESET);
    printf(AZUL " Trocas restantes: %d\n" RESET, jogo->trocas_restantes);
}

void verificar_ganho_movimentos_especiais(Jogo *jogo) {
    int n = jogo->tamanho_tab;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (jogo->matriz_tab[i][j] == 256) {
                jogo->desfazer_restantes++;
                printf("Parabéns! Você ganhou uma chance de desfazer por atingir 256!\n");
            }
            if (jogo->matriz_tab[i][j] == 512) {
                jogo->trocas_restantes++;
                printf("Parabéns! Você ganhou uma chance de trocar por atingir 512!\n");
            }
        }
    }
}

// ==================== VERIFICAÇÕES ====================
// Adicione estas funções no final do funcoes.c

int verificar_jogo(Jogo *jogo) {
    int tamanho = jogo->tamanho_tab;
    
    // Verifica se há espaços vazios
    for (int linha = 0; linha < tamanho; linha++) {
        for (int coluna = 0; coluna < tamanho; coluna++) {
            if (jogo->matriz_tab[linha][coluna] == 0) {
                return 1; // Ainda há espaços vazios
            }
        }
    }
    
    // Verifica se há movimentos possíveis
    for (int linha = 0; linha < tamanho; linha++) {
        for (int coluna = 0; coluna < tamanho; coluna++) {
            // Verifica direita
            if (coluna < tamanho - 1 && jogo->matriz_tab[linha][coluna] == jogo->matriz_tab[linha][coluna + 1]) {
                return 1;
            }
            // Verifica baixo
            if (linha < tamanho - 1 && jogo->matriz_tab[linha][coluna] == jogo->matriz_tab[linha + 1][coluna]) {
                return 1;
            }
        }
    }
    
    return 0; // Não há mais movimentos
}

int verificar_vitoria(Jogo *jogo) {
    int tamanho = jogo->tamanho_tab;
    
    for (int linha = 0; linha < tamanho; linha++) {
        for (int coluna = 0; coluna < tamanho; coluna++) {
            if (jogo->matriz_tab[linha][coluna] == 2048) {
                printf(CIANA "\n╔══════════════════════════════════════════════╗\n" RESET);
                printf(CIANA "║" AMARELO "           🎉 PARABÉNS %s! 🎉           " CIANA "║\n" RESET, jogo->nome);
                printf(CIANA "║" VERDE "        Você atingiu 2048! 🏆         " CIANA "║\n" RESET);
                printf(CIANA "╚══════════════════════════════════════════════╝\n" RESET);
                
                printf(AMARELO "\n Deseja continuar jogando? (S/N): " RESET);
                
                char opcao;
                do {
                    scanf(" %c", &opcao);
                    limpar_buffer();
                    opcao = toupper(opcao);
                } while (opcao != 'S' && opcao != 'N');
                
                if (opcao == 'N') {
                    printf(VERDE "\n Obrigado por jogar! 👋\n" RESET);
                    return 0;
                }
                
                printf(VERDE "\n Continuando o jogo... 🎮\n" RESET);
                printf(AMARELO " Pressione ENTER para continuar..." RESET);
                getchar();
                
                return 1;
            }
        }
    }
    return 2;
}

int verificar_derrota(Jogo *jogo) {
    if (verificar_jogo(jogo)) return 1;

    printf(CIANA "\n╔══════════════════════════════════════════════╗\n" RESET);
    printf(CIANA "║" VERMELHO "           ⚠️  GAME OVER! ⚠️           " CIANA "║\n" RESET);
    printf(CIANA "║" VERMELHO "   Não há mais movimentos possíveis   " CIANA "║\n" RESET);
    printf(CIANA "╚══════════════════════════════════════════════╝\n" RESET);
    
    if (jogo->desfazer_restantes > 0) {
        printf(AMARELO "\n Deseja usar DESFAZER? Você tem %d chances. (S/N): " RESET, jogo->desfazer_restantes);
        char opcao;
        scanf(" %c", &opcao);
        limpar_buffer();
        
        if (toupper(opcao) == 'S') {
            desfazer_movimento(jogo);
            printf(AMARELO " Pressione ENTER para continuar..." RESET);
            getchar();
            return 2;
        }
    }
    
    printf(VERDE "\n Voltando ao menu principal... 🏠\n" RESET);
    printf(AMARELO " Pressione ENTER para continuar..." RESET);
    getchar();
    
    return 0;
}