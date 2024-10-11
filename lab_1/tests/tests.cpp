//
// Created by Aleksandr Mikhailov on 27.09.2024.
//

#include <fstream>

#include "../src/parent.h"
#include "../src/utils.h"
#include "gtest/gtest.h";

TEST(BASE_TEST, TEST1) {
    std::string fileInput("test_input.txt");
    std::string fileOutput("test_output.txt");
    std::string fileResult("test_result.txt");

    std::array<std::string, 7> testData {
        "test_result.txt\n",
        "Hello there.\n",
        "Bad string\n",
        "Hi hi;\n",
        "Some valid string;\n",
        "Again bad string\n",
        "!\n"
    };

    std::vector<std::string> expectedOutput {
        "Enter a name for file",
        "Verification failed",
        "Verification failed"
    };

    std::vector<std::string> expectedResult {
        "Hello there.",
        "Hi hi;",
        "Some valid string;"
    };

    {
        std::ofstream file(fileInput);

        for (auto & string : testData) {
            file << string;
        }
    }

    {
        std::ifstream fileCin(fileInput);
        std::ofstream fileCout(fileOutput);

        ParentWork(fileCin, fileCout);
    }

    std::ifstream fileCout(fileOutput);
    std::ifstream fileRes(fileResult);

    std::vector<std::string> output = ReadFile(fileCout);
    std::vector<std::string> result = ReadFile(fileRes);

    ASSERT_TRUE(CheckEqualsOfStrings(output, expectedOutput));
    ASSERT_TRUE(CheckEqualsOfStrings(result, expectedResult));

    if (std::filesystem::exists(fileInput)) {
        std::filesystem::remove(fileInput);
    }

    if (std::filesystem::exists(fileOutput)) {
        std::filesystem::remove(fileOutput);
    }

    if (std::filesystem::exists(fileResult)) {
        std::filesystem::remove(fileResult);
    }
}