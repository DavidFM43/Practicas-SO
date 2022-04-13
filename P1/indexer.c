/*
 * Indexes the id_source
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ride.h"

void print_ride(struct Ride ride);

int main()
{

    FILE *bfp = fopen("rides.bin", "rb+");
    struct Ride ride;

    // this arrays contains
    long heads_id_source[1200]; // heads of the linked lists
    long tails_id_source[1200]; // tails of the linked lists
    long current_pos = 0;       // current file position

    // initalizes the heads and tails in -1
    for (int i = 0; i < 1200; i++)
    {
        tails_id_source[i] = -1;
        heads_id_source[i] = -1;
    }

    while (fread(&ride, sizeof(struct Ride), 1, bfp)) // reads one row at a time and puts it in a Ride struct
    {
        int current_id_source = ride.id_source;

        if (tails_id_source[current_id_source] == -1) // new linked list
        {
            tails_id_source[current_id_source] = current_pos;
            heads_id_source[current_id_source] = current_pos;
        }
        else // add one to linked list
        {
            struct Ride old_tail;                             // previous tail
            fsetpos(bfp, tails_id_source[current_id_source]); // moves to the infile position on the old tail
            fread(&old_tail, sizeof(struct Ride), 1, bfp);    // reads the structure
            old_tail.next_id_source = current_pos;

            fsetpos(bfp, -sizeof(struct Ride));               // moves the infile position back to where the old tail was located
            fwrite(&old_tail, sizeof(struct Ride), 1, bfp);   // write old tail
            tails_id_source[current_id_source] = current_pos; // put new tail in the tails array
            fsetpos(bfp, current_pos + sizeof(struct Ride));  // set infile position back were it was
        }

        current_pos = ftell(bfp); // change current pos
    }

    FILE *heads_fp = fopen("source_id_heads.bin", "wb");
    fwrite(&heads_id_source, sizeof(heads_id_source), 1, heads_fp);

    // close files
    fclose(heads_fp);
    fclose(bfp);
}

void print_ride(struct Ride ride)
{
    printf("Id_source: %d\n", ride.id_source);
    printf("Id_dest: %d\n", ride.id_dest);
    printf("Hour: %d\n", ride.hour);
    printf("Average time : %f\n", ride.avg_time);
    printf("next: %d\n\n", ride.next_id_source);
}