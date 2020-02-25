#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define send_data_tag 2001
#define return_data_tag 2002

int main(int argc, char **argv) {
   MPI_Status status;
   int ierr;

   ierr = MPI_Init(&argc, &argv);
   if (ierr != 0) {
      printf("Error detected, quitting!\n");
      exit(-1);
   }

   int root_id = 0;
   int id, num_proc;
   ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
   ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);

   if (id == root_id) {
      // root process

      // start a timer
      double wtime = MPI_Wtime();


      // send sub-problems to each worker process
      double n = pow(2, 16);
      double size = n / (num_proc - 1);

      for (int worker_id = 1; worker_id < num_proc; ++worker_id) {
         ierr = MPI_Send(&size, 1, MPI_DOUBLE,
            worker_id, send_data_tag, MPI_COMM_WORLD);
      }

      // recieve all the partial solutions and add them up
      int parital_count, count = 0;
      for (int worker_id = 1; worker_id < num_proc; ++worker_id) {
         ierr = MPI_Recv(&parital_count, 1, MPI_INT, MPI_ANY_SOURCE,
            return_data_tag, MPI_COMM_WORLD, &status);
         count += parital_count;
      }
      // print out the final sum
      printf("The value of pi yielded by the monte carlo simulation is: %f\n",
         4*count / n);
      wtime = MPI_Wtime() - wtime;
      printf("Time = %f\n", wtime);
   }
   else {
      // it's a worker process
      // do monte carlo sim
      int count = 0;
      double x, y, size;
      // set the seed
      srand(id);

      ierr = MPI_Recv( &size, 1, MPI_DOUBLE,
            root_id, send_data_tag, MPI_COMM_WORLD, &status);

      for (int i = 0; i < (int) size; ++i) {
         // get x, y in the range [0, 1]
         x = rand() / (double) RAND_MAX;
         y = rand() / (double) RAND_MAX;
         if (x*x + y*y <= 1)
            ++count;
      }

      // send the number in the circle back to the root
      ierr = MPI_Send( &count, 1, MPI_INT, root_id,
            return_data_tag, MPI_COMM_WORLD);
   }
   // finish off
   ierr = MPI_Finalize();
}
