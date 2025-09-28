#include <stdio.h>

int* runningSum( int*, int, int* );
void teste( const int*, int );
void PressioneEnter( void );

int main( void ) {
    int numsSize = 0, returnSize = 0;

    int case1[ 4 ] = { 1, 2, 3, 4 }, case2[ 5 ] = { 1, 1, 1, 1, 1 }, case3[ 5 ] = { 3, 1, 2, 10, 1 };

    int* nums = case1;
    numsSize = sizeof( case1 ) / sizeof( case1[ 0 ] );
    nums = runningSum( nums, numsSize, &returnSize );
    teste( nums, returnSize );

    nums = case2;
    numsSize = sizeof( case2 ) / sizeof( case2[ 0 ] );
    nums = runningSum( nums, numsSize, &returnSize );
    teste( nums, returnSize );

    nums = case3;
    numsSize = sizeof( case3 ) / sizeof( case3[ 0 ] );
    nums = runningSum( nums, numsSize, &returnSize );
    teste( nums, returnSize );

    return 0;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* runningSum( int* nums, int numsSize, int* returnSize ) {
    int somador = 0;

    *returnSize = numsSize;

    for ( int i = 0; i < numsSize; i++ ) {
        somador += nums[ i ];
        nums[ i ] = somador;
    }

    return nums;
}

void teste( const int* nums, const int returnSize ) {
    printf( "[" );
    for ( int i = 0; i < returnSize; i++ ) {
        printf( "%d", nums[ i ] );
        if ( i + 1 < returnSize ) {
            printf( ", " );
        }
    }
    printf( "]\n\n" );
}
