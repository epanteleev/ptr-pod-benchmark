#pragma once

#include <utility>
#include <cstdint>
#include <cstdlib>
#include <random>
#include <cstring>

class Person64bH final {
public:
    Person64bH(const char *name, const char *lastname, std::size_t balance) :
        header1(static_cast<std::size_t>(rand())),
        m_name(name),
        m_lastname(lastname),
        m_balance(balance) {}

    Person64bH(Person64bH&& p) noexcept :
        header1(p.header1),
        m_name(std::exchange(p.m_name, nullptr)),
        m_lastname(std::exchange(p.m_lastname, nullptr)),
        m_balance(p.m_balance) {}

    ~Person64bH() {
        destroy();
    }

    void destroy() {
        delete m_name;
        delete m_lastname;
    }

    inline void updateBalance(std::size_t percent) noexcept {
        m_balance += (std::size_t)(m_balance * ((double)percent / 100));
    }

public:
    std::uint64_t header1;
    const char *m_name;
    const char *m_lastname;
    std::size_t m_balance;
};

class Person128bH final {
public:
    Person128bH(const char *name, const char *lastname, std::size_t balance) :
            header1(static_cast<std::size_t>(rand())),
            header2(static_cast<std::size_t>(rand())),
            m_name(name),
            m_lastname(lastname),
            m_balance(balance) {}

    Person128bH(Person128bH&& p) noexcept :
            header1(p.header1),
            header2(p.header2),
            m_name(std::exchange(p.m_name, nullptr)),
            m_lastname(std::exchange(p.m_lastname, nullptr)),
            m_balance(p.m_balance) {}

    ~Person128bH() {
        destroy();
    }

    void destroy() const {
        delete m_name;
        delete m_lastname;
    }

    inline void updateBalance(std::size_t percent) noexcept {
        m_balance += (std::size_t)(m_balance * ((double)percent / 100));
    }

public:
    std::uint64_t header1;
    std::uint64_t header2;
    const char *m_name;
    const char *m_lastname;
    std::size_t m_balance;
};

static_assert(sizeof(Person64bH) == sizeof(std::uint64_t) * 4, "Unexpected Person64bH class align");
static_assert(sizeof(Person128bH) == sizeof(std::uint64_t) * 5, "Unexpected Person128bH class align");

namespace rd {
    const char* genstring(size_t length);

    std::size_t genbalance();
}