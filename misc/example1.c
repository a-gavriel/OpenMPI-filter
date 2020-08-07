#include <stdio.h>
//#include <math.h>
#include <mpi.h>

int main(int argc, char** argv) {
 int rank, n_ranks, numbers_per_rank;
 int my_first, my_last;
 int numbers = 10;

 // First call MPI_Init
 MPI_Init(&argc, &argv);
 // Get my rank and the number of ranks
 MPI_Comm_rank(MPI_COMM_WORLD,&rank);
 MPI_Comm_size(MPI_COMM_WORLD,&n_ranks);

 // Calculate the number of iterations for each rank
 numbers_per_rank = numbers / n_ranks;
 if( numbers%n_ranks > 0 ){
    // Add 1 in case the number of ranks doesn't divide the number of numbers
    numbers_per_rank += 1;
 }

 // Figure out the first and the last iteration for this rank
 my_first = rank * numbers_per_rank;
 my_last = my_first + numbers_per_rank;

 // Run only the part of the loop this rank needs to run
 // The if statement makes sure we don't go over
 for( int i=my_first; i<my_last; i++ ) {
   if( i < numbers ) {
     printf("I'm rank %d and I'm printing the number %d.\n", rank, i);
   }
 }

 // Call finalize at the end
 return MPI_Finalize();
}

