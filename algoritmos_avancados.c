#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 10

// Estrutura do nó da árvore binária (sala)
typedef struct Sala {
    char nome[50];
    char *pista;  // Pista encontrada nesta sala (NULL se não houver)
    char *suspeito; // Suspeito relacionado à pista
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura do nó da árvore de busca (pistas)
typedef struct NoPista {
    char pista[100];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// Estrutura para a tabela hash (pista -> suspeito)
typedef struct NoHash {
    char pista[100];
    char suspeito[50];
    struct NoHash *proximo; // Para tratar colisões por encadeamento
} NoHash;

// Tabela hash global
NoHash *tabelaHash[TAMANHO_HASH];

// Função hash simples (soma ASCII mod tamanho)
int funcaoHash(char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAMANHO_HASH;
}

// Inicializa a tabela hash
void inicializarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

// Insere uma relação pista -> suspeito na tabela hash
void inserirNaHash(char *pista, char *suspeito) {
    int indice = funcaoHash(pista);

    // Cria novo nó
    NoHash *novo = (NoHash*)malloc(sizeof(NoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = NULL;

    // Insere no início da lista (encadeamento)
    if (tabelaHash[indice] == NULL) {
        tabelaHash[indice] = novo;
    } else {
        novo->proximo = tabelaHash[indice];
        tabelaHash[indice] = novo;
    }
}

// Busca o suspeito relacionado a uma pista
char* buscarSuspeitoNaHash(char *pista) {
    int indice = funcaoHash(pista);
    NoHash *atual = tabelaHash[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }

    return NULL;
}

// Conta ocorrências de cada suspeito
void contarSuspeitos(char suspeitos[][50], int contagens[], int *numSuspeitos) {
    *numSuspeitos = 0;

    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoHash *atual = tabelaHash[i];
        while (atual != NULL) {
            // Verifica se o suspeito já está na lista
            int encontrado = 0;
            for (int j = 0; j < *numSuspeitos; j++) {
                if (strcmp(suspeitos[j], atual->suspeito) == 0) {
                    contagens[j]++;
                    encontrado = 1;
                    break;
                }
            }

            // Se não foi encontrado, adiciona novo suspeito
            if (!encontrado) {
                strcpy(suspeitos[*numSuspeitos], atual->suspeito);
                contagens[*numSuspeitos] = 1;
                (*numSuspeitos)++;
            }

            atual = atual->proximo;
        }
    }
}

// Exibe todas as relações pista -> suspeito
void exibirRelacoes() {
    printf("\n--- RELAÇÕES PISTA → SUSPEITO ---\n");
    int encontrouAlguma = 0;

    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoHash *atual = tabelaHash[i];
        while (atual != NULL) {
            printf("  • %s → %s\n", atual->pista, atual->suspeito);
            encontrouAlguma = 1;
            atual = atual->proximo;
        }
    }

    if (!encontrouAlguma) {
        printf("  Nenhuma relação encontrada ainda.\n");
    }
}

// Funções da Árvore Binária (Mansão)
Sala* criarSala(char *nome, char *pista, char *suspeito) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);

    if (pista != NULL) {
        novaSala->pista = (char*)malloc(strlen(pista) + 1);
        strcpy(novaSala->pista, pista);
    } else {
        novaSala->pista = NULL;
    }

    if (suspeito != NULL) {
        novaSala->suspeito = (char*)malloc(strlen(suspeito) + 1);
        strcpy(novaSala->suspeito, suspeito);
    } else {
        novaSala->suspeito = NULL;
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
        char *suspeito = buscarSuspeitoNaHash(raiz->pista);
        if (suspeito != NULL) {
            printf("  - %s [Suspeito: %s]\n", raiz->pista, suspeito);
        } else {
            printf("  - %s\n", raiz->pista);
        }
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
            if (salaAtual->suspeito != NULL) {
                printf("🕵️  SUSPEITO RELACIONADO: %s\n", salaAtual->suspeito);
                inserirNaHash(salaAtual->pista, salaAtual->suspeito);
            }
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
        printf("  [r] - Ver relações pista → suspeito\n");
        printf("  [a] - Analisar suspeitos\n");
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
            case 'r':
            case 'R':
                exibirRelacoes();
                break;
            case 'a':
            case 'A':
                {
                    char suspeitos[10][50];
                    int contagens[10];
                    int numSuspeitos;

                    contarSuspeitos(suspeitos, contagens, &numSuspeitos);

                    if (numSuspeitos == 0) {
                        printf("\nNenhum suspeito identificado ainda.\n");
                    } else {
                        printf("\n--- ANÁLISE DE SUSPEITOS ---\n");
                        int maxContagem = 0;
                        int indiceCulpado = 0;

                        for (int i = 0; i < numSuspeitos; i++) {
                            printf("  • %s: %d pista(s)\n", suspeitos[i], contagens[i]);
                            if (contagens[i] > maxContagem) {
                                maxContagem = contagens[i];
                                indiceCulpado = i;
                            }
                        }

                        printf("\n🎯 PRINCIPAL SUSPEITO: %s (%d pistas)\n",
                               suspeitos[indiceCulpado], maxContagem);
                    }
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

// Função para liberar memória da árvore de salas
void liberarArvore(Sala *raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    if (raiz->pista != NULL) {
        free(raiz->pista);
    }
    if (raiz->suspeito != NULL) {
        free(raiz->suspeito);
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

// Função para liberar memória da tabela hash
void liberarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoHash *atual = tabelaHash[i];
        while (atual != NULL) {
            NoHash *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

int main() {
    printf("╔════════════════════════════════════════╗\n");
    printf("║   BEM-VINDO AO DETECTIVE QUEST!       ║\n");
    printf("║          Nível Mestre                 ║\n");
    printf("║   Desvende o Mistério da Mansão       ║\n");
    printf("╚════════════════════════════════════════╝\n\n");

    // Inicializa estruturas
    inicializarHash();
    NoPista *pistasEncontradas = NULL;

    // Construindo a árvore da mansão com pistas e suspeitos
    Sala *raiz = criarSala("Hall de Entrada", NULL, NULL);

    // Nível 1
    raiz->esquerda = criarSala("Biblioteca", "Livro com páginas rasgadas", "Professor Archibald");
    raiz->direita = criarSala("Sala de Estar", NULL, NULL);

    // Nível 2 - Esquerda (Biblioteca)
    raiz->esquerda->esquerda = criarSala("Sala de Leitura", "Marca de sangue no tapete", "Mordomo James");
    raiz->esquerda->direita = criarSala("Arquivo Secreto", "Documento confidencial", "Professor Archibald");

    // Nível 2 - Direita (Sala de Estar)
    raiz->direita->esquerda = criarSala("Cozinha", "Faca desaparecida", "Chef Marie");
    raiz->direita->direita = criarSala("Sala de Jantar", "Taça com veneno", "Lady Victoria");

    // Nível 3 - Cozinha
    raiz->direita->esquerda->esquerda = criarSala("Despensa", "Alimentos envenenados", "Chef Marie");
    raiz->direita->esquerda->direita = criarSala("Adega", "Garrafa de vinho vazia", "Mordomo James");

    // Nível 3 - Sala de Jantar
    raiz->direita->direita->esquerda = criarSala("Salão de Festas", "Bilhete anônimo", "Lady Victoria");
    raiz->direita->direita->direita = criarSala("Varanda", "Pegadas suspeitas", "Jardineiro Thomas");

    // Inicia a exploração
    explorarSalas(raiz, &pistasEncontradas);

    // Exibe relatório final
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      RELATÓRIO FINAL DA INVESTIGAÇÃO  ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("\nTotal de pistas encontradas: %d\n", contarPistas(pistasEncontradas));

    printf("\nPistas em ordem alfabética:\n");
    exibirPistasEmOrdem(pistasEncontradas);

    exibirRelacoes();

    // Análise final
    char suspeitos[10][50];
    int contagens[10];
    int numSuspeitos;

    contarSuspeitos(suspeitos, contagens, &numSuspeitos);

    if (numSuspeitos > 0) {
        printf("\n--- ANÁLISE FINAL DE SUSPEITOS ---\n");
        int maxContagem = 0;
        int indiceCulpado = 0;

        for (int i = 0; i < numSuspeitos; i++) {
            printf("  • %s: %d pista(s)\n", suspeitos[i], contagens[i]);
            if (contagens[i] > maxContagem) {
                maxContagem = contagens[i];
                indiceCulpado = i;
            }
        }

        printf("\n🎯 CULPADO IDENTIFICADO: %s\n", suspeitos[indiceCulpado]);
        printf("   Evidências encontradas: %d pistas\n", maxContagem);
    }

    // Libera memória
    liberarArvore(raiz);
    liberarPistas(pistasEncontradas);
    liberarHash();

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║    CASO RESOLVIDO! PARABÉNS!          ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\nObrigado por jogar Detective Quest!\n");

    return 0;
}

