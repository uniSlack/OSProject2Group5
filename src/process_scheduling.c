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

/// Comparator function for sorting pcbs based on arrival time
int compare_pcb_arrival(const void *a, const void *b) {
    const ProcessControlBlock_t *pcb_first = (const ProcessControlBlock_t *)a; //cast void pointers
    const ProcessControlBlock_t *pcb_second = (const ProcessControlBlock_t *)b;

    if (pcb_first->arrival > pcb_second->arrival) {    //pcb1 arrives after pcb2
        return 1;
    }
     else {         //pcb2 arrives after or at the same time as pcb1 (sort not needed) 
        return -1;
    } 
}

// Runs the First Come First Served Process Scheduling algorithm over the incoming ready_queue
    // \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
    // \param result used for first come first served stat tracking \ref ScheduleResult_t
    // \return true if function ran successful else false for an error
bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL || dyn_array_size(ready_queue) == 0) return false; //Check for bad data

    result->average_waiting_time = 0.0;
    result->average_turnaround_time = 0.0;
    result->total_run_time = 0;
    unsigned long currTime = 0;

    //need to test
    //compares arrive time of pcbs and does a quick sort using that function
    if (!dyn_array_sort(ready_queue, compare_pcb_arrival))  return false;

    // Iterate through the processes in the pcbs and updates the result
    for (size_t i = 0; i < dyn_array_size(ready_queue); ++i) {

        ProcessControlBlock_t *pcb_t = dyn_array_at(ready_queue, i); //Grab queue at index i

        /****cant decide if this should be += or =  *****/
        result->average_waiting_time += currTime;

        result->average_turnaround_time += currTime + pcb_t->remaining_burst_time; // Calculates the total turnaround time for the current process

        currTime += pcb_t->remaining_burst_time;    // Update current time to be the total run time

        pcb_t->started = true;
    }

    // Calculate averages based on the total number of pcbs
    result->average_waiting_time /= dyn_array_size(ready_queue);
    result->average_turnaround_time /= dyn_array_size(ready_queue);

    return true;
}

    // Runs the Shortest Job First Scheduling algorithm over the incoming ready_queue
    // \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
    // \param result used for shortest job first stat tracking \ref ScheduleResult_t
    // \return true if function ran successful else false for an error
bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

// Runs the Priority algorithm over the incoming ready_queue
// \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
// \param result used for shortest job first stat tracking \ref ScheduleResult_t
// \return true if function ran successful else false for an error
bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

// Runs the Round Robin Process Scheduling algorithm over the incoming ready_queue
// \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
// \param result used for round robin stat tracking \ref ScheduleResult_t
// \param the quantum
// \return true if function ran successful else false for an error
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
    if(input_file){
        FILE* f = fopen(input_file, "rb");
        if(f){
            int32_t numPCBs = 0;
            if(fread(&numPCBs, sizeof(int32_t), 1, f) == 1) {                                   // get num pcbs
                dyn_array_t* da = dyn_array_create(numPCBs, sizeof(int32_t), NULL);             // set up array
                for(int i = 0; i < numPCBs; i++){                                               // for each expected pcb...
                    ProcessControlBlock_t* pcb = malloc(sizeof(ProcessControlBlock_t));         // Create heap var
                    if(fread(pcb, sizeof(int32_t), 3, f) != 3) {                                // read in pcb to heap var
                        free(pcb);
                        return NULL;                                                            // error clause
                    }    
                    dyn_array_push_front(da, pcb);                                              // else save
                }
                return da;
            }
        }
    }
    return NULL;
}

// Runs the Shortest Remaining Time First Process Scheduling algorithm over the incoming ready_queue
// \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
// \param result used for shortest job first stat tracking \ref ScheduleResult_t
// \return true if function ran successful else false for an error
bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
