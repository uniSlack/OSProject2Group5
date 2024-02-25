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

/// Comparator function for sorting pcbs based on remaining burst time
// For SRTF
int compare_burst_time(const void *a, const void *b) {
    const ProcessControlBlock_t *pcb_first = (const ProcessControlBlock_t *)a; //cast void pointers
    const ProcessControlBlock_t *pcb_second = (const ProcessControlBlock_t *)b;

    if (pcb_first->remaining_burst_time > pcb_second->remaining_burst_time) {
        return 1;
    }
     else {
        return -1;
    } 
}

// Runs the First Come First Served Process Scheduling algorithm over the incoming ready_queue
    // \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
    // \param result used for first come first served stat tracking \ref ScheduleResult_t
    // \return true if function ran successful else false for an error
bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL || dyn_array_empty(ready_queue)) return false; //Check for bad data

    result->average_waiting_time = 0;
    result->average_turnaround_time = 0;
    result->total_run_time = 0;
    unsigned long current_time = 0;

    //compares arrive time of pcbs and does a quick sort using that function
    if (!dyn_array_sort(ready_queue, compare_pcb_arrival))  return false;

    // Iterate through the processes in the pcbs and updates the result
    for (size_t i = 0; i < dyn_array_size(ready_queue); i++) {
        ProcessControlBlock_t *pcb_t = dyn_array_at(ready_queue, i); //Grab queue at index i

        result->average_waiting_time += current_time;
        result->average_turnaround_time += current_time + pcb_t->remaining_burst_time; // Calculates the total turnaround time for the current process
        
        current_time += pcb_t->remaining_burst_time;    // Update current time to be the total run time
        pcb_t->started = true;
    }

    // Calculate averages based on the total number of pcbs
    result->average_waiting_time /= dyn_array_size(ready_queue);
    result->average_turnaround_time /= dyn_array_size(ready_queue);
    result->total_run_time = current_time;

    return true;
}

    // Runs the Shortest Job First Scheduling algorithm over the incoming ready_queue
    // \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
    // \param result used for shortest job first stat tracking \ref ScheduleResult_t
    // \return true if function ran successful else false for an error
bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL || dyn_array_empty(ready_queue)) return false; //Check for bad data

    result->average_waiting_time = 0;
    result->average_turnaround_time = 0;
    result->total_run_time = 0;
    unsigned long current_time = 0;

    //compares arrive time of pcbs and does a quick sort using that function
    if (!dyn_array_sort(ready_queue, compare_burst_time))  return false;

    // Iterate through the processes in the pcbs and updates the result
    for (size_t i = 0; i < dyn_array_size(ready_queue); i++) {
        ProcessControlBlock_t *pcb_t = dyn_array_at(ready_queue, i); //Grab queue at index i

        result->average_waiting_time += current_time;
        result->average_turnaround_time += current_time + pcb_t->remaining_burst_time; // Calculates the total turnaround time for the current process
        
        current_time += pcb_t->remaining_burst_time;    // Update current time to be the total run time
        pcb_t->started = true;
    }

    // Calculate averages based on the total number of pcbs
    result->average_waiting_time /= dyn_array_size(ready_queue);
    result->average_turnaround_time /= dyn_array_size(ready_queue);
    result->total_run_time = current_time;

    return true;
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
    if(!(ready_queue && quantum > 0)) {return false;}   // check params

    result->average_turnaround_time = (float)0;
    result->average_waiting_time = (float)0;
    result->total_run_time = 0;                 // initializes results
    
    int initial_size = dyn_array_size(ready_queue);

    if(initial_size <= 0) {return false;}       // Checks there are pcbs to run

    while(dyn_array_size(ready_queue) > 0){
        ProcessControlBlock_t *pcb = malloc(sizeof(ProcessControlBlock_t));
        if(!dyn_array_extract_front(ready_queue, pcb)) { // pop first index
            free(pcb);
            return false;
        }    

        if(pcb->started == false) {
            pcb->started = true;
            result->average_waiting_time += result->total_run_time;     // if first run, make active and update wait time
        }

        if(pcb->remaining_burst_time > quantum){  
                     
            pcb->remaining_burst_time -= quantum;       // run pcb
            
            result->total_run_time += quantum;          // account for run time

            if (!dyn_array_push_back(ready_queue, pcb)) { // push to back of array
                free(pcb);
                return false;
            }     
        }
        else {
            result->total_run_time += pcb->remaining_burst_time;        // account for last bit of run time
            pcb->remaining_burst_time = 0;                              // finish process
            result->average_turnaround_time += result->total_run_time;  // update turnaround time once finished
        }
        free(pcb);
    }

    result->average_turnaround_time /= (float)initial_size;
    result->average_waiting_time /= (float)initial_size;   // get averages from totals
    return true;
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
    if(input_file == NULL) return NULL;

    FILE* f = fopen(input_file, "rb");
    if(f == NULL) return NULL;

    uint32_t numPCBs = 0;
    if (fread(&numPCBs, sizeof(uint32_t), 1, f) != 1) // get num pcbs
    {
        // Reading in number failed...
        fclose(f);
        return NULL;
    }

    dyn_array_t* da = dyn_array_create(numPCBs, sizeof(ProcessControlBlock_t), NULL);             // set up array
    if (da == NULL) 
    {
        // array setup failed...
        fclose(f);
        return NULL;
    }
    
    for(uint32_t i = 0; i < numPCBs; i++)   // for each expected pcb...
    {      
        uint32_t buffer[3];

        if(fread(buffer, sizeof(uint32_t), 3, f) != 3)  // read in pcb to heap var
        {
            fclose(f);
            dyn_array_destroy(da);
            return NULL;                                                            // error clause
        }    

        ProcessControlBlock_t* pcb = malloc(sizeof(ProcessControlBlock_t));         // Create heap var

        pcb->remaining_burst_time = buffer[0];
        pcb->priority = buffer[1];
        pcb->arrival = buffer[2];
        pcb->started = false;

        dyn_array_push_back(da, pcb);                                              // save
    }

    fclose(f);
    return da;
}

void find_waiting_time(dyn_array_t *ready_queue, int *waiting_time, unsigned long *run_time)
{
    // https://www.geeksforgeeks.org/shortest-remaining-time-first-preemptive-sjf-scheduling-algorithm/
    // SRTF is confusing...

    size_t size = dyn_array_size(ready_queue);
    unsigned long remaining_times[size];
    
    // copy burst times
    for (size_t i = 0; i < size; i++)
    {
        ProcessControlBlock_t *pcb_t = dyn_array_at(ready_queue, i);
        remaining_times[i] = pcb_t->remaining_burst_time;
    }

    size_t completed = 0;
    unsigned long time = 0;
    unsigned long minm = 10000000;
    unsigned long shortest = 0;
    unsigned long finish_time = 0;
    int check = 0;

    while (completed != size)
    {
        for (size_t i = 0; i < size; i++)
        {
            ProcessControlBlock_t *pcb_t = dyn_array_at(ready_queue, i);
            if (pcb_t->arrival <= time && remaining_times[i] < minm && remaining_times[i] > 0)
            {
                minm = remaining_times[i];
                shortest = i;
                check = 1;
            }
        } // end for loop

        if (check == 0)
        {
            time++;
            continue;
        }

        // Reduce remaining time by one
        remaining_times[shortest] = remaining_times[shortest] - 1;

        // Update minimum
        minm = remaining_times[shortest];
        if (minm == 0)
        {
            minm = 10000000;
        }

        // Process is completely executed
        if (remaining_times[shortest] == 0)
        {
            completed++;
            check = 0;

            // Find finish time
            finish_time = time + 1;

            // Calculate waiting time
            ProcessControlBlock_t *shortest_pcb = dyn_array_at(ready_queue, shortest);
            waiting_time[shortest] = finish_time - shortest_pcb->remaining_burst_time - shortest_pcb->arrival;

            if (waiting_time[shortest] < 0)
            {
                waiting_time[shortest] = 0;
            }
        }

        time++;
    }

    *run_time = time;
}

// Runs the Shortest Remaining Time First Process Scheduling algorithm over the incoming ready_queue
// \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
// \param result used for shortest job first stat tracking \ref ScheduleResult_t
// \return true if function ran successful else false for an error
bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL || dyn_array_empty(ready_queue)) return false; //Check for bad data

    result->average_waiting_time = 0;
    result->average_turnaround_time = 0;
    result->total_run_time = 0;

    size_t size = dyn_array_size(ready_queue);
    int waiting_time[size];
    find_waiting_time(ready_queue, waiting_time, &(result->total_run_time));

    for (size_t i = 0; i < size; i++)
    {
        ProcessControlBlock_t *pcb_t = dyn_array_at(ready_queue, i);

        result->average_waiting_time += (unsigned long)waiting_time[i];
        result->average_turnaround_time += pcb_t->remaining_burst_time + (unsigned long)waiting_time[i];

        pcb_t->started = true;
    }

    result->average_waiting_time /= size;
    result->average_turnaround_time /= size;
    
    return true;
}
