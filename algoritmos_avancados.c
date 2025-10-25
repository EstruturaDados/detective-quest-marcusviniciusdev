#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do nó da árvore binária (sala)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Função para criar uma nova sala
Sala* criarSala(char *nome) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para explorar as salas interativamente
void explorarSalas(Sala *salaAtual) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\n================================\n");
        printf("Você está em: %s\n", salaAtual->nome);
        printf("================================\n");

        // Verifica se é uma folha (fim do caminho)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Fim do caminho! Não há mais salas para explorar.\n");
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
            case 's':
            case 'S':
                printf("\nSaindo da exploração...\n");
                return;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// Função para liberar memória da árvore
void liberarArvore(Sala *raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

int main() {
    printf("╔════════════════════════════════════════╗\n");
    printf("║   BEM-VINDO AO DETECTIVE QUEST!       ║\n");
    printf("║        Nível Novato                   ║\n");
    printf("║   Explore a Mansão Misteriosa         ║\n");
    printf("╚════════════════════════════════════════╝\n\n");

    // Construindo a árvore da mansão
    Sala *raiz = criarSala("Hall de Entrada");

    // Nível 1
    raiz->esquerda = criarSala("Biblioteca");
    raiz->direita = criarSala("Sala de Estar");

    // Nível 2 - Esquerda (Biblioteca)
    raiz->esquerda->esquerda = criarSala("Sala de Leitura");
    raiz->esquerda->direita = criarSala("Arquivo Secreto");

    // Nível 2 - Direita (Sala de Estar)
    raiz->direita->esquerda = criarSala("Cozinha");
    raiz->direita->direita = criarSala("Sala de Jantar");

    // Nível 3 - Cozinha
    raiz->direita->esquerda->esquerda = criarSala("Despensa");
    raiz->direita->esquerda->direita = criarSala("Adega");

    // Nível 3 - Sala de Jantar
    raiz->direita->direita->esquerda = criarSala("Salão de Festas");
    raiz->direita->direita->direita = criarSala("Varanda");

    // Inicia a exploração
    explorarSalas(raiz);

    // Libera memória
    liberarArvore(raiz);

    printf("\nObrigado por jogar Detective Quest!\n");

    return 0;
}

