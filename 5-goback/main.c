#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_FRAMES 6
#define WINDOW_SIZE 3

// Simulates a 25% chance of a frame getting lost or corrupted
int is_lost() {
    return (rand() % 100) < 25;
}

int main() {
    int base = 1;          // Oldest unacknowledged frame
    int next_seq_num = 1;  // Next frame to send
    int expected_frame = 1;// Frame the receiver is waiting for
    
    srand(time(NULL));

    printf("--- Starting Go-Back-N ARQ Simulation (Window Size = %d) ---\n\n", WINDOW_SIZE);

    while (base <= TOTAL_FRAMES) {
        
        // 1. SENDER: Send as many frames as the window allows
        while (next_seq_num < base + WINDOW_SIZE && next_seq_num <= TOTAL_FRAMES) {
            printf("[SENDER] Sending Frame %d\n", next_seq_num);
            next_seq_num++;
            sleep(1);
        }

        // 2. NETWORK: Simulate transmission of the oldest frame in the window (base)
        printf("\n[NETWORK] Checking status of Frame %d...\n", base);
        
        if (is_lost()) {
            // Disaster strike: Base frame is lost!
            printf("[NETWORK] Alert: Frame %d was LOST!\n", base);
            printf("[SENDER] Timeout handled! Rolling back to Frame %d...\n", base);
            printf("--------------------------------------------------\n\n");
            
            // The core of Go-Back-N: Reset next_seq_num back to the base to retransmit the entire window
            next_seq_num = base; 
            sleep(1);
        } else {
            // Success: Base frame arrived safely
            printf("[RECEIVER] Frame %d received in order. Sending ACK %d\n", base, base);
            printf("[SENDER] ACK %d received! Sliding window forward.\n", base);
            printf("--------------------------------------------------\n\n");
            
            base++; // Slide the window boundary forward
        }
    }

    printf("--- All %d frames successfully transmitted using Go-Back-N! ---\n", TOTAL_FRAMES);
    return 0;
}
