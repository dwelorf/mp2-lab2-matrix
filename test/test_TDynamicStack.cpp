#include <../gtest/gtest.h>
#include "TDynamicStack.h"

TEST(TDynamicStackTest, ConstructorAndSize) {
    TDynamicStack<int> stack;
    EXPECT_EQ(stack.size(), 0);
    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_FALSE(stack.IsFull());
}

TEST(TDynamicStackTest, PushAndPop) {
    TDynamicStack<int> stack;

    stack.Push(10);
    EXPECT_EQ(stack.size(), 1);
    EXPECT_FALSE(stack.IsEmpty());
    EXPECT_EQ(stack.Top(), 10);

    stack.Push(20);
    stack.Push(30);
    EXPECT_EQ(stack.size(), 3);
    EXPECT_EQ(stack.Top(), 30);

    EXPECT_EQ(stack.Pop(), 30);
    EXPECT_EQ(stack.Pop(), 20);
    EXPECT_EQ(stack.Pop(), 10);
    EXPECT_TRUE(stack.IsEmpty());
}

TEST(TDynamicStackTest, MemoryReallocation) {
    TDynamicStack<int> stack(2);

    stack.Push(1);
    stack.Push(2);
    EXPECT_TRUE(stack.IsFull());

    stack.Push(3);
    EXPECT_EQ(stack.size(), 3);
    EXPECT_EQ(stack.Top(), 3);

    stack.Push(4);
    stack.Push(5);
    EXPECT_EQ(stack.size(), 5);

    EXPECT_EQ(stack.Pop(), 5);
    EXPECT_EQ(stack.Pop(), 4);
    EXPECT_EQ(stack.Pop(), 3);
    EXPECT_EQ(stack.Pop(), 2);
    EXPECT_EQ(stack.Pop(), 1);
}

TEST(TDynamicStackTest, PopFromEmptyStack) {
    TDynamicStack<int> stack;
    EXPECT_THROW(stack.Pop(), std::underflow_error);
    EXPECT_THROW(stack.Top(), std::underflow_error);
}

TEST(TDynamicStackTest, TopMethod) {
    TDynamicStack<std::string> stack;
    stack.Push("hello");
    stack.Push("world");

    EXPECT_EQ(stack.Top(), "world");
    stack.Pop();
    EXPECT_EQ(stack.Top(), "hello");

    const auto& constStack = stack;
    EXPECT_EQ(constStack.Top(), "hello");
}

TEST(TDynamicStackTest, ComplexTypes) {
    TDynamicStack<std::vector<int>> stack;

    std::vector<int> v1 = { 1, 2, 3 };
    std::vector<int> v2 = { 4, 5, 6 };

    stack.Push(v1);
    stack.Push(v2);

    EXPECT_EQ(stack.Top(), v2);
    stack.Pop();
    EXPECT_EQ(stack.Top(), v1);
}