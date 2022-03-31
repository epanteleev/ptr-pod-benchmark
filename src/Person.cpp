#include "Person.h"

namespace rd {
    const char *RandomString::operator()() {
        const char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
        char *randomString = new char[m_length + 1];

        for (std::size_t n = 0; n < m_length; n++) {
            randomString[n] = string[m_un()];
        }
        randomString[m_length] = '\0';

        return randomString;
    }
}