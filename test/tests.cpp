#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"

// Using a C library requires extern "C" to prevent function managling
extern "C" 
{
#include <dyn_array.h>
}

TEST (load_process_control_blocks, NullInputFilename) {
    const char *input_filename = NULL;
    dyn_array_t* res = load_process_control_blocks(input_filename);
    dyn_array_t* temp = NULL;
    EXPECT_EQ(res, temp);
}

TEST (load_process_control_blocks, GoodRead) {
    const char *input_filename = "testData.bin";
    FILE* f = fopen(input_filename, "wb");

    uint32_t numPCBs = 5;
    ProcessControlBlock_t pcb[numPCBs]; 

    uint32_t arr[1] = {numPCBs};
    size_t wroteOut = fwrite(arr, sizeof(int32_t), 1, f);     

    for(uint32_t i = 0; i < numPCBs; i++){
        pcb[i].remaining_burst_time = i;
        pcb[i].priority = i;
        pcb[i].arrival = i;
        pcb[i].started = false;
        uint32_t outArray[3] = {pcb[i].remaining_burst_time, pcb[i].priority, pcb[i].arrival };
        wroteOut += fwrite(outArray, sizeof(uint32_t), 3, f); 
    }

    fclose(f);

    dyn_array_t* res = load_process_control_blocks(input_filename);

    EXPECT_EQ(wroteOut, 1 + (numPCBs * 3));
    EXPECT_EQ(dyn_array_empty(res), false);
    EXPECT_EQ(dyn_array_size(res), numPCBs);
    for(uint32_t i = 0; i < numPCBs; i++){
        void *temp = dyn_array_at(res, i);
        EXPECT_FALSE(temp == NULL);
        ProcessControlBlock_t *tester = (ProcessControlBlock_t*)temp;
        EXPECT_EQ(tester->remaining_burst_time, pcb[i].remaining_burst_time);
        EXPECT_EQ(tester->priority, pcb[i].priority);
        EXPECT_EQ(tester->arrival, pcb[i].arrival);
        EXPECT_EQ(tester->started, pcb[i].started);
    }
    EXPECT_FALSE(res == NULL);
}

TEST (load_process_control_blocks, pcbbinHasExpectedValues) {
    const char *input_filename = "pcb.bin";

    dyn_array* res = load_process_control_blocks(input_filename);
    dyn_array_t* temp = NULL;
    ASSERT_NE(res, temp);

    EXPECT_EQ(dyn_array_size(res), 4);

    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 0))->remaining_burst_time, 15);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 0))->priority, 0);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 0))->arrival, 0);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 0))->started, false);

    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 1))->remaining_burst_time, 10);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 1))->priority, 0);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 1))->arrival, 1);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 1))->started, false);

    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 2))->remaining_burst_time, 5);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 2))->priority, 0);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 2))->arrival, 2);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 2))->started, false);

    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 3))->remaining_burst_time, 20);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 3))->priority, 0);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 3))->arrival, 3);
    EXPECT_EQ(((ProcessControlBlock_t*)dyn_array_at(res, 3))->started, false);

    dyn_array_destroy(res);
}

TEST (first_come_first_serve, LoadAndSort) {
    const char *input_filename = "testData.bin";
    FILE* f = fopen(input_filename, "wb");

    int numPCBs = 10;
    ProcessControlBlock_t pcb[numPCBs]; 

    int32_t arr[1] = {numPCBs};
    size_t wroteOut = fwrite(arr, sizeof(int32_t), 1, f);     

    for(int i = 0; i < numPCBs; i++){
        pcb[i].arrival = (i%3==0) ? i+2 : i;
        pcb[i].priority = i;
        pcb[i].remaining_burst_time = i;
        pcb[i].started = false;
    }

    wroteOut += fwrite(pcb, sizeof(ProcessControlBlock_t), numPCBs, f); // writes out array of pcbs
    fclose(f);

    dyn_array_t* pcbs = load_process_control_blocks(input_filename);
    ScheduleResult_t * FCFS_result = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));  

    //sort and get results from FCFS
    bool validFCFS = first_come_first_serve(pcbs, FCFS_result);

    int totalTime = ((ProcessControlBlock_t *)dyn_array_at(pcbs,0))->remaining_burst_time; // Initialize time since we dont add 0

    //loop through the pcbs to make sure they are sorted and get total time
    for(int i = 1; i < numPCBs; i++){
        ProcessControlBlock_t *pcb_at_index = (ProcessControlBlock_t *)dyn_array_at(pcbs,i);
        ProcessControlBlock_t *pcb_at_prev_index = (ProcessControlBlock_t *)dyn_array_at(pcbs,i-1);
        totalTime += pcb_at_index->remaining_burst_time;

        //If sorted worked then all arrival times should go from lowest -> highest
        EXPECT_GE(pcb_at_index->arrival, pcb_at_prev_index->arrival);
    }

    EXPECT_TRUE(validFCFS);
    EXPECT_EQ(FCFS_result->total_run_time, totalTime);
    EXPECT_EQ(wroteOut, 1 + numPCBs);
    EXPECT_EQ(dyn_array_empty(pcbs), false);
    EXPECT_FALSE(pcbs == NULL);
}

TEST (first_come_first_serve, NULL_queue) {
    ScheduleResult_t * FCFS_result = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));  
    bool validFCFS = first_come_first_serve(NULL, FCFS_result);
    EXPECT_FALSE(validFCFS);
}

TEST (shortest_job_first, NULL_queue) {
    ScheduleResult_t *SJF_result = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));
    bool validSJF = shortest_job_first(NULL, SJF_result);
    EXPECT_FALSE(validSJF);

    free(SJF_result);
}

TEST (shortest_job_first, LoadAndSort) {
    const char *input_filename = "testData.bin";
    FILE* f = fopen(input_filename, "wb");

    int numPCBs = 10;
    ProcessControlBlock_t pcb[numPCBs]; 

    int32_t arr[1] = {numPCBs};
    size_t wroteOut = fwrite(arr, sizeof(int32_t), 1, f);     

    for(int i = 0; i < numPCBs; i++){
        pcb[i].arrival = i;
        pcb[i].priority = i;
        pcb[i].remaining_burst_time = i;
        pcb[i].started = false;
    }

    wroteOut += fwrite(pcb, sizeof(ProcessControlBlock_t), numPCBs, f); // writes out array of pcbs
    fclose(f);

    dyn_array_t *pcbs = load_process_control_blocks(input_filename);
    ScheduleResult_t *SJF_result = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));
    bool validSJF = shortest_job_first(pcbs, SJF_result);

    ASSERT_TRUE(validSJF);

    for (int i = 1; i < numPCBs; i++) {
        ProcessControlBlock_t *pcb_at_index = (ProcessControlBlock_t *)dyn_array_at(pcbs,i);
        ProcessControlBlock_t *pcb_at_prev_index = (ProcessControlBlock_t *)dyn_array_at(pcbs,i-1);

        // Burst time should be ordered from least to greatest
        EXPECT_GE(pcb_at_index->remaining_burst_time, pcb_at_prev_index->remaining_burst_time);
    }

    // Clean up
    dyn_array_destroy(pcbs);
    free(SJF_result);
}

TEST (round_robin, NULL_queue) {
    ScheduleResult_t * RR_result = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));  
    bool validRR = first_come_first_serve(NULL, RR_result);
    EXPECT_FALSE(validRR);
}

TEST (round_robin, pcbbinRunAndValCheck){
    const char *input_filename = "pcb.bin";

    dyn_array* pcbs = load_process_control_blocks(input_filename);
    dyn_array_t* temp = NULL;
    ASSERT_NE(pcbs, temp);

    EXPECT_EQ(dyn_array_size(pcbs), 4);

    ScheduleResult_t * RR_result = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));  

    bool res = round_robin(pcbs, RR_result, 5);

    EXPECT_TRUE(res);
    EXPECT_EQ(dyn_array_size(pcbs), 0);

    EXPECT_EQ(RR_result->total_run_time, 50);
    EXPECT_EQ(RR_result->average_waiting_time, 7.5);
    EXPECT_EQ(RR_result->average_turnaround_time, 33.75);
}


int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}

/*
#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            score = 0;
            total = 210;
        }

        virtual void TearDown()
        {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};
*/

