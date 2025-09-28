#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 51

typedef enum {
    MENU_ADICIONAR = 1,
    MENU_REMOVER   = 2,
    MENU_LISTAR    = 3,
    MENU_SAIR      = 4
} menu_t;

void Menu( void );
void MenuSelect( menu_t* );

char* AdicionarNome( char* );
char* RemoverNome( char* );
void Listar( char* );

void PressioneEnter( void );
void LimparTela( void );

int main( void ) {
    Menu();
    return 0;
}

void Menu( void ) {
    menu_t op;
    char* nomes = nullptr;

    for ( ;; ) {
        LimparTela();
        MenuSelect( &op );
        LimparTela();

        switch ( op ) {
            case MENU_ADICIONAR:
                nomes = AdicionarNome( nomes );
                break;

            case MENU_REMOVER:
                nomes = RemoverNome( nomes );
                break;

            case MENU_LISTAR:
                Listar( nomes );
                break;

            case MENU_SAIR:
                free( nomes );
                return;

            default:
                break;
        }
    }
}

void MenuSelect( menu_t* op ) {
    do {
        char lerMenuSelect[ 3 ], *endptr;

        printf( ".:MENU:.\n\n%d - Adicionar\n%d - Remover\n%d - Listar\n%d - Sair\n\nEscolha: ", MENU_ADICIONAR,
                MENU_REMOVER, MENU_LISTAR, MENU_SAIR );
        char* opTemporario = fgets( lerMenuSelect, sizeof( lerMenuSelect ), stdin );
        opTemporario[ strcspn( opTemporario, "\n" ) ] = 0;

        *op = (menu_t)strtol( opTemporario, &endptr, 10 );

        if ( *op < MENU_ADICIONAR || *op > MENU_SAIR ) {
            LimparTela();
            printf( "Erro - Opção Inválida! [%d]", *op );
            PressioneEnter();
            LimparTela();
        }
    } while ( *op < MENU_ADICIONAR || *op > MENU_SAIR );
}

char* AdicionarNome( char* nomes ) {
    char lerAdicionarNome[ TAM ] = {};

    printf( "Informe o Nome para ser adicionado: " );
    char* nomeAdicionado = fgets( lerAdicionarNome, sizeof( lerAdicionarNome ), stdin );
    nomeAdicionado[ strcspn( nomeAdicionado, "\n" ) ] = 0;

    strcat( nomeAdicionado, ";" );

    const size_t tamanhoAtual = nomes
                                    ? strlen( nomes )
                                    : 0;
    size_t tamanhoNovo = strlen( nomeAdicionado );
    tamanhoNovo += tamanhoAtual + 1;

    char* temporario = (char*)realloc( nomes, tamanhoNovo );
    if ( !temporario ) {
        printf( "Erro - Não foi possivel alocar memória dinâmicamente!" );
        PressioneEnter();
    } else {
        if ( !nomes ) {
            strcpy( temporario, "\0" );
        }
        strcat( temporario, nomeAdicionado );

        return temporario;
    }

    return nomes;
}

char* RemoverNome( char* nomes ) {
    if ( !nomes ) {
        printf( "Erro - Nenhum nome foi adicionado!" );
        PressioneEnter();
    } else {
        char* auxiliar = (char*)calloc( 1, sizeof( char ) * ( strlen( nomes ) + 1 ) );
        if ( !auxiliar ) {
            printf( "Erro - Não foi possivel alocar memória dinâmicamente!" );
            PressioneEnter();
        } else {
            strcpy( auxiliar, nomes );

            char lerRemoverNome[ TAM ] = {};

            printf( "Informe o Nome para ser removido: " );
            char* nomeRemover = fgets( lerRemoverNome, sizeof( lerRemoverNome ), stdin );
            nomeRemover[ strcspn( nomeRemover, "\n" ) ] = 0;

            char* token = strtok( auxiliar, ";" );
            while ( token ) {
                if ( strcmp( token, nomeRemover ) == 0 ) {
                    strcat( nomeRemover, ";" );

                    char* localizado = strstr( nomes, nomeRemover );
                    if ( localizado ) {
                        const size_t tamanhoPalavra = strlen( nomeRemover );
                        const size_t tamanhoRestante = strlen( localizado + tamanhoPalavra );

                        memmove( localizado, localizado + tamanhoPalavra, tamanhoRestante + 1 );

                        if ( strlen( nomes ) == 0 ) {
                            free( auxiliar );
                            free( nomes );
                            return nullptr;
                        }

                        char* temporario = (char*)realloc( nomes, strlen( nomes ) + 1 );
                        if ( !temporario ) {
                            printf( "Erro - Não foi possivel alocar memória dinâmicamente!" );
                            PressioneEnter();
                        } else {
                            Listar( temporario );

                            free( auxiliar );
                            return temporario;
                        }
                    }
                }

                token = strtok( nullptr, ";" );
            }

            printf( "Erro - Nome não encontrado! [%s]", nomeRemover );
            PressioneEnter();
        }
        free( auxiliar );

        return nomes;
    }

    return nullptr;
}

void Listar( char* nomes ) {
    if ( !nomes ) {
        printf( "Erro - Nenhum nome foi adicionado!" );
    } else {
        printf( ".:NOMES:.\n\n%s", nomes );
    }
    PressioneEnter();
}

void PressioneEnter( void ) {
    printf( "\n\nPressione ENTER..." );
    getchar();
}

void LimparTela( void ) {
    printf( "\x1b[2J\x1b[H" );
}
