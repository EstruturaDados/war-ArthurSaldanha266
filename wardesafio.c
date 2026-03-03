#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
#define TOTAL_TERRITORIOS 6
#define TOTAL_MISSOES 4
#define MAX_NOME 30

// --- Estrutura de Dados ---
typedef struct {
    char nome[MAX_NOME];
    char cor[15];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---

// Setup e Memória
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa, char* missao);

// Interface e Lógica
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(const char* missao);
void atribuirMissao(char** destino);
void faseDeAtaque(Territorio* mapa, const char* corJogador);
void simularAtaque(Territorio* atacante, Territorio* defensor);

// Utilitários e Vitória
int verificarVitoria(const char* missao, const Territorio* mapa, const char* corJogador);
void limparBufferEntrada();

// --- Função Principal ---
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    // 1. Configuração Inicial
    Territorio* mapa = alocarMapa();
    if (!mapa) return 1;

    char* missaoJogador = NULL;
    char* minhaCor = "Verde";

    inicializarTerritorios(mapa);
    atribuirMissao(&missaoJogador);

    int opcao;
    int venceu = 0;

    // 2. Game Loop
    do {
        system("cls || clear");
        printf("=== WAR ESTRATEGICO - COMANDO GERAL ===\n");
        exibirMissao(missaoJogador);
        exibirMapa(mapa);
        exibirMenuPrincipal();

        printf("\nEscolha sua acao: ");
        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            continue;
        }

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, minhaCor);
                break;
            case 2:
                venceu = verificarVitoria(missaoJogador, mapa, minhaCor);
                if (venceu) {
                    printf("\n[!] ESTRATEGIA CONCLUIDA! Voce dominou o campo de batalha.\n");
                    opcao = 0; 
                } else {
                    printf("\n[i] A missao ainda nao foi atingida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("\nRetirando tropas... Ate a proxima general!\n");
                break;
            default:
                printf("\nComando invalido!\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            limparBufferEntrada();
            getchar();
        }

    } while (opcao != 0);

    // 3. Limpeza
    liberarMemoria(mapa, missaoJogador);
    return 0;
}

// --- Implementação das Funções ---

Territorio* alocarMapa() {
    return (Territorio*) calloc(TOTAL_TERRITORIOS, sizeof(Territorio));
}

void inicializarTerritorios(Territorio* mapa) {
    char* nomes[] = {"Brasil", "USA", "China", "Russia", "Egito", "Franca"};
    char* cores[] = {"Verde", "Azul", "Vermelho", "Azul", "Vermelho", "Verde"};
    
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[i]);
        mapa[i].tropas = (rand() % 5) + 2; 
    }
}

void atribuirMissao(char** destino) {
    char* banco[] = {
        "Dominar 3 territorios quaisquer",
        "Eliminar a presenca Vermelha do mapa",
        "Conquistar a Russia e o Egito",
        "Acumular um total de 15 tropas"
    };
    int sorteio = rand() % TOTAL_MISSOES;
    *destino = (char*) malloc((strlen(banco[sorteio]) + 1) * sizeof(char));
    strcpy(*destino, banco[sorteio]);
}

void exibirMapa(const Territorio* mapa) {
    printf("\n%-4s | %-15s | %-10s | %-6s\n", "ID", "TERRITORIO", "OCUPANTE", "TROPAS");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        printf("%-4d | %-15s | %-10s | %-6d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void exibirMissao(const char* missao) {
    printf("OBJETIVO ATUAL: %s\n", missao);
    printf("--------------------------------------------------\n");
}

void exibirMenuPrincipal() {
    printf("\n[1] Lancar Ataque\n");
    printf("[2] Reivindicar Vitoria\n");
    printf("[0] Sair do Jogo\n");
}

void faseDeAtaque(Territorio* mapa, const char* corJogador) {
    int idAtk, idDef;
    printf("\nID do seu territorio atacante: ");
    scanf("%d", &idAtk);
    printf("ID do territorio alvo: ");
    scanf("%d", &idDef);

    if (idAtk < 0 || idAtk >= TOTAL_TERRITORIOS || idDef < 0 || idDef >= TOTAL_TERRITORIOS) {
        printf("Territorio inexistente!\n");
        return;
    }

    if (strcmp(mapa[idAtk].cor, corJogador) != 0) {
        printf("Voce so pode atacar com seus proprios territorios!\n");
    } else if (strcmp(mapa[idDef].cor, corJogador) == 0) {
        printf("Voce nao pode atacar a si mesmo!\n");
    } else if (mapa[idAtk].tropas < 2) {
        printf("Tropas insuficientes para avancar (minimo 2)!\n");
    } else {
        simularAtaque(&mapa[idAtk], &mapa[idDef]);
    }
}

void simularAtaque(Territorio* atacante, Territorio* defensor) {
    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("\nCOMBATE: %s (%d) VS %s (%d)\n", atacante->nome, dadoAtk, defensor->nome, dadoDef);

    if (dadoAtk > dadoDef) {
        printf("Vitoria! O territorio %s foi conquistado.\n", defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = 1; 
        atacante->tropas--; 
    } else {
        printf("Derrota! O inimigo resistiu.\n");
        atacante->tropas--;
    }
}

int verificarVitoria(const char* missao, const Territorio* mapa, const char* corJogador) {
    int contagem = 0;
    int totalTropas = 0;

    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            contagem++;
            totalTropas += mapa[i].tropas;
        }
    }

    if (strstr(missao, "3 territorios") && contagem >= 3) return 1;
    if (strstr(missao, "15 tropas") && totalTropas >= 15) return 1;
    
    // Verificação de território específico (Ex: Russia e Egito)
    if (strstr(missao, "Russia e o Egito")) {
        int r = 0, e = 0;
        for(int i=0; i<TOTAL_TERRITORIOS; i++) {
            if(strcmp(mapa[i].nome, "Russia") == 0 && strcmp(mapa[i].cor, corJogador) == 0) r = 1;
            if(strcmp(mapa[i].nome, "Egito") == 0 && strcmp(mapa[i].cor, corJogador) == 0) e = 1;
        }
        if (r && e) return 1;
    }

    return 0;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void liberarMemoria(Territorio* mapa, char* missao) {
    free(mapa);
    free(missao);
}