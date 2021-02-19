/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#include "Stud.h"

struct pkt pkg_cpy;
struct msg msg_cpy;

int bin_num_send; //used for seq-ack-check, alters 0-1-0-1...etc
int bin_num_rec; //used for seq-ack-check, alters 0-1-0-1...etc


int make_check_num(char *string)
{
    int number;
    return number;
}
/* called from layer 5, passed the data to be sent to other side */
void A_output( struct msg message)
{
    struct pkt packet;

    /*create pkt with msg as payload */
    strcpy(packet.payload, message.data);
    packet.seqnum = bin_num_send;
    packet.acknum = bin_num_send;
    packet.checksum = make_check_num(message.data);

    pkg_cpy = packet;

    /*send pkt to lay 3 */
    tolayer3(1, packet);

    bin_num_send = !bin_num_send; //toggle
    

    /* A will not run again until it receives ack from B side OR timer runs out */
    
}

void B_output(struct msg message)  /* need be completed only for extra credit */
{}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{}


/* called when A's timer goes off */
void A_timerinterrupt()
{}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
    bin_num_send = 0;
    printf("A initiated...\n");
}


/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
    struct msg message;

    if(packet.seqnum =! bin_num_rec)
    {
        /*timeout*/
    }
    else
    {
        if(packet.checksum == make_check_num(packet.payload))
        {
            strcpy(message.data, packet.payload);
            tolayer5(1, message.data);

            /*send ack msg + do something with timer*/

            bin_num_rec = !bin_num_rec; //toggle
        }
    }


    
}

/* called when B's timer goes off */
void B_timerinterrupt()
{}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
    bin_num_rec = 0;
    printf("B initiated...\n");
}
