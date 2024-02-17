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

    int numPCBs = 5;
    ProcessControlBlock_t pcb[numPCBs]; 

    int32_t arr[1] = {numPCBs};
    size_t wroteOut = fwrite(arr, sizeof(int32_t), 1, f);     

    for(int i = 0; i < numPCBs; i++){
        pcb[i].remaining_burst_time = i;
        pcb[i].priority = i;
        pcb[i].arrival = i;
        pcb[i].started = false;
    }

    wroteOut += fwrite(pcb, sizeof(ProcessControlBlock_t), numPCBs, f); // writes out array of pcbs
    fclose(f);

    dyn_array_t* res = load_process_control_blocks(input_filename);

    EXPECT_EQ(wroteOut, 1 + numPCBs);
    EXPECT_EQ(dyn_array_empty(res), false);
    EXPECT_EQ(dyn_array_size(res), numPCBs);
    for(int i = 0; i < numPCBs; i++){
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

