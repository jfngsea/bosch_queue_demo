//
// Created by jfngsea on 2/5/25.
//

#include <gtest/gtest.h>

#include "MyQueue.h"

class MyQueueTest : public testing::Test {
protected:
    MyQueueTest(): q0_(2), q1_(2), q2_(2), q3_(2) {
        // q0_ remains empty
        q1_.push(1);
        q2_.push(2);
        q2_.push(3);

        q3_.push(1);
        q3_.push(2);
        q3_.push(3);
    }

    MyQueue<int> q0_;
    MyQueue<int> q1_;
    MyQueue<int> q2_;
    MyQueue<int> q3_;

};

TEST_F(MyQueueTest, IsEmptyInitially) {
    EXPECT_EQ(q0_.count(), 0);
}

TEST_F(MyQueueTest, countIsCorrect) {
    EXPECT_EQ(q1_.count(), 1);
    EXPECT_EQ(q2_.count(), 2);
    EXPECT_EQ(q3_.count(), 2);

    q1_.pop();

    q2_.pop();
    q2_.pop();

    q3_.pop();

    EXPECT_EQ(q1_.count(), 0);
    EXPECT_EQ(q2_.count(), 0);
    EXPECT_EQ(q3_.count(), 1);

}

TEST_F(MyQueueTest, popWorks) {
    EXPECT_EQ(q1_.pop(), 1);

    EXPECT_EQ(q2_.pop(), 2);
    EXPECT_EQ(q2_.pop(), 3);
}

TEST_F(MyQueueTest, queueOverflowWorks) {
    //pop only one element
    EXPECT_EQ(q3_.pop(), 2);
    EXPECT_EQ(q3_.count(), 1);

    q3_.push(4);
    q3_.push(5);

    EXPECT_EQ(q3_.count(), 2);

    EXPECT_EQ(q3_.pop(), 4);
    EXPECT_EQ(q3_.pop(), 5);
}



