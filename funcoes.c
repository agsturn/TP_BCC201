// Ana Gabriela Gomes Lopes Pereira - 25.1.4119

#include "exemplo.h"        
#include "funcoes.h"        
#include <stdio.h>          
#include <stdlib.h>         
#include <string.h>        
#include <ctype.h>         
#include <time.h>           

// ==================== PROTÓTIPOS DAS FUNÇÕES LOCAIS ====================
// REMOVIDO: int ha_movimentos_possiveis(Jogo *jogo); // AGORA ESTÁ NO HEADER

// ==================== FUNÇÕES BÁSICAS ====================

// Função 1: Centraliza um texto na tela considerando códigos de cor ANSI
void centralizar(const char *texto, int largura_total) {
    int comprimento_real = 0;    // Armazena o comprimento real do texto visível
    int dentro_codigo = 0;      
    
    // Calcula comprimento real ignorando códigos de cor ANSI
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == '\x1b') { // Caractere que mostra início do código ANSI
            dentro_codigo = 1;    // Marca que está dentro de um código
            continue;             // Pula para o próximo caractere
        }
        if (dentro_codigo) {      
            if (texto[i] == 'm') { // 'm' indica fim do código ANSI
                dentro_codigo = 0; // Marca que saiu do código
            }
            continue;             // Pula caracteres do código ANSI
        }
        comprimento_real++;       // Conta apenas caracteres visíveis do texto
    }
    
    // Calcula espaços necessários para centralizar o texto
    int espacos = (largura_total - comprimento_real) / 2;
    if (espacos < 0) espacos = 0; 
    
    // Imprime os espaços antes do texto para centralização
    for (int i = 0; i < espacos; i++) {
        printf(" ");              
    }
    printf("%s\n", texto);        // Imprime o texto com cores
}

// Função 2: Exibe a tela inicial do jogo
void tela_inicial(void) {
    int result = system("clear");  // Armazena o resultado
    (void)result;  // Descarta o resultado intencionalmente (elimina warning)
    
    printf("\n");
    printf(CIANA "════════════════════════════════════════════\n" RESET);
    printf(AMARELO NEGRITO "            BEM-VINDO AO 2048!\n" RESET);
    printf(CIANA "════════════════════════════════════════════\n" RESET);
    printf(VERDE "         Criado por: Ana Gabriela\n");
    printf("           Matricula: 25.1.4119\n");
    printf(MAGENTA "             ♥ Puca lover ♥\n");
    printf(AMARELO "   \"Professor, espero que se divirta!\"\n" RESET);
    printf(CIANA "════════════════════════════════════════════\n" RESET);
    printf("\n");
    printf(VERDE "   Pressione ENTER para iniciar o jogo...\n" RESET);
    printf("\n");
    
    getchar(); // Aguarda ENTER 
}

// Função 3: Limpa o buffer de entrada do teclado
void limpar_buffer(void) {
    int c; 
    // Lê e descarta todos os caracteres até encontrar nova linha ou fim de arquivo
    while ((c = getchar()) != '\n' && c != EOF);
}

// ==================== FUNÇÕES AUXILIARES ====================

// Função 4: Processa posição "A1"
int processar_posicao(const char *entrada, int *linha, int *coluna) {
    // Verifica se a entrada tem pelo menos 2 caracteres
    if (strlen(entrada) < 2) return 0; 
    
    char letra = toupper(entrada[0]);    // Converte a primeira letra para maiúscula
    char numero_char = entrada[1];       // Pega o dígito
    
    if (letra < 'A' || letra > 'Z') {
        return 0;
    }
    
    if (numero_char < '1' || numero_char > '9') {
        return 0;
    }
    
    // Converte letra para índice de coluna (A=0, B=1, C=2, ...)
    *coluna = letra - 'A';
    // Converte dígito para índice de linha (1=0, 2=1, 3=2, ...)
    *linha = numero_char - '1';
    
    return 1; // Retorna 1 se deu certo 
}

// ==================== FUNÇÕES DE MATRIZ ====================

// Função 5: Cria uma matriz quadrada dinamicamente na memória
int **criar_matriz(int tamanho) {
    int **matriz = malloc(tamanho * sizeof(int *));
    if (!matriz) {
        return NULL; // Retorna NULL se falhar a alocação
    }
    
    for (int i = 0; i < tamanho; i++) {
        matriz[i] = malloc(tamanho * sizeof(int)); // Aloca uma linha
        if (!matriz[i]) { 
            for (int k = 0; k < i; k++) {
                free(matriz[k]);
            }
            free(matriz);
            return NULL; // Retorna NULL indicando falha
        }
        // Inicializa todas as posições da linha com zero
        for (int j = 0; j < tamanho; j++) {
            matriz[i][j] = 0;
        }
    }
    return matriz; 
}

// Função 6: Retorna a cor ANSI correspondente ao valor da peça
const char* obter_cor_valor(int valor) {
    switch(valor) {
        case 2:       return VERMELHO;    
        case 4:       return VERDE;       
        case 8:       return AMARELO;     
        case 16:      return AZUL;        
        case 32:      return MAGENTA;     
        case 64:      return CIANA;       
        case 128:     return NEGRITO VERMELHO;   
        case 256:     return NEGRITO VERDE;      
        case 512:     return NEGRITO AMARELO;   
        case 1024:    return NEGRITO AZUL;       
        case 2048:    return NEGRITO MAGENTA;    
        default:      return NEGRITO;            
    }
}

// Função 7: Imprime o tabuleiro do jogo formatado com cores e bordas
void imprimir_tabuleiro(Jogo *jogo) {
    int n = jogo->tamanho_tabuleiro; 
    
    printf("\n");
    printf(CIANA "══════════════════════════════════════════════════\n" RESET);
    printf(AMARELO NEGRITO "                🎮 JOGO 2048 🎮\n" RESET);
    printf(CIANA "══════════════════════════════════════════════════\n" RESET);
 
    printf(VERDE "Jogador: %s\n" RESET, jogo->nome_jogador);
    printf(AZUL "Pontos: %d  " MAGENTA "Trocas: %d  " VERMELHO "Desfazer: %d\n" RESET,
           jogo->pontuacao, jogo->trocas_disponiveis, jogo->desfazer_disponiveis);
    printf(CIANA "══════════════════════════════════════════════════\n" RESET);
    printf("\n");

    // Imprime números das colunas 
    printf("       ");
    for (int j = 0; j < n; j++) {
        printf(AMARELO "     %d      " RESET, j + 1);
    }
    printf("\n");

    // Linha superior do tabuleiro com caracteres de borda
    printf("       ╔");
    for (int j = 0; j < n; j++) {
        printf("═══════════"); // Linha horizontal
        if (j < n - 1) printf("╦"); // Junta se não for a última coluna
        else printf("╗\n");     // Canto superior direito
    }

    // Imprime cada linha do tabuleiro com letras 
    for (int i = 0; i < n; i++) {
        printf(AMARELO "   %c   " RESET "║", 'A' + i); // Letra da linha
        
        // Imprime cada célula da linha
        for (int j = 0; j < n; j++) {
            int valor = jogo->matriz_atual[i][j]; // Valor da célula
            if (valor == 0) {
                printf("           ║"); 
            } else {
                if (valor < 10) {
                    printf("     %s%d%s     ║", obter_cor_valor(valor), valor, RESET);
                } else if (valor < 100) {
                    printf("    %s%2d%s    ║", obter_cor_valor(valor), valor, RESET);
                } else if (valor < 1000) {
                    printf("    %s%3d%s   ║", obter_cor_valor(valor), valor, RESET);
                } else if (valor < 10000) {
                    printf("   %s%4d%s   ║", obter_cor_valor(valor), valor, RESET);
                } else {
                    printf("   %s%5d%s  ║", obter_cor_valor(valor), valor, RESET);
                }
            }
        }
        printf("\n");

        // Linhas separadoras entre as linhas do tabuleiro
        if (i < n - 1) {
            printf("       ╠");
            for (int j = 0; j < n; j++) {
                printf("═══════════");
                if (j < n - 1) printf("╬"); // Junta do meio
                else printf("╣\n");         // Borda direita
            }
        }
    }

    // Linha inferior do tabuleiro
    printf("       ╚");
    for (int j = 0; j < n; j++) {
        printf("═══════════");
        if (j < n - 1) printf("╩"); // Junta de baixo
        else printf("╝\n");         // Canto inferior direito
    }

    printf(CIANA "\n══════════════════════════════════════════════════\n" RESET);
    printf(AMARELO NEGRITO "COMANDOS:\n" RESET);
    printf(VERDE "  W, A, S, D - Mover (cima, esquerda, baixo, direita)\n");
    printf(MAGENTA "  U          - Desfazer movimento\n");
    printf(AZUL "  T pos1 pos2- Trocar peças (ex: T A1 B2)\n");
    printf(VERMELHO "  voltar     - Voltar ao menu\n" RESET);
    printf(CIANA "══════════════════════════════════════════════════\n" RESET);
    printf("\n");
}

// Função 8: Adiciona número aleatório em posição vazia do tabuleiro
void adicionar_numero_aleatorio(Jogo *jogo) {
    int n = jogo->tamanho_tabuleiro;
    int pecas_adicionar;   
    int p_2;               // Probabilidade de ser 2 
    
    // Configura baseado no tamanho do tabuleiro
    switch (n) {
        case 4: pecas_adicionar = 1; p_2 = 90; break; // 4x4: 1 peça, 90% chance de 2
        case 5: pecas_adicionar = 1; p_2 = 85; break; // 5x5: 1 peça, 85% chance de 2
        case 6: pecas_adicionar = 2; p_2 = 80; break; // 6x6: 2 peças, 80% chance de 2
        default: return; // Tamanho inválido
    }

    // Conta quantas posições vazias existem no tabuleiro
    int posicoes_vazias = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (jogo->matriz_atual[i][j] == 0) {
                posicoes_vazias++;
            }
        }
    }

    if (posicoes_vazias == 0) {
        return; // Não há espaço para adicionar peças
    }

    // Adiciona as peças aleatórias
    for (int p = 0; p < pecas_adicionar && posicoes_vazias > 0; p++) {
        int valor;
        int probabilidade = rand() % 100; // Gera número aleatório entre 0-99
        
        if (probabilidade < p_2) {
            valor = 2; // Maior probabilidade (90%, 85% ou 80%)
        } else {
            valor = 4; // Menor probabilidade (10%, 15% ou 20%)
        }
        
        // Escolhe uma posição vazia aleatória
        int pos_aleatoria = rand() % posicoes_vazias;
        int contador = 0;    // Contador de posições vazias encontradas
        int inseriu = 0;     // Controlar se inseriu a peça

        // Percorre o tabuleiro procurando a posição vazia escolhida
        for (int i = 0; i < n && !inseriu; i++) {
            for (int j = 0; j < n; j++) {
                if (jogo->matriz_atual[i][j] == 0) {
                    if (contador == pos_aleatoria) {
                        jogo->matriz_atual[i][j] = valor; 
                        posicoes_vazias--;               
                        inseriu = 1;                     
                        break;                           
                    }
                    contador++; 
                }
            }
        }
    }
}

// Função 9: Libera memória alocada para uma matriz
void liberar_matriz(int **matriz, int tamanho) {
    if (!matriz) {
        return;
    } 
    
    for (int i = 0; i < tamanho; i++) {
        free(matriz[i]);
    }
    free(matriz); 
}

// Função 10: Cria uma cópia da matriz
int **copiar_matriz(int **matriz, int tamanho) {
    if (!matriz) {
        return NULL; 
    }
    
    // Cria uma nova matriz
    int **copia = criar_matriz(tamanho);
    if (!copia) {
        return NULL;
    } 
    
    // Copia todos os valores da matriz original para a cópia
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            copia[i][j] = matriz[i][j];
        }
    }
    return copia; 
}

// ==================== FUNÇÕES DE DESFAZER E TROCAR ====================

// Função 11: Desfaz o último movimento realizado
void desfazer_movimento(Jogo *jogo) {
    // Verifica se ainda há chances de desfazer
    if (jogo->desfazer_disponiveis <= 0) {
        printf(VERMELHO "\n Sem chances de desfazer! ❌\n" RESET);
        return;
    }

    // Verifica se existe estado anterior salvo
    if (!jogo->matriz_anterior) {
        printf(VERMELHO "\n Nada para desfazer! ❌\n" RESET);
        return;
    }

    // Restaura o estado anterior copiando a matriz
    for (int linha = 0; linha < jogo->tamanho_tabuleiro; linha++) {
        for (int coluna = 0; coluna < jogo->tamanho_tabuleiro; coluna++) {
            jogo->matriz_atual[linha][coluna] = jogo->matriz_anterior[linha][coluna];
        }
    }

    // Restaura a pontuação anterior
    jogo->pontuacao = jogo->pontuacao_anterior;
    jogo->desfazer_disponiveis--; // Reduz as chances disponíveis
    
    printf(VERDE "\n Movimento desfeito! ↩️\n" RESET);
    printf(AZUL " Chances restantes: %d\n" RESET, jogo->desfazer_disponiveis);
}

// Função 12: Troca peças de forma interativa (pede posições ao usuário)
void trocar_pecas(Jogo *jogo) {
    // Verifica se ainda há trocas disponíveis
    if (jogo->trocas_disponiveis <= 0) {
        printf(VERMELHO "\n Sem chances de trocar! ❌\n" RESET);
        return;
    }

    char entrada1[20], entrada2[20]; 
    int linha1, coluna1, linha2, coluna2; 

    // Pede a primeira posição ao usuário
    printf(AMARELO "\n Primeira posição (ex: A1, a1, A 1): " RESET);
    if (scanf("%19s", entrada1) != 1) { // Lê até 19 caracteres
        limpar_buffer(); // Limpa buffer em caso de erro
        printf(VERMELHO "\n Entrada inválida!\n" RESET);
        return;
    }
    limpar_buffer(); // Limpa o buffer após leitura

    // Pede a segunda posição ao usuário
    printf(AMARELO " Segunda posição (ex: B2, b2, B 2): " RESET);
    if (scanf("%19s", entrada2) != 1) {
        limpar_buffer();
        printf(VERMELHO "\n Entrada inválida!\n" RESET);
        return;
    }
    limpar_buffer();

    // Processa e valida a primeira posição
    if (!processar_posicao(entrada1, &linha1, &coluna1)) {
        printf(VERMELHO "\n Primeira posição inválida! Use formato como A1, B2, etc.\n" RESET);
        return;
    }

    // Processa e valida a segunda posição
    if (!processar_posicao(entrada2, &linha2, &coluna2)) {
        printf(VERMELHO "\n Segunda posição inválida! Use formato como A1, B2, etc.\n" RESET);
        return;
    }

    // Verifica se as posições estão dentro dos limites do tabuleiro
    if (linha1 < 0 || linha1 >= jogo->tamanho_tabuleiro || 
        linha2 < 0 || linha2 >= jogo->tamanho_tabuleiro ||
        coluna1 < 0 || coluna1 >= jogo->tamanho_tabuleiro || 
        coluna2 < 0 || coluna2 >= jogo->tamanho_tabuleiro) {
        printf(VERMELHO "\n Posições fora do tabuleiro! ❌\n" RESET);
        return;
    }

    // Verifica se não está tentando trocar a mesma posição
    if (linha1 == linha2 && coluna1 == coluna2) {
        printf(VERMELHO "\n Não é possível trocar a mesma posição! ❌\n" RESET);
        return;
    }

    // Verifica se ambas as posições não estão vazias
    if (jogo->matriz_atual[linha1][coluna1] == 0 || jogo->matriz_atual[linha2][coluna2] == 0) {
        printf(VERMELHO "\n Não é possível trocar posições vazias! ❌\n" RESET);
        return;
    }

    // Realiza a troca dos valores
    int temp = jogo->matriz_atual[linha1][coluna1];
    jogo->matriz_atual[linha1][coluna1] = jogo->matriz_atual[linha2][coluna2];
    jogo->matriz_atual[linha2][coluna2] = temp;

    jogo->trocas_disponiveis--; // Reduz o contador de trocas disponíveis
    
    printf(VERDE "\n Peças trocadas com sucesso! 🔄\n" RESET);
    printf(AZUL " Trocas restantes: %d\n" RESET, jogo->trocas_disponiveis);
}

// ==================== FUNÇÕES DE MOVIMENTAÇÃO ====================

//Função 13: Simula o movimento seguindo a regra do jogo
void movimento_nao_guloso(int *linha, int tamanho, int *pontuacao, int *houve_movimento) {
    int combinada[tamanho]; // Controla quais posições já combinaram
    
    for (int i = 0; i < tamanho; i++) {
        combinada[i] = 0;
    }
    
    *houve_movimento = 0;
    
    // Empurra todos para o início
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
    
    // Combina números iguais vizinhos
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
    
    // Empurra novamente após combinações
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

// Função 14: Move para esquerda
int mover_esquerda(Jogo *jogo){
    int n = jogo->tamanho_tabuleiro;
    int movimento = 0;

    for(int i = 0; i < n; i++){
        int houve_movimento = 0;
        int linha[n];

        // Copia a linha atual
        for(int j = 0; j < n; j++)
            linha[j] = jogo->matriz_atual[i][j];

        movimento_nao_guloso(linha, n, &(jogo->pontuacao), &houve_movimento);

        // Atualiza a linha no tabuleiro
        for(int j = 0; j < n; j++)
            jogo->matriz_atual[i][j] = linha[j];

        if(houve_movimento) movimento = 1;
    }
    return movimento;
}

//Função 15: Move para direita
int mover_direita(Jogo *jogo){
    int n = jogo->tamanho_tabuleiro;
    int movimento = 0;

    for(int i = 0; i < n; i++){
        int houve_movimento = 0;
        int linha[n];

        // Copia a linha invertida (para simular movimento à direita)
        for(int j = 0; j < n; j++)
            linha[j] = jogo->matriz_atual[i][n-1-j];

        movimento_nao_guloso(linha, n, &(jogo->pontuacao), &houve_movimento);

        // Restaura a linha invertida
        for(int j = 0; j < n; j++)
            jogo->matriz_atual[i][n-1-j] = linha[j];

        if(houve_movimento) movimento = 1;
    }
    return movimento;
}

// Função 16: Move para cima
int mover_cima(Jogo *jogo){
    int n = jogo->tamanho_tabuleiro;
    int movimento = 0;

    for(int j = 0; j < n; j++){
        int houve_movimento = 0;
        int coluna[n];

        // Copia a coluna
        for(int i = 0; i < n; i++)
            coluna[i] = jogo->matriz_atual[i][j];

        movimento_nao_guloso(coluna, n, &(jogo->pontuacao), &houve_movimento);

        // Atualiza a coluna
        for(int i = 0; i < n; i++)
            jogo->matriz_atual[i][j] = coluna[i];

        if(houve_movimento) movimento = 1;
    }
    return movimento;
}

// Função 17: Move para baixo
int mover_baixo(Jogo *jogo){
    int n = jogo->tamanho_tabuleiro;
    int movimento = 0;

    for(int j = 0; j < n; j++){
        int houve_movimento = 0;
        int coluna[n];

        // Copia a coluna invertida (para simular movimento para baixo)
        for(int i = 0; i < n; i++)
            coluna[i] = jogo->matriz_atual[n-1-i][j];

        movimento_nao_guloso(coluna, n, &(jogo->pontuacao), &houve_movimento);

        // Restaura a coluna invertida
        for(int i = 0; i < n; i++)
            jogo->matriz_atual[n-1-i][j] = coluna[i];

        if(houve_movimento) movimento = 1;
    }
    return movimento;
}

// ==================== FUNÇÕES DE VERIFICAÇÃO ====================

// Função 18: Verifica se ainda há movimentos possíveis no tabuleiro
int ha_movimentos_possiveis(Jogo *jogo) {
    int n = jogo->tamanho_tabuleiro;
    
    // 1. Verifica se há espaços vazios 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (jogo->matriz_atual[i][j] == 0) {
                return 1; // Ainda há movimentos possíveis
            }
        }
    }
    
    // 2. Verifica se há possíveis fusões na horizontal
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (jogo->matriz_atual[i][j] == jogo->matriz_atual[i][j + 1]) {
                return 1; // Ainda há movimentos possíveis
            }
        }
    }
    
    // 3. Verifica se há possíveis fusões na vertical
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n - 1; i++) {
            if (jogo->matriz_atual[i][j] == jogo->matriz_atual[i + 1][j]) {
                return 1; // Ainda há movimentos possíveis
            }
        }
    }
    
    return 0; // Não há mais movimentos possíveis 
}

// Função 19: Verifica se o jogador perdeu 
int verificar_derrota(Jogo *jogo) {
    if (ha_movimentos_possiveis(jogo)) {
        return 2; // Retorna 2 indicando que ainda há movimentos possíveis
    }
    
    printf(CIANA "\n══════════════════════════════════════════════════\n" RESET);
    centralizar(VERMELHO NEGRITO "⚠️  GAME OVER! ⚠️" RESET, 50);
    centralizar(VERMELHO "Não há mais movimentos possíveis" RESET, 50);
    printf(CIANA "══════════════════════════════════════════════════\n" RESET);
    
    // Oferece chance de usar desfazer se disponível
    if (jogo->desfazer_disponiveis > 0) {
        printf(AMARELO "\n Deseja usar DESFAZER? Você tem %d chances. (S/N): " RESET, 
               jogo->desfazer_disponiveis);
        char opcao;
        int resultado = scanf(" %c", &opcao);
        limpar_buffer();
        
        // Se escolher desfazer, executa e continua
        if (resultado == 1 && toupper(opcao) == 'S') {
            desfazer_movimento(jogo);
            printf(AMARELO " Pressione ENTER para continuar..." RESET);
            getchar();
            return 2; // Retorna 2 indicando que usou desfazer
        }
    }
    
    printf(VERMELHO "\n Fim de jogo! Sua pontuação final: %d pontos 🏁\n" RESET, jogo->pontuacao);
    printf(AMARELO " Pressione ENTER para voltar ao menu..." RESET);
    getchar();
    
    return 0; // Retorna 0 indicando fim definitivo do jogo
}

// Função 20: Verifica se o jogador atingiu 2048 
int verificar_vitoria(Jogo *jogo) {
    int tamanho = jogo->tamanho_tabuleiro;
    
    // Percorre toda a matriz procurando o valor 2048
    for (int linha = 0; linha < tamanho; linha++) {
        for (int coluna = 0; coluna < tamanho; coluna++) {
            if (jogo->matriz_atual[linha][coluna] == 2048) {
                printf(CIANA "\n══════════════════════════════════════════════════\n" RESET);
                centralizar(AMARELO NEGRITO "🎉 PARABÉNS!" RESET, 50);
                centralizar(AMARELO NEGRITO "Você atingiu 2048! 🏆" RESET, 50);
                printf(CIANA "══════════════════════════════════════════════════\n" RESET);
                
                printf(VERDE "\n Deseja continuar jogando? (S/N): " RESET);
                
                char opcao;
                int resultado;
                do {
                    resultado = scanf(" %c", &opcao);
                    limpar_buffer();
                    opcao = toupper(opcao); 
                } while (opcao != 'S' && opcao != 'N' && resultado == 1);
                
                if (opcao == 'N') {
                    printf(AMARELO "\n Obrigado por jogar! 👋\n" RESET);
                    return 0; 
                }
                
                printf(VERDE "\n Continuando o jogo... 🎮\n" RESET);
                printf(AMARELO " Pressione ENTER para continuar..." RESET);
                getchar(); 

                return 1; // Retorna 1 indicando continuar jogo
            }
        }
    }
    return 2; // Retorna 2 indicando que não atingiu 
}

// Função 21: Função de verificação do jogo 
int verificar_jogo(Jogo *jogo) {
    int resultado_vitoria = verificar_vitoria(jogo);
    if (resultado_vitoria != 2) { // 2 = continua jogando
        return resultado_vitoria;
    }
    
    int resultado_derrota = verificar_derrota(jogo);
    return resultado_derrota;
}