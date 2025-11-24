#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Initializes an array of PCBs from burst times array.
 * Creates PCBs in heap memory with PID=index, burst_left from bursts array, wait=0.
 */
struct pcb* init_procs(int* bursts, int blen) {
    struct pcb* procs = malloc(blen * sizeof(struct pcb));
    if (procs == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < blen; i++) {
        procs[i].pid = i;         
        procs[i].burst_left = bursts[i]; 
        procs[i].wait = 0;             
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
    
    procs[current].burst_left -= amount;
    
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
    
    for (int i = 0; i < plen; i++) {
        int burst_time = procs[i].burst_left;
        
        run_proc(procs, plen, i, burst_time);
        
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
    
    for (int offset = 1; offset <= plen; offset++) {
        int next = (current + offset) % plen;
        if (procs[next].burst_left > 0) {
            return next;
        }
    }
    
    return -1;
}
/**
 * Round-robin scheduling algorithm with time quantum.
 * Each process runs for at most 'quantum' time units before switching.
 * Returns the total time elapsed when all processes are finished.
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    int current_time = 0;
    int current_process = 0; 
    
    while (true) {
        if (current_process >= 0 && procs[current_process].burst_left > 0) {
            int run_time = (procs[current_process].burst_left < quantum) ? 
                          procs[current_process].burst_left : quantum;
            run_proc(procs, plen, current_process, run_time);
            current_time += run_time;
        }
        
        current_process = rr_next(current_process, procs, plen);
        
        if (current_process == -1) {
            break;
        }
    }
    
    return current_time;
}

