//
// Created by Aleksandr Mikhailov on 04.12.2024.
//


#include <numbers>
#include <gtest/gtest.h>

#include "libraryFirst/libraryFirst.h"
TEST(PiFunction, ShouldGivePiCorrectly) {
    EXPECT_NEAR(std::numbers::pi_v<float>, pi(1000000), 1e-5);
}

TEST(TranslationFunction, ShouldTranslateOneToBinaryCorrectly) {
    EXPECT_EQ("1", translation(1));
}

TEST(TranslationFunction, ShouldTranslateTwoToBinaryCorrectly) {
    EXPECT_EQ("10", translation(2));
}

TEST(TranslationFunction, ShouldTranslateTenToBinaryCorrectly) {
    EXPECT_EQ("1010", translation(10));
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
