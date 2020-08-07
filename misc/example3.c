#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
   int rank, n_ranks, neighbour;
   int n_numbers = 10000;
   int *send_message;
   int *recv_message;
   MPI_Status status;

   send_message = malloc(n_numbers*sizeof(int));
   recv_message = malloc(n_numbers*sizeof(int));

   // First call MPI_Init
   MPI_Init(&argc, &argv);

   // Get my rank and the number of ranks
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);

   // Check that there are exactly two ranks
   if( n_ranks != 2 ){
        printf("This example requires exactly two ranks\n");
        MPI_Finalize();
        return(1);
   }

   // Call the other rank the neighbour
   if( rank == 0 ){
      neighbour = 1;      
   } else {
      neighbour = 0;
   }

   // Generate numbers to send
   for( int i=0; i<n_numbers; i++){
      send_message[i] = i;
   }

   if( rank == 0 ){
      // Rank 0 will send first
      MPI_Send(send_message, n_numbers, MPI_INT, 1, 0, MPI_COMM_WORLD);
   }

   if( rank == 1 ){
      // Rank 1 will receive it's message before sending
      MPI_Recv(recv_message, n_numbers, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      printf("Message received by rank %d \n", rank);
   }

   if( rank == 1 ){
      // Now rank 1 is free to send
      MPI_Send(send_message, n_numbers, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }

   if( rank == 0 ){
      // And rank 0 will receive the message
      MPI_Recv(recv_message, n_numbers, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
      printf("Message received by rank %d \n", rank);
   }
   free(send_message);
   free(recv_message);

   // Call finalize at the end
   return MPI_Finalize();
}

