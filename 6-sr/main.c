#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_FRAMES 6
#define WINDOW_SIZE 3

typedef struct {
    int frame_num;
    int sent;
    int acked;
} Frame;

int is_lost() {
    // 25% chance of frame loss simulation
    return (rand() % 100) < 25; 
}

int main() {
    Frame sender_window[TOTAL_FRAMES + 1];
    int receiver_buffered[TOTAL_FRAMES + 1] = {0};
    
    int base = 1;
    int flag_lost_triggered = 0; // Ensures we simulate a realistic single loss event for demonstration
    
    srand(time(NULL));

    // Initialize frames
    for (int i = 1; i <= TOTAL_FRAMES; i++) {
        sender_window[i].frame_num = i;
        sender_window[i].sent = 0;
        sender_window[i].acked = 0;
    }

    printf("--- Starting Selective Repeat ARQ Simulation (Window Size = %d) ---\n\n", WINDOW_SIZE);

    while (base <= TOTAL_FRAMES) {
        
        // 1. SENDER: Transmit/Retransmit outstanding frames within the current window
        for (int i = base; i < base + WINDOW_SIZE && i <= TOTAL_FRAMES; i++) {
            if (!sender_window[i].sent || (!sender_window[i].acked && flag_lost_triggered)) {
                if (!sender_window[i].sent) {
                    printf("[SENDER] Sending Frame %d\n", i);
                } else {
                    printf("[SENDER] Timeout/NAK! *Selectively Retransmitting* Frame %d only\n", i);
                }
                sender_window[i].sent = 1;
                sleep(1);
            }
        }

        printf("\n[NETWORK] Processing window transmission status...\n");

        // 2. RECEIVER & NETWORK: Evaluate the status of frames in the window
        for (int i = base; i < base + WINDOW_SIZE && i <= TOTAL_FRAMES; i++) {
            if (sender_window[i].acked) continue; // Skip if already completed past loop

            // Simulate a loss specifically on Frame 2 to clearly demonstrate out-of-order buffering
            if (i == 2 && !flag_lost_triggered) {
                if (is_lost()) {
                    printf("[NETWORK] Alert: Frame %d was LOST!\n", i);
                    flag_lost_triggered = 1; 
                    continue; // Skip receiver processing for this frame
                }
            }

            // If it wasn't lost, receiver handles it
            if (i == base) {
                printf("[RECEIVER] Frame %d received in order. Sending ACK %d\n", i, i);
                sender_window[i].acked = 1;
                base++; // Slide window base forward because the expected frame arrived
            } else {
                // Out of order frame arrival! Buffer it instead of dropping it.
                if (!receiver_buffered[i]) {
                    printf("[RECEIVER] Frame %d received *OUT OF ORDER*. Buffering it. Sending ACK %d\n", i, i);
                    receiver_buffered[i] = 1;
                    sender_window[i].acked = 1;
                }
            }
        }
        
        // Check if any buffered frames can now help slide the window forward
        while (base <= TOTAL_FRAMES && receiver_buffered[base]) {
            printf("[RECEIVER] Pulling buffered Frame %d to application layer.\n", base);
            base++;
        }
        
        printf("--------------------------------------------------\n\n");
        sleep(1);
    }

    printf("--- All %d frames successfully transmitted using Selective Repeat! ---\n", TOTAL_FRAMES);
    return 0;
}
