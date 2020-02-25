#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

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
   ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);

   if (id == process_a) {
      // root process

      // no error checking, so make sure to only pass in int's
      int n = atoi(argv[1]);
	printf("size = 2^%i\n", n);
      // tell the other process the size
      ierr = MPI_Send(&n, 1, MPI_INT,
         process_b, send_data_tag, MPI_COMM_WORLD);
      // allocate an array of size 2^n
      int size = (int) pow(2, (double) n);
      int a[size];

      // pass this array back and forth 100 times
     int i = 0; 
     double wtime = MPI_Wtime(); 
     for (i = 0; i < 100; ++i) {
         ierr = MPI_Send(a, size, MPI_INT,
            process_b, send_data_tag, MPI_COMM_WORLD);
         ierr = MPI_Recv(a, size, MPI_INT,
            process_b, return_data_tag, MPI_COMM_WORLD, &status);
      }
      wtime = MPI_Wtime() - wtime;
      printf("%f\n", wtime);

   }
   else if (id == process_b) {
      // recieve the array size and allocate it
      int n;
      ierr = MPI_Recv(&n, 1, MPI_INT, 0,
         send_data_tag, MPI_COMM_WORLD, &status);

      int size = (int) pow(2, (double) n);
      int a[size];

      int i = 0;
      for (i = 0; i < 100; ++i) {
         ierr = MPI_Recv(a, size, MPI_INT,
            process_a, send_data_tag, MPI_COMM_WORLD, &status);
         ierr = MPI_Send(a, size, MPI_INT,
            process_a, return_data_tag, MPI_COMM_WORLD);

      }
   }
   else {
      printf("more than two CPUs not recommended! (%i of %i)\n", id, num_proc);
   }
   // finish off
   ierr = MPI_Finalize();
}
