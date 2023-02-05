#include<stdlib.h>
#include<pthread.h>
#include <stdio.h>
# include<time.h>
#define Item(X, I, J, NCOL)      X[((J) + (I) * (NCOL))]

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */

void matmult(int* A, int* B, int* C, int l, int m, int n)
{
    for(int i=0; i<l; i++)
        for(int j=0; j<n; j++)
        {
            int sum = 0;
            for(int k=0; k<m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }
}

typedef struct
{
    int i;
    int j;
    int m;
    int n;
    int* A;
    int* B;
    int* C;
}aux;
void* calc1(void *arg)
{

    int sum=0;
    aux  *values =(aux *) arg;
    // a for loop to calculate the value of a specific row and col and storing it in C which is our result matrix
    for(int k=0; k<values->m; k++)
        sum += Item(values->A, values->i, k, values->m) * Item(values->B, k, values->j, values->n);
    Item(values->C, values->i, values->j, values->n) = sum;

}

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult_v1(int* A, int* B, int* C, int l, int m, int n)
{
    // initializing 2d array 0f threads
    pthread_t thread_id[l][n];
    // 2 nested loops to get the number of row and col and send it to the function to calculate the value of that element
    for(int i=0; i <l; i++)
    {
        for(int j=0; j<n;j++)
        {   // creating instance of struct and allocate storage for it
            aux *args;
            args = (aux *) malloc(sizeof(aux));
            args->A=A;
            args->B=B;
            args->C=C;
            args->m=m;
            args->n=n;
            args->i=i;
            args->j=j;
            // creating the thread and passing a function to handle the matrix multiplication with all of the matrices and every which row and col this element belongs to
            pthread_create( &thread_id[i][j], NULL, &calc1,(void *)args);
        }
    }
    // a for loop to wait until all threads complete their job to avoid function termination before any thread finishes
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < n; j++)
        {
            pthread_join(thread_id[i][j], NULL);
        }

    }
}



/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void* calc2(void *arg)
{
    // creating an instance of struct aux
    aux  *values =(aux *) arg;
    // a nested for loops to calc all the values of a specific row using it's index given as i in the above struct
    for(int j=0;j<values->n;j++)
    {
        int sum=0;
        for(int k=0; k<values->m; k++)
            sum += Item(values->A, values->i, k, values->m) * Item(values->B, k, j, values->n);
        Item(values->C, values->i, j, values->n) = sum;
    }
}
void matmult_v2(int* A, int* B, int* C, int l, int m, int n)
{
    // creating array of threads equal to the number of rows
    pthread_t thread_id[l];
    for(int i=0; i <l; i++)
    {
        // allocating memory for the struct and creating the thread by passing it's id and the function to execute and the args
            aux *args;
            args = (aux *) malloc(sizeof(aux));
            args->A=A;
            args->B=B;
            args->C=C;
            args->m=m;
            args->n=n;
            args->i=i;
            pthread_create( &thread_id[i], NULL, &calc2,(void *)args);
    }
    // waiting for all the threads to finish their work before moving the next part to avoid function termination before any threads finishing it's job
    for (int i = 0; i < l; i++)
    {
            pthread_join(thread_id[i], NULL);
    }
}
