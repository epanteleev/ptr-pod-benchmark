#include "Person.h"

namespace rd {
    const char *genstring(std::size_t length) {
        const char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<std::size_t> dist(0, strlen(string) - 1);

        char *randomString = new char[length + 1];

        for (std::size_t n = 0; n < length; n++) {
            randomString[n] = string[dist(mt)];
        }

        randomString[length] = '\0';

        return randomString;
    }

    std::size_t genbalance() {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<std::size_t> dist(5, 100000);
        return dist(mt);
    }
}