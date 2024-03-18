#include <gtest/gtest.h>
#include "interval_map.h"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

TEST(IntervalMapTest, BasicFunctionality) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    EXPECT_EQ(imap[0], 'A');
    EXPECT_EQ(imap[1], 'B');
    EXPECT_EQ(imap[2], 'B');
    EXPECT_EQ(imap[3], 'A');
}

TEST(IntervalMapTest, OverlappingIntervals) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    EXPECT_EQ(imap[0], 'A');
    EXPECT_EQ(imap[1], 'B');
    EXPECT_EQ(imap[2], 'C');
    EXPECT_EQ(imap[3], 'C');
    EXPECT_EQ(imap[4], 'A');
}

TEST(IntervalMapTest, EmptyIntervals) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    EXPECT_EQ(imap[4], 'A');
    EXPECT_EQ(imap[5], 'A');
}

TEST(IntervalMapTest, InsertAtBeginning) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    imap.assign(-5, 1, 'E');
    EXPECT_EQ(imap[-5], 'E');
    EXPECT_EQ(imap[0], 'E');
    EXPECT_EQ(imap[1], 'B');
    EXPECT_EQ(imap[2], 'C');
    EXPECT_EQ(imap[3], 'C');
    EXPECT_EQ(imap[4], 'A');
    EXPECT_EQ(imap[5], 'A');
}

TEST(IntervalMapTest, InsertAtBeginningEndDoesntExist) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    imap.assign(-5, 1, 'E');
    imap.assign(-8, -6, 'F');
    EXPECT_EQ(imap[-8], 'F');
    EXPECT_EQ(imap[-7], 'F');
    EXPECT_EQ(imap[-6], 'A');
    EXPECT_EQ(imap[-5], 'E');
}

TEST(IntervalMapTest, InsertAtBeginningValueIsSameAsMValBeginAndEndExists) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    imap.assign(-5, 1, 'E');
    imap.assign(-8, -6, 'F');
    imap.assign(-20, -7, 'A');
    EXPECT_EQ(imap[-20], 'A');
    EXPECT_EQ(imap[-7], 'F');
    EXPECT_EQ(imap[-6], 'A');
}

TEST(IntervalMapTest, InsertAtBeginningValueIsSameAsMValBeginAndEndDoesntExist) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    imap.assign(-5, 1, 'E');
    imap.assign(-8, -6, 'F');
    imap.assign(-20, -7, 'A');
    imap.assign(-20, -19, 'A');
    EXPECT_EQ(imap[-20], 'A');
    EXPECT_EQ(imap[-19], 'A');
    EXPECT_EQ(imap[-7], 'F');
}

TEST(IntervalMapTest, InsertAtBeginningEndIsBeforeFirstInterval) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    imap.assign(-5, 1, 'E');
    imap.assign(-8, -6, 'F');
    imap.assign(-20, -7, 'A');
    imap.assign(-20, -19, 'A');
    imap.assign(-20, -10, 'G');
    EXPECT_EQ(imap[-20], 'G');
    EXPECT_EQ(imap[-10], 'A');
    EXPECT_EQ(imap[-7], 'F');
}

TEST(IntervalMapTest, InsertAtEndValueIsSameAsMValBegin) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    imap.assign(-5, 1, 'E');
    imap.assign(-8, -6, 'F');
    imap.assign(-20, -7, 'A');
    imap.assign(-20, -19, 'A');
    imap.assign(-20, -10, 'G');
    imap.assign(5, 10, 'A');
    EXPECT_EQ(imap[5], 'A');
    EXPECT_EQ(imap[10], 'A');
    EXPECT_EQ(imap[4], 'A');
    EXPECT_EQ(imap[3], 'C');
}

TEST(IntervalMapTest, InsertAtEndValueIsSameAsExistingInterval) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    imap.assign(-5, 1, 'E');
    imap.assign(-8, -6, 'F');
    imap.assign(-20, -7, 'A');
    imap.assign(-20, -19, 'A');
    imap.assign(-20, -10, 'G');
    imap.assign(5, 10, 'A');
    imap.assign(4, 10, 'C');
    EXPECT_EQ(imap[4], 'C');
    EXPECT_EQ(imap[10], 'A');
    EXPECT_EQ(imap[3], 'C');
}

TEST(IntervalMapTest, InsertAtStartValueIsSameAsExistingInterval) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    imap.assign(-5, 1, 'E');
    imap.assign(-8, -6, 'F');
    imap.assign(-20, -7, 'A');
    imap.assign(-20, -19, 'A');
    imap.assign(-20, -10, 'G');
    imap.assign(5, 10, 'A');
    imap.assign(4, 10, 'C');
    imap.assign(-13, -9, 'F');
    EXPECT_EQ(imap[-13], 'F');
    EXPECT_EQ(imap[-9], 'A');
    EXPECT_EQ(imap[-14], 'G');
}

TEST(IntervalMapTest, OverrideWholeMapWithoutDefaultValue) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    imap.assign(-5, 1, 'E');
    imap.assign(-8, -6, 'F');
    imap.assign(-20, -7, 'A');
    imap.assign(-20, -19, 'A');
    imap.assign(-20, -10, 'G');
    imap.assign(5, 10, 'A');
    imap.assign(4, 10, 'C');
    imap.assign(-13, -9, 'F');
    imap.assign(-100, 100, 'Z');
    for (int i = -100; i < 100; i++) {
        EXPECT_EQ(imap[i], 'Z');
    }
    EXPECT_EQ(imap[100], 'A');
}

TEST(IntervalMapTest, OverrideWholeMapWithDefaultValue) {
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(2, 4, 'C');
    imap.assign(5, 5, 'D');
    imap.assign(-5, 1, 'E');
    imap.assign(-8, -6, 'F');
    imap.assign(-20, -7, 'A');
    imap.assign(-20, -19, 'A');
    imap.assign(-20, -10, 'G');
    imap.assign(5, 10, 'A');
    imap.assign(4, 10, 'C');
    imap.assign(-13, -9, 'F');
    imap.assign(-100, 100, 'A');
    for (int i = -200; i < 200; i++) {
        EXPECT_EQ(imap[i], 'A');
    }
}

TEST(IntervalMapTest, RandomTest) {
    interval_map<int, char> imap('A');
    srand(time(0));
    for (int i = 0; i < 1000000; i++) {
        int start = rand() % 20001 - 10000;
        int end = start + rand() % 20001 - 10000;
        char val = 'A' + rand() % 26;
        imap.assign(start, end, val);
    }

    for (char c = 'Z'; c >= 'A'; c--) {
        imap.assign(-1000000, 10000000, c);
    }

    for (int i = -1000000; i < 10000000; i++) {
        EXPECT_EQ(imap[i], 'A');
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
