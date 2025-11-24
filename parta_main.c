#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

/**
 * Main program for process scheduling simulation.
 * Usage: ./parta_main fcfs <burst1> <burst2> ...
 *        ./parta_main rr <quantum> <burst1> <burst2> ...
 */
int main(int argc, char* argv[]) {
    // Check minimum arguments
    if (argc < 2) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
    
    char* algorithm = argv[1];
    
    // Check for FCFS
    if (strcmp(algorithm, "fcfs") == 0) {
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        
        // Parse burst times
        int num_processes = argc - 2;
        int* bursts = malloc(num_processes * sizeof(int));
        if (!bursts) {
            printf("ERROR: Memory allocation failed\n");
            return 1;
        }
        
        for (int i = 0; i < num_processes; i++) {
            bursts[i] = atoi(argv[i + 2]);
        }
        
        // Initialize processes and run FCFS
        struct pcb* procs = init_procs(bursts, num_processes);
        if (!procs) {
            free(bursts);
            printf("ERROR: Memory allocation failed\n");
            return 1;
        }
        
        printf("Using FCFS\n\n");
        
        // Print accepted processes
        for (int i = 0; i < num_processes; i++) {
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }
        
        // Run FCFS scheduler
        fcfs_run(procs, num_processes);
        
        // Calculate average wait time
        double total_wait = 0.0;
        for (int i = 0; i < num_processes; i++) {
            total_wait += procs[i].wait;
        }
        double avg_wait = total_wait / num_processes;
        
        printf("Average wait time: %.2f\n", avg_wait);
        
        // Clean up
        free(bursts);
        free(procs);
        
    } else if (strcmp(algorithm, "rr") == 0) {
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        
        // Parse quantum and burst times
        int quantum = atoi(argv[2]);
        int num_processes = argc - 3;
        int* bursts = malloc(num_processes * sizeof(int));
        if (!bursts) {
            printf("ERROR: Memory allocation failed\n");
            return 1;
        }
        
        for (int i = 0; i < num_processes; i++) {
            bursts[i] = atoi(argv[i + 3]);
        }
        
        // Initialize processes and run Round Robin
        struct pcb* procs = init_procs(bursts, num_processes);
        if (!procs) {
            free(bursts);
            printf("ERROR: Memory allocation failed\n");
            return 1;
        }
        
        printf("Using RR(%d)\n\n", quantum);
        
        // Print accepted processes
        for (int i = 0; i < num_processes; i++) {
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }
        
        // Run Round Robin scheduler
        rr_run(procs, num_processes, quantum);
        
        // Calculate average wait time
        double total_wait = 0.0;
        for (int i = 0; i < num_processes; i++) {
            total_wait += procs[i].wait;
        }
        double avg_wait = total_wait / num_processes;
        
        printf("Average wait time: %.2f\n", avg_wait);
        
        // Clean up
        free(bursts);
        free(procs);
        
    } else {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
    
    return 0;
}
