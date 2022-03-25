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

    Person64bH(Person64bH &&p) noexcept:
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
        m_balance += (std::size_t) (m_balance * ((double) percent / 100));
    }

    bool operator==(const Person64bH& other) const {
        return m_balance == other.m_balance &&
            (std::strcmp(m_name, other.m_name) == 0) &&
            (std::strcmp(m_lastname, other.m_lastname) == 0);
    }
public:
    std::uint64_t header1;
    const char *m_name;
    const char *m_lastname;
    std::size_t m_balance;
};

namespace std {
    template <>
    struct hash<Person64bH> {
        std::size_t operator()(const Person64bH& p) const {
            return ((std::hash<const char*>()(p.m_name) ^ (std::hash<const char*>()(p.m_lastname) << 1)) >> 1)
                   ^ (std::hash<std::size_t>()(p.m_balance) << 1);
        }
    };
}

class Person128bH final {
public:
    Person128bH(const char *name, const char *lastname, std::size_t balance) :
            header1(static_cast<std::size_t>(rand())),
            header2(static_cast<std::size_t>(rand())),
            m_name(name),
            m_lastname(lastname),
            m_balance(balance) {}

    Person128bH(Person128bH &&p) noexcept:
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
        m_balance += (std::size_t) (m_balance * ((double) percent / 100));
    }

    bool operator==(const Person128bH& other) const {
        return m_balance == other.m_balance &&
               (std::strcmp(m_name, other.m_name) == 0) &&
               (std::strcmp(m_lastname, other.m_lastname) == 0);
    }
public:
    std::uint64_t header1;
    std::uint64_t header2;
    const char *m_name;
    const char *m_lastname;
    std::size_t m_balance;
};

namespace std {
    template <>
    struct hash<Person128bH> {
        std::size_t operator()(const Person128bH& p) const {
            return ((std::hash<const char*>()(p.m_name) ^ (std::hash<const char*>()(p.m_lastname) << 1)) >> 1)
                   ^ (std::hash<std::size_t>()(p.m_balance) << 1);
        }
    };
}

static_assert(sizeof(Person64bH) == sizeof(std::uint64_t) * 4, "Unexpected Person64bH class align");
static_assert(sizeof(Person128bH) == sizeof(std::uint64_t) * 5, "Unexpected Person128bH class align");

namespace rd {
    const char *genstring(size_t length);

    class Uniform final {
    public:
        Uniform(std::size_t min, std::size_t max, std::size_t seed) :
                m_gen(seed),
                m_dist(min, max) {}

        std::size_t operator()() {
            return m_dist(m_gen);
        }

        void reset() {
            m_dist.reset();
        }
    private:
        std::mt19937 m_gen;
        std::uniform_int_distribution<std::size_t> m_dist;
    };

    class RandomAccess final {
    public:
        explicit RandomAccess(std::size_t arrlen) :
                m_rd(0, arrlen, 20222022) {}

        ~RandomAccess() = default;

    public:
        inline std::size_t next() noexcept {
            return m_rd();
        }

    public:
        rd::Uniform m_rd;
    };
}