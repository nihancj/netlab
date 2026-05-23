#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_FRAMES 5
#define TIMEOUT 2

// Simulates a network transmission that has a 30% chance of failing
int is_lost() {
    return (rand() % 100) < 30; 
}

int main() {
    int frame_to_send = 1;
    int ack_received;
    
    // Seed the random number generator for realistic network simulation
    srand(time(NULL));

    printf("--- Starting Stop-and-Wait ARQ Simulation ---\n\n");

    while (frame_to_send <= TOTAL_FRAMES) {
        printf("[SENDER] Sending Frame %d...\n", frame_to_send);
        sleep(1); // Simulate propagation delay

        // 1. Check if the frame itself gets lost
        if (is_lost()) {
            printf("[NETWORK] Alert: Frame %d was LOST in transit!\n", frame_to_send);
            printf("[SENDER] Timer running out... Timeout! Resending...\n\n");
            sleep(TIMEOUT);
            continue; // Skip to next iteration (retransmit)
        }

        printf("[RECEIVER] Frame %d received successfully.\n", frame_to_send);
        printf("[RECEIVER] Sending ACK %d back to sender...\n", frame_to_send);
        sleep(1);

        // 2. Check if the ACK gets lost
        if (is_lost()) {
            printf("[NETWORK] Alert: ACK %d was LOST in transit!\n", frame_to_send);
            printf("[SENDER] Timer running out... Timeout! Resending...\n\n");
            sleep(TIMEOUT);
            continue; // Skip to next iteration (retransmit)
        }

        // 3. Successful transmission and ACK arrival
        printf("[SENDER] ACK %d received! Preparing next frame.\n", frame_to_send);
        printf("--------------------------------------------------\n\n");
        frame_to_send++; // Move on to the next frame
    }

    printf("--- All %d frames sent and acknowledged successfully! ---\n", TOTAL_FRAMES);
    return 0;
}
