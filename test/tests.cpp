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

TEST (load_process_control_blocks, NullInputFilename) {
    const char *input_filename = NULL;
    dyn_array_t* res = load_process_control_blocks(input_filename);
    dyn_array_t* temp = NULL;
    EXPECT_EQ(res, temp);
}

TEST (load_process_control_blocks, GoodReadHasContent) {
    const char *input_filename = "testData.bin";
    FILE* f = fopen(input_filename, "wb");
    int32_t arr[4] = {1, 5, 6, 7};
    size_t wroteOut = fwrite(arr,sizeof(int32_t), 4, f);
    fclose(f);
    dyn_array_t* res = load_process_control_blocks(input_filename);
    EXPECT_EQ(wroteOut, 4);
    EXPECT_EQ(dyn_array_empty(res), false);
    EXPECT_FALSE(res == NULL);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
