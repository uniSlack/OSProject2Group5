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

    const char* pcb_file = argv[1];
    const char* schedule_algorithm = argv[2];

    // long quantum = 0;
    // if (argc > 3) 
    // {
    //     quantum = atol(argv[3]);
    // }

    dyn_array_t *pcb = load_process_control_blocks(pcb_file);
    ScheduleResult_t *result = malloc(sizeof(ScheduleResult_t));

    if (strncmp(schedule_algorithm, FCFS, 4) == 0) 
    {
        if (!first_come_first_serve(pcb, result)) printf("First-come-first-serve failed!\n");
    }
    else if (strncmp(schedule_algorithm, P, 4) == 0) 
    {
        printf("Priority is not implemented yet!\n");
    }
    else if (strncmp(schedule_algorithm, RR, 4) == 0) 
    {
        printf("Round Robin is not implemented yet!\n");
    }
    else if (strncmp(schedule_algorithm, SJF, 4) == 0) 
    {
        printf("Shortest Job First is not implemented yet!\n");
    }
    else 
    {
        printf("Invalid schedule algorithm!\n");
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("avg turnaround time: %f, avg waiting time: %f, total runtime: %lu\n", result->average_turnaround_time, result->average_waiting_time, result->total_run_time);

    free(result);

    return EXIT_SUCCESS;
}
