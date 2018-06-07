/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int block_i, block_j, i, j;
    int diag, tmp, a3, a4, a5, a6, a7, a8;
    if(M==64){
        for(block_i=0; block_i<64; block_i +=4){
            for(block_j=0; block_j<64; block_j +=4){
                diag = A[block_i][block_j];
                tmp = A[block_i][block_j+1];
                a3 = A[block_i][block_j+2];
                a4 = A[block_i][block_j+3];
                a5 = A[block_i+1][block_j];
                a6 = A[block_i+1][block_j+1];
                a7 = A[block_i+1][block_j+2];
                a8 = A[block_i+1][block_j+3];
                B[block_j][block_i] = diag;
                B[block_j][block_i+1] = a5;
                B[block_j][block_i+2] = tmp;
                B[block_j][block_i+3] = a6;
                B[block_j+1][block_i] = a3;
                B[block_j+1][block_i+1] = a7;
                B[block_j+1][block_i+2] = a4;
                B[block_j+1][block_i+3] = a8;

                diag = A[block_i+2][block_j];
                tmp = A[block_i+2][block_j+1];
                a3 = A[block_i+2][block_j+2];
                a4 = A[block_i+2][block_j+3];
                a5 = A[block_i+3][block_j];
                a6 = A[block_i+3][block_j+1];
                a7 = A[block_i+3][block_j+2];
                a8 = A[block_i+3][block_j+3];
                B[block_j+2][block_i] = diag;
                B[block_j+2][block_i+1] = a5;
                B[block_j+2][block_i+2] = tmp;
                B[block_j+2][block_i+3] = a6;
                B[block_j+3][block_i] = a3;
                B[block_j+3][block_i+1] = a7;
                B[block_j+3][block_i+2] = a4;
                B[block_j+3][block_i+3] = a8;

                diag = B[block_j+1][block_i];
                tmp = B[block_j+1][block_i+1];
                a3 = B[block_j+1][block_i+2];
                a4 = B[block_j+1][block_i+3];
                a5 = B[block_j+2][block_i];
                a6 = B[block_j+2][block_i+1];
                a7 = B[block_j+2][block_i+2];
                a8 = B[block_j+2][block_i+3];
                i = B[block_j+3][block_i];
                j = B[block_j+3][block_i+1];

                B[block_j+1][block_i] = B[block_j][block_i+2];
                B[block_j+1][block_i+1] = B[block_j][block_i+3];
                B[block_j][block_i+2] = a5;
                B[block_j][block_i+3] = a6;

                B[block_j+1][block_i+2] = a7;
                B[block_j+1][block_i+3] = a8;
                B[block_j+2][block_i] = diag;
                B[block_j+2][block_i+1] = tmp;
                B[block_j+2][block_i+2] = i;
                B[block_j+2][block_i+3] = j;
                B[block_j+3][block_i] = a3;
                B[block_j+3][block_i+1] = a4;
                
                
            }
        } 
    }
    else if(M==32){
        for(block_i=0; block_i<32; block_i +=8){
            for(block_j=0; block_j<32; block_j +=8){
                for(i=block_i; i<block_i+8; i++){
                    for(j=block_j; j<block_j+8; j++){
                        if(i!=j)
                            B[j][i] = A[i][j];
                        else{
                            tmp = A[i][i];
                            diag = i;
                        }
                    }
                    if(block_i==block_j)
                        B[diag][diag] = tmp;							
                }
            }
        } 
    }
    else{
        for(block_i=0; block_i<N; block_i +=8){
            for(block_j=0; block_j<M; block_j +=8){
                for(i=block_i; i<block_i+8 && i<N; i++){
                    for(j=block_j; j<block_j+8 && j<M; j++){
                        if(i!=j)
                            B[j][i] = A[i][j];
                        else{
                            tmp = A[i][i];
                            diag = i;
                        }
                    }
                    if(block_i==block_j)
                        B[diag][diag] = tmp;							
                }
            }
        } 
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

