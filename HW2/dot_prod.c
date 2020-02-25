#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define send_data_tag 2001
#define return_data_tag 2002

int A[] = {1, 3, 5, 7, 9, 11, 13, 15};
int B[] = {2, 4, 6, 8, 10, 12, 14, 16};

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
   ierr = MPI_Comm_rank(MPI_COMM_WORLD, &root_id);

   if (id == root_id) {
      // root process

      // send sub-problems to each worker process
      int len = LENGTH(A);
      int sub_problem_size = len / num_proc;
      int start, end, size = 0;

      for (int worker_id = 1; worker_id < num_proc; ++worker_id) {
         start = (worker_id-1)*sub_problem_size;
         end = worker_id*sub_problem_size;
         if (end > LENGTH(A))
            end = LENGTH(A);
         size = end - start;

         ierr = MPI_Send(&size, 1, MPI_INT,
            worker_id, send_data_tag, MPI_COMM_WORLD);
         ierr = MPI_Send(&A[start], size, MPI_INT,
            worker_id, send_data_tag, MPI_COMM_WORLD);
         ierr = MPI_Send(&B[start], size, MPI_INT,
            worker_id, send_data_tag, MPI_COMM_WORLD);
      }

      // recieve all the partial solutions and add them up
      int dot_product = 0;
      int partial_dot = 0;
      for (int worker_id = 1; worker_id < num_proc; ++worker_id) {
         ierr = MPI_Recv(&partial_dot, 1, MPI_INT, MPI_ANY_SOURCE,
            return_data_tag, MPI_COMM_WORLD, &status);
         dot_product += partial_dot;
      }
      // print out the final sum
      printf("The dot product of A and B is %i\n", dot_product);
   }
   else {
      // it's a worker process

      // recieve the partial arrays
      int size;
      int* a;
      int* b;
      int partial_dot;

      ierr = MPI_Recv( &size, 1, MPI_INT,
            root_id, send_data_tag, MPI_COMM_WORLD, &status);
      ierr = MPI_Recv( &a, size, MPI_INT,
            root_id, send_data_tag, MPI_COMM_WORLD, &status);
      ierr = MPI_Recv( &b, size, MPI_INT,
            root_id, send_data_tag, MPI_COMM_WORLD, &status);

      // Calculate the dot product of the partial arrays
      partial_dot = 0;
      for(int i = 0; i < size; i++) {
         partial_dot += a[i]*b[i];
      }

      // send the partial dot product back to the root
      ierr = MPI_Send( &partial_dot, 1, MPI_INT, root_id,
            return_data_tag, MPI_COMM_WORLD);
   }
   // finish off
   ierr = MPI_Finalize();
}
