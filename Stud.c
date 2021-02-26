/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#include "Stud.h"

struct pkt pkg_cpy;
struct msg msg_cpy;

int bin_num_send; //used for seq-ack-check, alters 0-1-0-1...etc
int bin_num_send_cpy;
int expected_seqnum_reciver; //used for seq-ack-check, alters 0-1-0-1...etc
int expected_seqnum_reciver_cpy;

int A_transmissionstate;

int timeontimer = 1000;

int make_check_num(struct pkt package)
{

    int checksum = 0;

    checksum = package.acknum + package.seqnum;

    for (int i = 0; i < 20; i++)
    {
        checksum += package.payload[i];
    }

    return checksum;
}
/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{

    if (A_transmissionstate != recived)
    {
        printf("messeage is already beeing sent\n");
        return;
    }

    struct pkt packet;

    /*create pkt with msg as payload */
    strcpy(packet.payload, message.data);
    packet.seqnum = bin_num_send;
    packet.acknum = bin_num_send; // acnum sak nog sättas dit av reciver och inte sender
    packet.checksum = make_check_num(packet);

    pkg_cpy = packet;
    bin_num_send_cpy = bin_num_send;

    A_transmissionstate = sending;

    /*send pkt to lay 3 */
    tolayer3(B, packet);
    starttimer(A, timeontimer);
    printf("Pkt %d sent\n", packet.acknum);

    /* A will not run again until it receives ack from B side OR timer runs out */
}

void B_output(struct msg message) /* need be completed only for extra credit */
{
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
    //printf("got\n");

    if (A_transmissionstate != sending)
    {
        printf("messeage is already recived\n");
        return;
    }

    stoptimer(A);
    printf("Pkt %d recieved\n", packet.acknum);

    if (bin_num_send_cpy == packet.acknum)
    {
        bin_num_send = !bin_num_send; //toggle
        A_transmissionstate = recived;
        //printf("got ack\n");
    }
    else
    {
        printf("wrong ack\n");
    }
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
    stoptimer(A);
    tolayer3(B, pkg_cpy);
    starttimer(A, timeontimer);
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
    bin_num_send = 0;
    A_transmissionstate = recived;
    printf("A initiated...\n");
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
    struct msg message;

    if (packet.checksum == make_check_num(packet))
    {
        if (packet.seqnum == expected_seqnum_reciver)
        {
            strcpy(message.data, packet.payload);
            tolayer5(B, message.data);

            packet.acknum = expected_seqnum_reciver;

            tolayer3(A, packet);

            /*send ack msg */

            expected_seqnum_reciver = !expected_seqnum_reciver; //toggle
        }
        else
        {
            printf("worng packet expected\n");
            packet.acknum = expected_seqnum_reciver;
            tolayer3(A, packet);
        }
    }
    else
    {
        printf("packet corupted\n");
    }
}

/* called when B's timer goes off */
void B_timerinterrupt()
{
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
    expected_seqnum_reciver = 0;
    printf("B initiated...\n");
}
