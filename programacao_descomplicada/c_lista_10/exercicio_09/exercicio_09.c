#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PressioneEnter( void );

int main( void ) {
    size_t tamanho = 0;
    float valor;
    char lerValor[ 16 ], *endptr;

    float* vetor = (float*)malloc( sizeof( float ) );

    do {
        printf( "Informe um Valor: " );
        if ( !fgets( lerValor, sizeof( lerValor ), stdin ) ) {
            printf( "Erro - Ocorreu um erro na leitura!" );
            PressioneEnter();
            valor = -1;
        } else {
            lerValor[ strcspn( lerValor, "\n" ) ] = 0;

            valor = strtof( lerValor, &endptr );

            if ( *endptr != '\0' || strlen( lerValor ) == 0 ) {
                printf( "Erro - Valor inválido!" );
                PressioneEnter();
                continue;
            }

            if ( valor >= 0 ) {
                float* novoVetor = (float*)realloc( vetor, sizeof( float ) * ( tamanho + 1 ) );

                vetor = novoVetor;
                vetor[ tamanho ] = valor;
                tamanho++;

                printf( "\nValor %.2f adicionado. Total de elementos: %zu\n", (double)valor, tamanho );

                // Mostra todos os valores até agora
                printf( "Valores no vetor: " );
                for ( size_t i = 0; i < tamanho; i++ ) {
                    printf( "%.2f ", (double)vetor[ i ] );
                }
                PressioneEnter();
            }
        }
    } while ( valor >= 0 );

    if ( vetor ) {
        printf( "\nTamanho do vetor: %zu\nValores no vetor: ", tamanho );
        for ( size_t i = 0; i < tamanho; i++ ) {
            printf( "%.2f ", (double)vetor[ i ] );
        }

        free( vetor );
        vetor = nullptr;
    }

    return 0;
}

void PressioneEnter( void ) {
    printf( "\n\nPressione ENTER..." );
    while ( getchar() != '\n' ); // Limpa o buffer
}
