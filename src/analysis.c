#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);

        return EXIT_FAILURE;
    }

    
    dyn_array_t* da = load_process_control_blocks(argv[1]);
    if(!da) {abort();} // replace with better bug catch

    // float avg_turnaround, sum_turnaround;
    float avg_wait, sum_wait;
    // unsigned long total_clock_time; // huh? Built in func maybe?

    for(size_t i = 0; i < dyn_array_size(da); i++){
        ProcessControlBlock_t* pcb = dyn_array_at(da, i);
        if(!pcb) {abort();} // replace with better bug catch
        sum_wait += pcb->remaining_burst_time;
    }

    avg_wait = sum_wait / dyn_array_size(da);
    printf("Average wait: %f", avg_wait);

    // abort();  // replace me with implementation.

    return EXIT_SUCCESS;
}
