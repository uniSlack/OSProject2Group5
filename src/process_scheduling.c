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

// Runs the First Come First Served Process Scheduling algorithm over the incoming ready_queue
    // \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
    // \param result used for first come first served stat tracking \ref ScheduleResult_t
    // \return true if function ran successful else false for an error
bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
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

// Reads the PCB burst time values from the binary file into ProcessControlBlock_t remaining_burst_time field
// for N number of PCB burst time stored in the file.
// \param input_file the file containing the PCB burst times
// \return a populated dyn_array of ProcessControlBlocks if function ran successful else NULL for an error
dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    if(input_file){
        FILE* f = fopen(input_file, "r");
        if(f){
            int32_t numPCBs = 0;
            if(fread(numPCBs, sizeof(int32_t), 1, f) == 1) {                                // get num pcbs
                struct dyn_array_t* da = dyn_array_create(numPCBs, sizeof(int32_t), NULL);  // set up array
                for(int i = 0; i < numPCBs; i++){                                           // for each expected pcb...
                   ProcessControlBlock_t* pcb;
                   if(fread(pcb, sizeof(int32_t), 3, f) != 3) {                             // read in pcb to temp var
                        return NULL;                                                        // error clause
                   }    
                    dyn_array_push_front(da, pcb);                                          // else save
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
