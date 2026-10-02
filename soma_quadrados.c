/*
Alunos:
Pedro Montarroyos de Pinho RA: 10440213
Gustavo Kiyoshi Ikeda RA: 10439179
Felipe Marques Leite Marta RA: 10437877

*/


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int total_size = 40;
    int process_rank, num_processes;
    int *data = NULL;
    int *local_data;
    int local_square_sum = 0;
    int total_square_sum = 0;
    
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    
    int chunk_size = total_size/num_processes;
    
    if(process_rank == 0)
    {
        data = (int*)malloc(total_size * sizeof(int));
        for(int i = 0; i < total_size; i++)
        {
            data[i] = i + 1;    
        }
    }
    
    local_data = (int*)malloc(chunk_size * sizeof(int));
    MPI_Scatter(data, chunk_size, MPI_INT, local_data, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    printf("Processo %d recebeu:", process_rank);
    for (int i = 0; i < chunk_size; i++)
    {
        printf(" %d", local_data[i]);
    }    
    printf("\n");
    
    for(int i = 0; i < chunk_size; i++)
    {
        local_square_sum += (local_data[i] * local_data[i]);
    }
    
    printf("Processo %d: soma local dos quadrados = %d\n", process_rank, local_square_sum);
    
    MPI_Reduce(&local_square_sum, &total_square_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(process_rank == 0)
    {
        int sequential_sum = 0;
        for (int i = 1; i <= total_size; i++)
        {
            sequential_sum += i * i;
        }    
        
        printf("\nProcesso 0: soma paralela dos quadrados = %d\n", total_square_sum);
        printf("Processo 0: soma sequencial esperada    = %d\n", sequential_sum);
        
        if (total_square_sum == sequential_sum)
        {
            printf("Os valors conferem!\n");
        }
        else
        {
            printf("Os valores não conferem!\n");    
        }
    }
    free(data);
    free(local_data);
    MPI_Finalize();
    
    return 0;
}
