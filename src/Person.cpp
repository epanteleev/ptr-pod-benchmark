#include "Person.h"

namespace rd {
    const char *genstring(std::size_t length) {
        const char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";

        rd::Uniform un(0, strlen(string) - 1, 2122212211);
        char *randomString = new char[length + 1];

        for (std::size_t n = 0; n < length; n++) {
            randomString[n] = string[un()];
        }

        randomString[length] = '\0';

        return randomString;
    }
}