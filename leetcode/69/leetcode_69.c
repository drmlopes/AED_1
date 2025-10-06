#include <stdio.h>

int mySqrt( int );
void teste( int );

int main( void ) {
    teste( 0 );
    teste( 1 );
    teste( 4 );
    teste( 8 );
    teste( 144 );
    teste( 34444444 );
    teste( 175 );

    return 0;
}

int mySqrt( int x ) {
    if ( x == 0 || x == 1 ) {
        return x;
    } else {
        long retorno = x;
        while ( retorno * retorno > x ) {
            retorno = ( retorno + x / retorno ) / 2;
        }
        return (int)retorno;
    }
}

void teste( const int x ) {
    const int retorno = mySqrt( x );
    printf( "Input: %d\nOutput: %d\n\n", x, retorno );
}
