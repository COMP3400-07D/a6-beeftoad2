#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Initializes an array of PCBs from burst times array.
 * Creates PCBs in heap memory with PID=index, burst_left from bursts array, wait=0.
 */
struct pcb* init_procs(int* bursts, int blen) {
    // Allocate memory for array of PCBs in the heap
    struct pcb* procs = malloc(blen * sizeof(struct pcb));
    if (procs == NULL) {
        return NULL;  // malloc failed
    }
    
    // Initialize each PCB
    for (int i = 0; i < blen; i++) {
        procs[i].pid = i;              // PID is the index
        procs[i].burst_left = bursts[i]; // Copy burst time
        procs[i].wait = 0;             // Initial wait is 0
    }
    
    return procs;
}

/**
 * Prints all PCBs showing their current state (PID, burst_left, wait).
 * Helper function for debugging.
 */
void printall(struct pcb* procs, int plen) {
    printf("PCB Status:\n");
    for (int i = 0; i < plen; i++) {
        printf("PID:%d burst_left:%d wait:%d\n", 
               procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
}

/**
 * Runs the current process for a specified amount of time.
 * Reduces the current process's burst_left by amount.
 * Increases the wait time for all other processes by the same amount.
 * Processes that are already completed (burst_left = 0) don't get additional wait time.
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    // Reduce the current process's burst_left by amount
    procs[current].burst_left -= amount;
    
    // Increase wait time for all other processes that are not completed
    for (int i = 0; i < plen; i++) {
        if (i != current && procs[i].burst_left > 0) {
            procs[i].wait += amount;
        }
    }
}

/**
 * First-Come-First-Serve scheduling algorithm.
 * Runs each process to completion in order (starting from PID 0).
 * Returns the total time elapsed when all processes are finished.
 */
int fcfs_run(struct pcb* procs, int plen) {
    int current_time = 0;
    
    // Run each process to completion in order
    for (int i = 0; i < plen; i++) {
        int burst_time = procs[i].burst_left;
        
        // Run the process to completion
        run_proc(procs, plen, i, burst_time);
        
        // Update current time
        current_time += burst_time;
    }
    
    return current_time;
}

/**
 * Round-robin helper function to find the next process to run.
 * Returns the next process that has burst_left > 0.
 * If all processes are complete (burst_left = 0), returns -1.
 * Always tries to move to the next process in round-robin order.
 * If the next process is done, keeps looking until finding an available one.
 * If only the current process has work left, returns current.
 */
int rr_next(int current, struct pcb* procs, int plen) {
    // Check if all processes are complete
    bool all_done = true;
    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left > 0) {
            all_done = false;
            break;
        }
    }
    if (all_done) {
        return -1;
    }
    
    // Try to find the next available process starting from the next position
    for (int offset = 1; offset <= plen; offset++) {
        int next = (current + offset) % plen;
        if (procs[next].burst_left > 0) {
            return next;
        }
    }
    
    // This should not happen if we checked all_done correctly above
    return -1;
}
/**
 * Round-robin scheduling algorithm with time quantum.
 * Each process runs for at most 'quantum' time units before switching.
 * Returns the total time elapsed when all processes are finished.
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    int current_time = 0;
    int current_process = 0;  // Start with process 0
    
    // Continue until all processes are done
    while (true) {
        // Check if current process exists and has work to do
        if (current_process >= 0 && procs[current_process].burst_left > 0) {
            // Determine how much time this process will run
            int run_time = (procs[current_process].burst_left < quantum) ? 
                          procs[current_process].burst_left : quantum;
            
            // Run the process
            run_proc(procs, plen, current_process, run_time);
            
            // Update total time
            current_time += run_time;
        }
        
        // Find next process to run
        current_process = rr_next(current_process, procs, plen);
        
        // If no next process, we're done
        if (current_process == -1) {
            break;
        }
    }
    
    return current_time;
}

