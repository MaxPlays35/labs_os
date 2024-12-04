//
// Created by Aleksandr Mikhailov on 04.12.2024.
//


#include <numbers>
#include <gtest/gtest.h>

#include "libraryFirst/libraryFirst.h"

TEST(PI_FUNCTION, PI_TEST) {
    EXPECT_NEAR(std::numbers::pi_v<float>, pi(1000000), 1e-5);
}

TEST(TRANSLATION_FUNCTION, FIRST_TEST) {
    EXPECT_EQ("1", translation(1));
}

TEST(TRANSLATION_FUNCTION, SECOND_TEST) {
    EXPECT_EQ("10", translation(2));
}

TEST(TRANSLATION_FUNCTION, THIRD_TEST) {
    EXPECT_EQ("1010", translation(10));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
