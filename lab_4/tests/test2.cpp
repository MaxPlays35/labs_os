//
// Created by Aleksandr Mikhailov on 04.12.2024.
//

#include <numbers>
#include <gtest/gtest.h>

#include "librarySecond/librarySecond.h"

TEST(PiFunction, ShouldGivePiCorrectly) {
    EXPECT_NEAR(std::numbers::pi_v<float>, pi(1000000), 1e-3);
}

TEST(TranslationFunction, ShouldTranslateOneToTernaryCorrectly) {
    EXPECT_EQ("1", translation(1));
}

TEST(TranslationFunction, ShouldTranslateThreeToTernaryCorrectly) {
    EXPECT_EQ("10", translation(3));
}

TEST(TranslationFunction, ShouldTranslateElevenToTernaryCorrectly) {
    EXPECT_EQ("102", translation(11));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
