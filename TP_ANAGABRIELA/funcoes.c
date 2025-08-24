// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "funcoes.h"
#include "exemplo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ==================== FUNÇÕES DE DESENHO ====================
// Função A: Desenha linha horizontal da tabela
void desenhar_linha(int largura, char inicio, char fim, char linha_char) { //char incio é as juntas
    printf("%c", inicio);
    for (int i = 0; i < largura - 2; i++) {
        printf("%c", linha_char);
    }
    printf("%c\n", fim);
}

// Função B: Desenha linha com texto centralizado (ignora códigos de cor)
void desenhar_texto_centralizado(int largura, const char *texto, const char *cor) {
    int comprimento_real = 0;
    int em_codigo = 0;
    
    // Calcula comprimento real sem códigos de cor
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == '\x1b') { em_codigo = 1; continue; }
        if (em_codigo) { if (texto[i] == 'm') em_codigo = 0; continue; }
        comprimento_real++;
    }
    
    int espacos_esquerda = (largura - comprimento_real - 2) / 2;
    int espacos_direita = largura - comprimento_real - espacos_esquerda - 3;
    
    printf("║");
    for (int i = 0; i < espacos_esquerda; i++) printf(" ");
    printf("%s%s%s", cor, texto, RESET);
    for (int i = 0; i < espacos_direita; i++) printf(" ");
    printf("║\n");
}

// [C] Desenha tabela completa com título e itens
void desenhar_tabela(const char *titulo, const char **itens, const char **cores, int total_itens, int largura) {
    desenhar_linha(largura, '╔', '╗', '═');
    desenhar_texto_centralizado(largura, titulo, AMARELO);
    desenhar_linha(largura, '╠', '╣', '═');
    
    for (int i = 0; i < total_itens; i++) {
        desenhar_texto_centralizado(largura, itens[i], cores[i]);
    }
    
    desenhar_linha(largura, '╚', '╝', '═');
}

// ==================== FUNÇÕES BÁSICAS ====================

// [D] Tela inicial do jogo
void telaInicial() {
    system("clear");
    
    const char *titulo = "🎮 BEM-VINDO AO 2048 🎮";
    const char *itens[] = {
        "Criado por: Ana Gabriela",
        "Matrícula: 25.1.4119",
        "♥ Puca lover ♥",
        "\"Professor, divirta-se!\""
    };
    const char *cores[] = {AZUL, AZUL, VERMELHO, VERDE};
    
    desenhar_tabela(titulo, itens, cores, 4, 50);
    
    printf("\n");
    printf(AMARELO "Pressione ENTER para começar...\n" RESET);
    getchar();
}

// [E] Limpa buffer do teclado
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ==================== FUNÇÕES DE MATRIZ ====================

// [1] Cria matriz dinamicamente
int **criar_matriz(int n) {
    int **matriz = malloc(n * sizeof(int *));
    if (!matriz) return NULL;
    
    for (int i = 0; i < n; i++) {
        matriz[i] = malloc(n * sizeof(int));
        if (!matriz[i]) {
            for (int j = 0; j < i; j++) free(matriz[j]);
            free(matriz);
            return NULL;
        }
        for (int j = 0; j < n; j++) matriz[i][j] = 0;
    }
    return matriz;
}

// [2] Escolhe cor baseada no valor
const char* cor_valor(int valor) {
    if (valor == 2) return VERMELHO;
    if (valor == 4) return VERDE;
    if (valor == 8) return AMARELO;
    if (valor == 16) return AZUL;
    if (valor == 32) return MAGENTA;
    if (valor == 64) return CIANA;
    if (valor == 128) return NEGRITO VERMELHO;
    if (valor == 256) return NEGRITO VERDE;
    if (valor == 512) return NEGRITO AMARELO;
    if (valor == 1024) return NEGRITO AZUL;
    if (valor == 2048) return NEGRITO MAGENTA;
    return NEGRITO CIANA;
}

// [3] Imprime o tabuleiro do jogo
void imprimir_matriz(Jogo *jogo) {
    int tamanho = jogo->tamanho_tab;
    
    // Cabeçalho
    printf("\n");
    desenhar_linha(60, '╔', '╗', '═');
    
    char info[100];
    snprintf(info, sizeof(info), "Jogador: %s | Pontos: %d | Trocas: %d | Desfazer: %d",
             jogo->nome, jogo->pontos, jogo->trocas_restantes, jogo->desfazer_restantes);
    desenhar_texto_centralizado(60, info, VERDE);
    
    desenhar_linha(60, '╚', '╝', '═');
    printf("\n");

    // Encontra maior valor para ajustar tamanho da célula
    int maior_valor = 0;
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            if (jogo->matriz_tab[i][j] > maior_valor) {
                maior_valor = jogo->matriz_tab[i][j];
            }
        }
    }
    
    int tamanho_celula = 6;
    if (maior_valor >= 1000) tamanho_celula = 8;
    else if (maior_valor >= 100) tamanho_celula = 7;

    // Imprime números das colunas
    printf("     ");
    for (int j = 0; j < tamanho; j++) {
        printf(AZUL " %*d " RESET, tamanho_celula, j + 1);
    }
    printf("\n");

    // Borda superior
    printf("   ┏");
    for (int j = 0; j < tamanho; j++) {
        for (int k = 0; k < tamanho_celula; k++) printf("━");
        if (j < tamanho - 1) printf("┳");
    }
    printf("┓\n");

    // Imprime cada linha do tabuleiro
    for (int i = 0; i < tamanho; i++) {
        printf(VERDE "%c  " RESET, 'A' + i);
        printf("┃");
        
        for (int j = 0; j < tamanho; j++) {
            int valor = jogo->matriz_tab[i][j];
            
            if (valor != 0) {
                const char* cor = cor_valor(valor);
                printf("%s%*d%s", cor, tamanho_celula, valor, RESET);
            } else {
                printf("%*s", tamanho_celula, " ");
            }
            printf("┃");
        }
        printf("\n");

        // Bordas entre linhas
        if (i < tamanho - 1) {
            printf("   ┣");
            for (int j = 0; j < tamanho; j++) {
                for (int k = 0; k < tamanho_celula; k++) printf("━");
                if (j < tamanho - 1) printf("╋");
            }
            printf("┫\n");
        } else {
            printf("   ┗");
            for (int j = 0; j < tamanho; j++) {
                for (int k = 0; k < tamanho_celula; k++) printf("━");
                if (j < tamanho - 1) printf("┻");
            }
            printf("┛\n");
        }
    }
    
    printf("\n");
    
    // Legenda de comandos
    printf(AMARELO "Comandos: W (Cima) A (Esquerda) S (Baixo) D (Direita)\n" RESET);
    printf(AMARELO "U (Desfazer) T (Trocar) VOLTAR (Menu)\n" RESET);
    printf("\n");
}

// [4] Adiciona números aleatórios no tabuleiro
void n_aleatorio(Jogo *jogo) {
    int tamanho = jogo->tamanho_tab;
    int quantidade = (tamanho == 6) ? 2 : 1;

    for (int p = 0; p < quantidade; p++) {
        int valor;
        int probabilidade = rand() % 100;

        // Define probabilidades baseadas no tamanho
        if (tamanho == 4) {
            valor = (probabilidade < 90) ? 2 : 4;
        } else if (tamanho == 5) {
            valor = (probabilidade < 85) ? 2 : 4;
        } else {
            valor = (probabilidade < 80) ? 2 : 4;
        }

        // Conta posições vazias
        int vazios = 0;
        for (int i = 0; i < tamanho; i++) {
            for (int j = 0; j < tamanho; j++) {
                if (jogo->matriz_tab[i][j] == 0) {
                    vazios++;
                }
            }
        }
        
        if (vazios == 0) return;
        
        // Escolhe posição aleatória
        int posicao = rand() % vazios;
        int contador = 0;
        
        // Coloca o número na posição escolhida
        for (int i = 0; i < tamanho; i++) {
            for (int j = 0; j < tamanho; j++) {
                if (jogo->matriz_tab[i][j] == 0) {
                    if (contador == posicao) {
                        jogo->matriz_tab[i][j] = valor;
                        return;
                    }
                    contador++;
                }
            }
        }
    }
}

// [5] Libera memória da matriz
void liberar_matriz(int **matriz, int n) {
    if (!matriz) return;
    for (int i = 0; i < n; i++) free(matriz[i]);
    free(matriz);
}

// [6] Cria cópia da matriz
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

// [7] Move e combina números em uma linha
void movimento_linha(int *linha, int tamanho, int *pontos, int *movimento) {
    int combinou[tamanho];
    for (int i = 0; i < tamanho; i++) combinou[i] = 0;
    
    *movimento = 0;
    
    // Primeiro empurra para esquerda
    for (int i = 0; i < tamanho; i++) {
        if (linha[i] != 0) {
            int k = i;
            while (k > 0 && linha[k-1] == 0) {
                linha[k-1] = linha[k];
                linha[k] = 0;
                k--;
                *movimento = 1;
            }
        }
    }
    
    // Combina números iguais
    for (int i = 0; i < tamanho - 1; i++) {
        if (linha[i] != 0 && linha[i] == linha[i+1] && !combinou[i]) {
            linha[i] *= 2;
            linha[i+1] = 0;
            *pontos += linha[i];
            combinou[i] = 1;
            *movimento = 1;
        }
    }
    
    // Empurra novamente após combinar
    for (int i = 0; i < tamanho; i++) {
        if (linha[i] != 0) {
            int k = i;
            while (k > 0 && linha[k-1] == 0) {
                linha[k-1] = linha[k];
                linha[k] = 0;
                k--;
                *movimento = 1;
            }
        }
    }
}

// [8] Move todo o tabuleiro para esquerda
int mover_esquerda(Jogo *jogo) {
    int movimento = 0;
    int n = jogo->tamanho_tab;
    
    for (int i = 0; i < n; i++) {
        int linha[n];
        int moveu = 0;
        
        for (int j = 0; j < n; j++) linha[j] = jogo->matriz_tab[i][j];
        movimento_linha(linha, n, &jogo->pontos, &moveu);
        for (int j = 0; j < n; j++) jogo->matriz_tab[i][j] = linha[j];
        
        if (moveu) movimento = 1;
    }
    return movimento;
}

// [9] Move todo o tabuleiro para direita
int mover_direita(Jogo *jogo) {
    int movimento = 0;
    int n = jogo->tamanho_tab;
    
    for (int i = 0; i < n; i++) {
        int linha[n];
        int moveu = 0;
        
        for (int j = 0; j < n; j++) linha[j] = jogo->matriz_tab[i][n-1-j];
        movimento_linha(linha, n, &jogo->pontos, &moveu);
        for (int j = 0; j < n; j++) jogo->matriz_tab[i][n-1-j] = linha[j];
        
        if (moveu) movimento = 1;
    }
    return movimento;
}

// [10] Move todo o tabuleiro para cima
int mover_cima(Jogo *jogo) {
    int movimento = 0;
    int n = jogo->tamanho_tab;
    
    for (int j = 0; j < n; j++) {
        int coluna[n];
        int moveu = 0;
        
        for (int i = 0; i < n; i++) coluna[i] = jogo->matriz_tab[i][j];
        movimento_linha(coluna, n, &jogo->pontos, &moveu);
        for (int i = 0; i < n; i++) jogo->matriz_tab[i][j] = coluna[i];
        
        if (moveu) movimento = 1;
    }
    return movimento;
}

// [11] Move todo o tabuleiro para baixo
int mover_baixo(Jogo *jogo) {
    int movimento = 0;
    int n = jogo->tamanho_tab;
    
    for (int j = 0; j < n; j++) {
        int coluna[n];
        int moveu = 0;
        
        for (int i = 0; i < n; i++) coluna[i] = jogo->matriz_tab[n-1-i][j];
        movimento_linha(coluna, n, &jogo->pontos, &moveu);
        for (int i = 0; i < n; i++) jogo->matriz_tab[n-1-i][j] = coluna[i];
        
        if (moveu) movimento = 1;
    }
    return movimento;
}

// ==================== DESFAZER / TROCAR ====================

// [12] Desfaz o último movimento
void desfazer_movimento(Jogo *jogo) {
    if (jogo->desfazer_restantes <= 0) {
        printf(VERMELHO "Sem chances de desfazer!\n" RESET);
        return;
    }

    if (!jogo->tab_anterior) {
        printf(VERMELHO "Nada para desfazer!\n" RESET);
        return;
    }

    // Restaura o tabuleiro anterior
    for (int i = 0; i < jogo->tamanho_tab; i++) {
        for (int j = 0; j < jogo->tamanho_tab; j++) {
            jogo->matriz_tab[i][j] = jogo->tab_anterior[i][j];
        }
    }

    jogo->pontos = jogo->pontos_anterior;
    jogo->desfazer_restantes--;
    
    printf(VERDE "Movimento desfeito! Restam %d chances\n" RESET, jogo->desfazer_restantes);
}

// [13] Troca duas peças de posição
void trocar_pecas(Jogo *jogo) {
    if (jogo->trocas_restantes <= 0) {
        printf(VERMELHO "Sem chances de trocar!\n" RESET);
        return;
    }

    char pos1[3], pos2[3];
    int linha1, coluna1, linha2, coluna2;

    printf(AMARELO "Primeira posição (ex: A1): " RESET);
    scanf("%2s", pos1);
    printf(AMARELO "Segunda posição (ex: B2): " RESET);
    scanf("%2s", pos2);
    limpar_buffer();

    // Converte para coordenadas
    linha1 = toupper(pos1[0]) - 'A';
    coluna1 = pos1[1] - '1';
    linha2 = toupper(pos2[0]) - 'A';
    coluna2 = pos2[1] - '1';

    // Valida as posições
    if (linha1 < 0 || linha1 >= jogo->tamanho_tab || coluna1 < 0 || coluna1 >= jogo->tamanho_tab ||
        linha2 < 0 || linha2 >= jogo->tamanho_tab || coluna2 < 0 || coluna2 >= jogo->tamanho_tab) {
        printf(VERMELHO "Posições inválidas!\n" RESET);
        return;
    }

    if (jogo->matriz_tab[linha1][coluna1] == 0 || jogo->matriz_tab[linha2][coluna2] == 0) {
        printf(VERMELHO "Não pode trocar posições vazias!\n" RESET);
        return;
    }

    // Faz a troca
    int temp = jogo->matriz_tab[linha1][coluna1];
    jogo->matriz_tab[linha1][coluna1] = jogo->matriz_tab[linha2][coluna2];
    jogo->matriz_tab[linha2][coluna2] = temp;

    jogo->trocas_restantes--;
    
    printf(VERDE "Peças trocadas! Restam %d trocas\n" RESET, jogo->trocas_restantes);
}

// ==================== VERIFICAÇÕES ====================

// [14] Verifica se ainda há jogadas possíveis
int verificar_jogo(Jogo *jogo) {
    int n = jogo->tamanho_tab;
    
    // Verifica se há espaços vazios
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (jogo->matriz_tab[i][j] == 0) {
                return 1;
            }
        }
    }
    
    // Verifica se há combinações possíveis
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j < n-1 && jogo->matriz_tab[i][j] == jogo->matriz_tab[i][j+1]) {
                return 1;
            }
            if (i < n-1 && jogo->matriz_tab[i][j] == jogo->matriz_tab[i+1][j]) {
                return 1;
            }
        }
    }
    
    return 0;
}

// [15] Verifica se o jogador venceu
int verificar_vitoria(Jogo *jogo) {
    int n = jogo->tamanho_tab;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (jogo->matriz_tab[i][j] == 2048) {
                printf(VERDE "\nParabéns %s! Você atingiu 2048!\n" RESET, jogo->nome);
                printf(AMARELO "Deseja continuar jogando? (S/N): " RESET);
                
                char opcao;
                do {
                    scanf(" %c", &opcao);
                    limpar_buffer();
                    opcao = toupper(opcao);
                } while (opcao != 'S' && opcao != 'N');
                
                if (opcao == 'N') {
                    return 0;
                }
                return 1;
            }
        }
    }
    return 2;
}

// [16] Verifica se o jogador perdeu
int verificar_derrota(Jogo *jogo) {
    if (verificar_jogo(jogo)) return 1;

    printf(VERMELHO "\nGame Over! Sem movimentos possíveis.\n" RESET);
    
    if (jogo->desfazer_restantes > 0) {
        printf(AMARELO "Deseja usar desfazer? (%d restantes) (S/N): " RESET, jogo->desfazer_restantes);
        char opcao;
        scanf(" %c", &opcao);
        limpar_buffer();
        
        if (toupper(opcao) == 'S') {
            desfazer_movimento(jogo);
            return 2;
        }
    }
    
    printf(VERDE "Voltando ao menu...\n" RESET);
    return 0;
}