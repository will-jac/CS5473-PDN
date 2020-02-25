
/*
 * greetings.c -- greetings program
 *
 * Send a message from all processes with rank != 0 to rank 0.
 *    Rank 0 prints the messages received.
 *
 * Input: none.
 * Output: contents of messages received by rank 0.
 *
 * See Parallel Programming with MPI, Pacheco 1996, chap. 3, pp. 41 & ff.
 *
 * Slightly modified by Henry Neeman.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int main (int argc, char* argv[])
{ /* main */

    const int maximum_message_length = 100;
    const int first_rank  = 0;       /* Ranks run 0..number_of_processes-1 */
    const int master_rank = 0;       /* Rank of master process             */
    const int tag         = 0;       /* Tag for messages                   */

    char       message[maximum_message_length]; /* Message string          */

    MPI_Status recv_status;          /* Return status for receive          */

    int        number_of_processes;  /* Number of processes in this run    */
    int        my_rank;              /* Rank of this process               */
    int        source;               /* Rank of sender                     */
    int        destination;          /* Rank of receiver                   */
    int        mpi_error_code;       /* Error code returned by MPI call    */

   /*
    * Start up MPI.
    */
    mpi_error_code =
        MPI_Init(&argc, &argv);

   /*
    * Query this MPI process's rank.
    */
    mpi_error_code =
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /*
    * Query the number of MPI processes in this run.
    */
    mpi_error_code =
        MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

   /*
    * What to do depends on which rank this is.
    *   Non-master rank: Create a message and send it to the master
    *     rank.
    *   Master rank: Loop over all the ranks, skipping itself,
    *     receive a message from each such rank, and print each
    *     such message.
    *
    * Note: Because the calls to MPI_Recv occur in rank order, the
    *   outputs will be in rank order -- which doesn't mean that
    *   the messages from the various ranks actually arrived at
    *   the master rank in that order.
    */
    if (my_rank != master_rank) {

       /*
        * Create the message for this rank to send.
        */
        sprintf(message, "Greetings from process %d!", my_rank);

       /*
        * Set the destination for this rank to send to.
        */
        destination = master_rank;

       /*
        * Send the message from this rank to the destination rank.
        *
        * Use length strlen + 1 so that the string terminator gets sent.
        * This is because MPI_Send/MPI_Recv treat the character string
        * as an array of characters, not as a string as such.
        */
        mpi_error_code =
            MPI_Send(message, strlen(message) + 1, MPI_CHAR,
                destination, tag, MPI_COMM_WORLD);

    } /* if (my_rank != master_rank) */

    else {

       /*
        * Loop over all the ranks.
        */
        for (source = first_rank; source < number_of_processes; source++) {

           /*
            * Skip the master rank, because it doesn't try to send a
            * message to itself.
            */
            if (source != master_rank) {

               /*
                * Receive the message on this rank (the master rank)
                * from the source rank.
                *
                * Use length maximum_message_length, which is the
                * maximum possible length of a string, including
                * its string terminator.
                */
                mpi_error_code =
                    MPI_Recv(message, maximum_message_length, MPI_CHAR,
                        MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &recv_status);

               /*
                * Output to stdout the message that was received.
                */
                printf("%s\n", message);

            } /* if (source != master_rank) */

        } /* for source */

    } /* if (my_rank != master_rank)...else */

   /*
    * Shut down MPI.
    */
    mpi_error_code =
        MPI_Finalize();

} /* main */

