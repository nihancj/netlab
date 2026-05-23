#include <stdio.h>
#include <unistdio.h>
#include <stdlib.h>
#include <time.h>

#define NANO_SEC 1000000000
#define ST "[SENDER]   "
#define RT "[RECEIVER] "

int n = 5; int nextFrame = 1;

_Bool is_lost() {
    return (rand() % 100) > 70;
}

void network(const char *msg, int dots, float delay, _Bool pf) {
    struct timespec ts;

    if (delay == 0.0) delay = 0.5;
    ts.tv_sec = (int)delay/(dots+1);
    ts.tv_nsec = ((long)(delay / (dots + 1) * NANO_SEC) - ts.tv_sec*NANO_SEC);

    printf("%s ", msg);
    if (pf) {
        printf("frame %d ", nextFrame);
    }
    fflush(stdout);
    nanosleep(&ts, NULL);

    for(int i=0; i<dots; ++i) {
        printf("..");
        fflush(stdout);
        nanosleep(&ts, NULL);
    }
    printf("\n\n");
}

int main() {
    srand(time(NULL));

    while (nextFrame <= n) {
        network(ST "Sending", 0, 1, 1);
        if (is_lost()) {
            network(RT "Waiting", 4, 4, 0);
            network(ST "Waiting", 4, 4, 0);
            network(ST "Timeout occured! Didn't recieve ACK for" , 0, 0, 1);
            continue;
        } else {
            network(RT "Recieved" , 0, 0, 1);
            network(RT "Sending ACK for" , 0, 0, 1);
        }

        if (is_lost()) {
            network(ST "Waiting", 2, 2, 0);
            network(ST "Timeout occured! Didn't recieve ACK for", 0, 0, 1);
            network(ST "Sending", 0, 1, 1);
            network(RT "Recieved" , 0, 0, 1);
            network(RT "Discarded. Sending ACK for" , 0, 0, 1);
        }
        network(ST "Recieved ACK for", 0, 1, 1);
        ++nextFrame;
    }


    return 0;
}
