#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL || dyn_array_size(ready_queue) == 0) return false; //Check for bad data

    //Initialize
    result->average_waiting_time = 0.0;
    result->average_turnaround_time = 0.0;
    result->total_run_time = 0;
    unsigned long currTime = 0;

    //need to sort so the indexing during the for loop will be correct (this should be semi optimal size the function will still run of 0(N) as there are no embedded loops)
    //I'm assuming this is done by using the dyn_array_sort
    //but need to find or create a function to pass into it

    // Iterate through the processes in the pcbs and updates the result
    for (size_t i = 0; i < dyn_array_size(ready_queue); ++i) {

        //Grab queue at index i
        ProcessControlBlock_t *pcb_t = dyn_array_at(ready_queue, i);

        /****cant decide if this should be += or =  *****/
        result->average_waiting_time += currTime;

        // Calculate the total turnaround time for the current process (dividing later to find average)
        result->average_turnaround_time += currTime + pcb_t->remaining_burst_time;

        // Update current time to be the total run time
        currTime += pcb_t->remaining_burst_time;
        pcb_t->started = true;
    }

    // Calculate averages based on the total number of pcbs
    result->average_waiting_time /= dyn_array_size(ready_queue);
    result->average_turnaround_time /= dyn_array_size(ready_queue);

    return true;
    // UNUSED(ready_queue);
    // UNUSED(result);
    // return false;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    UNUSED(quantum);
    return false;
}


// typedef struct 
//     {
//         uint32_t remaining_burst_time;  // the remaining burst of the pcb
//         uint32_t priority;				// The priority of the task
//         uint32_t arrival;				// Time the process arrived in the ready queue
//         bool started;              	    // If it has been activated on virtual CPU
//     } 
//     ProcessControlBlock_t;		        // you may or may not need to add more elements

// Reads the PCB burst time values from the binary file into ProcessControlBlock_t remaining_burst_time field
// for N number of PCB burst time stored in the file.
// \param input_file the file containing the PCB burst times
// \return a populated dyn_array of ProcessControlBlocks if function ran successful else NULL for an error
dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    FILE *file = fopen(input_file, "rb");
    
    if (file == NULL)  return NULL;

    dyn_array_t *pcb_burst = (dyn_array_t *)malloc(sizeof(255));


    if (pcb_burst == NULL) {
        fclose(file);
        return NULL;
    }

    ProcessControlBlock_t pcb_t;

    while (fread(&pcb_t, sizeof(ProcessControlBlock_t), 1, file) == 1) {

    }

    return NULL;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
