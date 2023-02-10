#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mpi.h>

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int send_data = world_size;
    int recv_data;

    // Broadcasting data from the root process to all other processes
    MPI_Bcast(&send_data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received broadcast data: %d\n", world_rank, send_data);

    // Scattering data from the root process to all other processes
    int send_data_array[4];
    for (int i = 0; i < world_size; i++) {
        send_data_array[i] = i;
    }
    MPI_Scatter(send_data_array, 1, MPI_INT, &recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received scattered data: %d\n", world_rank, recv_data);

    // Gathering data from all processes to the root process
    int recv_data_array[4];
    MPI_Gather(&send_data, 1, MPI_INT, recv_data_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (world_rank == 0) {
        printf("Process %d received gathered data:", world_rank);
        for (int i = 0; i < world_size; i++) {
            printf("%d,", recv_data_array[i]);
        }
        printf("\n");
    }

    // Reducing data from all processes to the root process
    int sum;
    MPI_Reduce(&send_data, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (world_rank == 0) {
        printf("Process %d received reduced data: %d\n", world_rank, sum);
    }
    printf("\n");

    MPI_Finalize();
}
