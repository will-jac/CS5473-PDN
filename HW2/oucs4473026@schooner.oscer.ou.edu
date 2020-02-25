#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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

   int id, num_proc;
   int process_a = 0;
   int process_b = 1;
   ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
   ierr = MPI_Comm_rank(MPI_COMM_WORLD, &process_a);

   if (id == 0) {
      // root process

      // no error checking, so make sure to only pass in int's
      int n = atoi(argv[1]);
      // tell the other process the size
      ierr = MPI_Send(&n, 1, MPI_INT,
         process_b, send_data_tag, MPI_COMM_WORLD);
      // allocate an array of size 2^n
      int a[2^n];

      // pass this array back and forth 100 times
      for (int i = 0; i < 100; ++i) {
         ierr = MPI_Send(a, 2^n, MPI_INT,
            process_b, send_data_tag, MPI_COMM_WORLD);
         ierr = MPI_Recv(a, 2^n, MPI_INT,
            process_b, return_data_tag, MPI_COMM_WORLD, &status);
      }


   }
   else if (id == 1) {
      // recieve the array size and allocate it
      int n;
      ierr = MPI_Recv(&n, 1, MPI_INT, 0,
         return_data_tag, MPI_COMM_WORLD, &status);
      int a[2^n];

      // pass this array back and forth 100 times
      for (int i = 0; i < 100; ++i) {
         ierr = MPI_Recv(a, 2^n, MPI_INT,
            process_b, return_data_tag, MPI_COMM_WORLD, &status);
         ierr = MPI_Send(a, 2^n, MPI_INT,
            process_a, send_data_tag, MPI_COMM_WORLD);

      }
   }
   else {
      printf("more than two CPUs not recommended!\n");
   }
   // finish off
   ierr = MPI_Finalize();
}
