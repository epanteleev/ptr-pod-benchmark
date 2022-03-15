#pragma once

#include <cassert>
#include <cstdlib>
#include <vector>

class Arena final {
public:
    static const constexpr std::size_t SIZE = 100 * 1024 * 1024;

public:
    Arena() {
        start = new std::uint8_t[SIZE];
        current = start;
    }

    ~Arena() {
        free(start);
    }

public:
    template<typename T, typename ...Args>
    T *alloc(Args... args) {
        std::size_t s = sizeof(T);
        reserved += s;
        if (reserved > SIZE) {
            fprintf(stderr, "Arena was overflow\n");
            std::terminate();
        }
        auto adr = current;
        current += s;
        return new(adr) T(std::forward<Args>(args)...);
    }

    void reset() {
        current = start;
        reserved = 0;
    }

private:
    std::uint8_t *start;
    std::uint8_t *current;
    std::size_t reserved{};
};

