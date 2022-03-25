#include "gtest/gtest.h"
#include <list>

#include "../src/HashMap.h"
#include "../src/Person.h"

TEST(HashMap, Creation0) {
    HashMap<int, int> t;
    int a = 4, b = 5;
    t.put(&a, &b);
    ASSERT_EQ(*t.get(&a), b);
}

TEST(HashMap, Creation1) {
    HashMap<int, int> t;
    int a = 4, b = 5;
    int a1 = 40, b1 = 50;
    t.put(&a, &b);
    t.put(&a1, &b1);
    ASSERT_EQ(*t.get(&a), b);
    ASSERT_EQ(*t.get(&a1), b1);
}

TEST(HashMap, Creation2) {
    HashMap<std::string, Person128bH> map;
    rd::Uniform un(5, 100000, 20222022122);
    std::size_t len = 10000;

    std::vector<Person128bH*> p;
    for (std::size_t i = 0; i < len; i++) {
        p.emplace_back(new Person128bH(rd::genstring(10),rd::genstring(10),un()));
    }

    std::vector<std::string*> s;
    for (std::size_t i = 0; i < len; i++) {
        s.emplace_back(new std::string(rd::genstring(10)));
    }

    for (std::size_t i = 0; i < len; i++) {
        map.put(s[i], p[i]);
    }

    for (std::size_t i = 0; i < len; i++) {
        ASSERT_EQ(*map.get(s[i]), *p[i]);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}