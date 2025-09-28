#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>

#define TAM 51

typedef enum {
    MENU_ADICIONAR = 1,
    MENU_REMOVER   = 2,
    MENU_LISTAR    = 3,
    MENU_SAIR      = 4
} menu_t;

typedef struct nomes_t {
    char* nome;
    struct nomes_t* proximo;
} nomes_t;

typedef struct {
    nomes_t* primeiro;
    nomes_t* ultimo;
} sentinela_t;

void Menu( void );
void MenuSelect( menu_t* );

void Adicionar( sentinela_t* );
void Remover( sentinela_t* );
void Listar( const sentinela_t* );

void LiberarLista( sentinela_t* );
void LimparBuffer( void );
void PressioneEnter( void );
void LimparTela( void );

int main( void ) {
    Menu();
    return 0;
}

/*!
 * @brief Menu principal.
 */
void Menu( void ) {
    menu_t op;
    sentinela_t* sentinela = (sentinela_t*)calloc( 1, sizeof( sentinela_t ) );
    if ( !sentinela ) {
        printf( "Erro - Não foi possível alocar memória dinâmicamente!\n\n" );
    } else {
        for ( ;; ) {
            LimparTela();
            MenuSelect( &op );
            LimparTela();

            switch ( op ) {
                case MENU_ADICIONAR:
                    Adicionar( sentinela );
                    break;

                case MENU_REMOVER:
                    Remover( sentinela );
                    break;

                case MENU_LISTAR:
                    Listar( sentinela );
                    break;

                case MENU_SAIR:
                    LiberarLista( sentinela );
                    free( sentinela );
                    return;

                default:
                    break;
            }
        }
    }
}

/*!
 * @brief Selecionar opções do menu principal.
 *
 * @param [in] op Ponteiro para opções do menu principal.
 */
void MenuSelect( menu_t* op ) {
    do {
        char opTemporario[ 3 ], *endptr;
        printf(
            ".:MENU:.\n\n%d - Adicionar\n%d - Remover\n%d - Listar\n%d - "
            "Sair\n\nEscolha: ",
            MENU_ADICIONAR, MENU_REMOVER, MENU_LISTAR, MENU_SAIR );
        fgets( opTemporario, sizeof( opTemporario ), stdin );
        opTemporario[ strcspn( opTemporario, "\n" ) ] = 0;

        if ( strlen( opTemporario ) >= 2 ) {
            LimparBuffer();
        }

        *op = (menu_t)strtol( opTemporario, &endptr, 10 );
        if ( *op < MENU_ADICIONAR or *op > MENU_SAIR ) {
            LimparTela();
            printf( "Erro - Opção Inválida! [%d]", *op );
            PressioneEnter();
            LimparTela();
        }
    } while ( *op < MENU_ADICIONAR or *op > MENU_SAIR );
}

/*!
 * @brief Adicionar nomes na lista encadeada.
 *
 * @param [in] sentinela Sentinela.
 */
void Adicionar( sentinela_t* sentinela ) {
    nomes_t* novo = (nomes_t*)calloc( 1, sizeof( nomes_t ) );
    if ( !novo ) {
        printf( "Erro - Não foi possível alocar memória dinâmicamente!" );
        PressioneEnter();
    } else {
        char nome[ TAM ];
        printf( "Informe um Nome: " );
        fgets( nome, sizeof( nome ), stdin );
        nome[ strcspn( nome, "\n" ) ] = 0;

        if ( strlen( nome ) >= TAM - 1 ) {
            LimparBuffer();
        }

        novo->nome = (char*)malloc( sizeof( char ) * ( strlen( nome ) + 1 ) );
        if ( !novo->nome ) {
            printf( "Erro - Não foi possivel alocar memória dinâmicamente!" );
            free( novo );
            PressioneEnter();
        } else {
            strcpy( novo->nome, nome );

            if ( !sentinela->primeiro ) {
                sentinela->primeiro = novo;
                sentinela->ultimo = novo;
            } else {
                sentinela->ultimo->proximo = novo;
                sentinela->ultimo = novo;
            }
        }
    }
}

/*!
 * @brief Remover nomes da lista encadeada.
 *
 * @param [in] sentinela Sentinela.
 */
void Remover( sentinela_t* sentinela ) {
    if ( !sentinela->primeiro ) {
        LimparTela();
        printf( "Lista Vazia!" );
        PressioneEnter();
    } else {
        char nome[ TAM ];
        printf( "Informe o Nome para Remover: " );
        fgets( nome, sizeof( nome ), stdin );
        nome[ strcspn( nome, "\n" ) ] = 0;

        if ( strlen( nome ) >= TAM - 1 ) {
            LimparBuffer();
        }

        nomes_t* auxiliar = sentinela->primeiro;
        nomes_t* anterior = nullptr;

        while ( auxiliar and strcmp( nome, auxiliar->nome ) not_eq 0 ) {
            anterior = auxiliar;
            auxiliar = auxiliar->proximo;
        }

        if ( !auxiliar ) {
            LimparTela();
            printf( "Erro - Nome não encontrado! [%s]", nome );
            PressioneEnter();
        } else {
            if ( !anterior ) {
                sentinela->primeiro = auxiliar->proximo;

                if ( !sentinela->primeiro ) {
                    sentinela->ultimo = sentinela->primeiro;
                }
            } else {
                anterior->proximo = auxiliar->proximo;

                if ( !anterior->proximo ) {
                    sentinela->ultimo = anterior;
                }
            }

            auxiliar->proximo = nullptr;
            free( auxiliar->nome );
            free( auxiliar );

            printf( "\n" );
            Listar( sentinela );
        }
    }
}

/*!
 * @brief Listar toda a lista encadeada.
 *
 * @param sentinela Sentinela.
 */
void Listar( const sentinela_t* sentinela ) {
    if ( !sentinela->primeiro ) {
        LimparTela();
        printf( "Lista Vazia!" );
        PressioneEnter();
    } else {
        size_t tamanho_total = 0;
        const nomes_t* auxiliar = sentinela->primeiro;

        while ( auxiliar ) {
            tamanho_total += strlen( auxiliar->nome );
            auxiliar = auxiliar->proximo;
        }

        char* texto = (char*)calloc( 1, sizeof( char ) * ( tamanho_total + 1 ) );
        if ( !texto ) {
            printf( "Erro - Não foi possivel alocar memória dinâmicamente!" );
            PressioneEnter();
        } else {
            auxiliar = sentinela->primeiro;

            while ( auxiliar ) {
                strcat( texto, auxiliar->nome );

                auxiliar = auxiliar->proximo;
            }

            printf( "%s", texto );
            PressioneEnter();
            free( texto );
        }
    }
}

/*!
 * @brief Liberar lista encadeada da heap.
 *
 * @param sentinela Sentinela.
 */
void LiberarLista( sentinela_t* sentinela ) {
    if ( sentinela->primeiro ) {
        nomes_t* liberar = sentinela->primeiro;

        while ( liberar ) {
            nomes_t* auxiliar = liberar->proximo;
            liberar->proximo = nullptr;
            free( liberar->nome );
            free( liberar );

            sentinela->primeiro = auxiliar;
            liberar = auxiliar;
        }

        sentinela->ultimo = nullptr;
    }
}

/*!
 * @brief Limpar buffer do teclado.
 */
void LimparBuffer( void ) {
    int c;
    while ( ( c = getchar() ) not_eq '\n' and c not_eq EOF );
}

/*!
 * @brief Pressionar ENTER para continuar.
 */
void PressioneEnter( void ) {
    printf( "\n\nPressione ENTER..." );
    getchar();
}

/*!
 * @brief Limpar terminal/prompt.
 */
void LimparTela( void ) {
    printf( "\x1b[2J\x1b[H" );
}
