#include <Tape.h>
#include <TapeSort.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <fstream>

#define TEST_MODEL_DIR      "../test_files/"
#define CONFIG_FILE_PATH    "../config.txt"


static void DoTest (const std::vector <std::pair <std::string, size_t>>& theFileNamesSizes)
{
    for (const auto& aFileNameSize : theFileNamesSizes) {

        std::string anOutputFileName = aFileNameSize.first + "_out.bin";
        {
            Tape anInTape (aFileNameSize.first, CONFIG_FILE_PATH);
            Tape anOutTape (anOutputFileName, CONFIG_FILE_PATH);

            SortTape (anInTape, anOutTape, 512, 4);

            size_t aCount = 0;
            int aPrevNumber = INT_MAX;
            while (!anOutTape.IsEnd()) {
                int aCurNumber = anOutTape.Read();

                ++aCount;
                EXPECT_TRUE (aCurNumber <= aPrevNumber);
                aPrevNumber = aCurNumber;

                if (!anOutTape.MoveToPrev()) {
                    break;
                }
            }

            EXPECT_TRUE (aCount == aFileNameSize.second);
        }

        std::remove (anOutputFileName.c_str());
    }
}

TEST(tape_test, zero_elem)
{
    std::vector <std::pair <std::string, size_t>> aFileNamesSizes =  {
        std::make_pair (TEST_MODEL_DIR + std::string ("InTapeTest_zero_1.bin"), 0),
    };

    DoTest (aFileNamesSizes);
}

TEST(tape_test, one_elem)
{
    std::vector <std::pair <std::string, size_t>> aFileNamesSizes =  {
        std::make_pair (TEST_MODEL_DIR + std::string ("InTapeTest_one_1.bin"), 1),
    };

    DoTest (aFileNamesSizes);
}

TEST(tape_test, two_elems)
{
    std::vector <std::pair <std::string, size_t>> aFileNamesSizes =  {
        std::make_pair (TEST_MODEL_DIR + std::string ("InTapeTest_two_1.bin"), 2),
        std::make_pair (TEST_MODEL_DIR + std::string ("InTapeTest_two_2.bin"), 2),
    };

    DoTest (aFileNamesSizes);
}

TEST(tape_test, many_elems)
{
    std::vector <std::pair <std::string, size_t>> aFileNamesSizes =  {
        std::make_pair (TEST_MODEL_DIR + std::string ("InTapeTest_many_1.bin"), 500),
        std::make_pair (TEST_MODEL_DIR + std::string ("InTapeTest_many_2.bin"), 1000),
        std::make_pair (TEST_MODEL_DIR + std::string ("InTapeTest_many_3.bin"), 2011),
        std::make_pair (TEST_MODEL_DIR + std::string ("InTapeTest_many_4.bin"), 13),
        std::make_pair (TEST_MODEL_DIR + std::string ("InTapeTest_many_5.bin"), 512),
    };

    DoTest (aFileNamesSizes);
}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
