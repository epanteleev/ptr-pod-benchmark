#pragma once

#include <cassert>
#include <cstdlib>
#include <vector>

class Arena final {
public:
    static const constexpr std::size_t SIZE = 5000000;

public:
    Arena() {
        start = new char[SIZE];
        current = start;
    }

    ~Arena() {
        free(start);
    }

public:
    template<typename T, typename ...Args>
    T *alloc(Args... args) {
        std::size_t s = sizeof(T);
        capacity += s;
        if (capacity > SIZE) {
            fprintf(stderr, "Arena was overflow\n");
            std::terminate();
        }
        void *adr = current;
        current += s;
        return new(adr) T(std::forward<Args>(args)...);
    }

private:
    char *start;
    char *current;
    std::size_t capacity{};
};

