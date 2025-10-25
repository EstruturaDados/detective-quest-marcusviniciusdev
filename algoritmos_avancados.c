#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do nó da árvore binária (sala)
typedef struct Sala {
    char nome[50];
    char *pista;  // Pista encontrada nesta sala (NULL se não houver)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura do nó da árvore de busca (pistas)
typedef struct NoPista {
    char pista[100];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// Funções da Árvore Binária (Mansão)
Sala* criarSala(char *nome, char *pista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    if (pista != NULL) {
        novaSala->pista = (char*)malloc(strlen(pista) + 1);
        strcpy(novaSala->pista, pista);
    } else {
        novaSala->pista = NULL;
    }
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Funções da Árvore de Busca (Pistas)
NoPista* inserirPista(NoPista *raiz, char *pista) {
    if (raiz == NULL) {
        NoPista *novo = (NoPista*)malloc(sizeof(NoPista));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se for igual, não insere (evita duplicatas)

    return raiz;
}

// Busca uma pista na BST
int buscarPista(NoPista *raiz, char *pista) {
    if (raiz == NULL) {
        return 0; // Não encontrada
    }

    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0) {
        return 1; // Encontrada
    } else if (cmp < 0) {
        return buscarPista(raiz->esquerda, pista);
    } else {
        return buscarPista(raiz->direita, pista);
    }
}

// Exibe todas as pistas em ordem alfabética
void exibirPistasEmOrdem(NoPista *raiz) {
    if (raiz != NULL) {
        exibirPistasEmOrdem(raiz->esquerda);
        printf("  - %s\n", raiz->pista);
        exibirPistasEmOrdem(raiz->direita);
    }
}

// Conta o número de pistas
int contarPistas(NoPista *raiz) {
    if (raiz == NULL) return 0;
    return 1 + contarPistas(raiz->esquerda) + contarPistas(raiz->direita);
}

// Função para explorar as salas interativamente
void explorarSalas(Sala *salaAtual, NoPista **pistasEncontradas) {
    char opcao;
    int totalPistas = 0;

    while (salaAtual != NULL) {
        printf("\n================================\n");
        printf("Você está em: %s\n", salaAtual->nome);
        printf("================================\n");

        // Verifica se há pista nesta sala
        if (salaAtual->pista != NULL) {
            printf("\n🔍 PISTA ENCONTRADA: %s\n", salaAtual->pista);
            *pistasEncontradas = inserirPista(*pistasEncontradas, salaAtual->pista);
            totalPistas = contarPistas(*pistasEncontradas);
            printf("Total de pistas coletadas: %d\n", totalPistas);
        }

        // Verifica se é uma folha (fim do caminho)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\nFim do caminho! Não há mais salas para explorar.\n");
            break;
        }

        // Mostra opções disponíveis
        printf("\nOpções:\n");
        if (salaAtual->esquerda != NULL) {
            printf("  [e] - Ir para a esquerda (%s)\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf("  [d] - Ir para a direita (%s)\n", salaAtual->direita->nome);
        }
        printf("  [p] - Ver todas as pistas coletadas\n");
        printf("  [s] - Sair da exploração\n");
        printf("\nEscolha uma opção: ");
        scanf(" %c", &opcao);

        // Processa a escolha do jogador
        switch (opcao) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda;
                } else {
                    printf("Não há sala à esquerda!\n");
                }
                break;
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita;
                } else {
                    printf("Não há sala à direita!\n");
                }
                break;
            case 'p':
            case 'P':
                printf("\n--- PISTAS COLETADAS (em ordem alfabética) ---\n");
                if (*pistasEncontradas == NULL) {
                    printf("  Nenhuma pista encontrada ainda.\n");
                } else {
                    exibirPistasEmOrdem(*pistasEncontradas);
                }
                printf("Total: %d pistas\n", contarPistas(*pistasEncontradas));
                break;
            case 's':
            case 'S':
                printf("\nSaindo da exploração...\n");
                return;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// Função para liberar memória da árvore de salas
void liberarArvore(Sala *raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    if (raiz->pista != NULL) {
        free(raiz->pista);
    }
    free(raiz);
}

// Função para liberar memória da árvore de pistas
void liberarPistas(NoPista *raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

int main() {
    printf("╔════════════════════════════════════════╗\n");
    printf("║   BEM-VINDO AO DETECTIVE QUEST!       ║\n");
    printf("║       Nível Aventureiro               ║\n");
    printf("║   Explore e Colete Pistas             ║\n");
    printf("╚════════════════════════════════════════╝\n\n");

    // Árvore de pistas (BST)
    NoPista *pistasEncontradas = NULL;

    // Construindo a árvore da mansão com pistas
    Sala *raiz = criarSala("Hall de Entrada", NULL);

    // Nível 1
    raiz->esquerda = criarSala("Biblioteca", "Livro com páginas rasgadas");
    raiz->direita = criarSala("Sala de Estar", NULL);

    // Nível 2 - Esquerda (Biblioteca)
    raiz->esquerda->esquerda = criarSala("Sala de Leitura", "Marca de sangue no tapete");
    raiz->esquerda->direita = criarSala("Arquivo Secreto", "Documento confidencial");

    // Nível 2 - Direita (Sala de Estar)
    raiz->direita->esquerda = criarSala("Cozinha", "Faca desaparecida");
    raiz->direita->direita = criarSala("Sala de Jantar", "Taça com veneno");

    // Nível 3 - Cozinha
    raiz->direita->esquerda->esquerda = criarSala("Despensa", "Alimentos envenenados");
    raiz->direita->esquerda->direita = criarSala("Adega", "Garrafa de vinho vazia");

    // Nível 3 - Sala de Jantar
    raiz->direita->direita->esquerda = criarSala("Salão de Festas", "Bilhete anônimo");
    raiz->direita->direita->direita = criarSala("Varanda", "Pegadas suspeitas");

    // Inicia a exploração
    explorarSalas(raiz, &pistasEncontradas);

    // Exibe relatório final
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║         RELATÓRIO FINAL               ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\nTotal de pistas encontradas: %d\n", contarPistas(pistasEncontradas));
    printf("\nPistas em ordem alfabética:\n");
    exibirPistasEmOrdem(pistasEncontradas);

    // Libera memória
    liberarArvore(raiz);
    liberarPistas(pistasEncontradas);

    printf("\nObrigado por jogar Detective Quest!\n");

    return 0;
}

