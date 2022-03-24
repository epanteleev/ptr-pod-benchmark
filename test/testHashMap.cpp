#include "gtest/gtest.h"
#include "../src/HashMap.h"

TEST(HashMapEntry, Creation) {
    HashMap<int, int> t;
    int a = 4, b = 5;
    t.put(&a, &b);
    ASSERT_EQ(t.get(&a), &b);
    ASSERT_EQ(*t.get(&a), b);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}