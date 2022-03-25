#include <benchmark/benchmark.h>
#include "../src/Person.h"
#include "../src/Arena.h"

class PtrPerson64bH : public benchmark::Fixture {
public:
    PtrPerson64bH() = default;

    ~PtrPerson64bH() override {
        for (auto& i: persons) {
            delete i;
        }
    }

    void SetUp(const benchmark::State &state) override {
        persons.clear();
        rd::Uniform un(5, 100000, 20222022122);
        for (auto i = 0; i < state.range(0); i++) {
            persons.push_back(new Person64bH(rd::genstring(10),
                                             rd::genstring(10),
                                             un()));
        }
    }

public:
    std::vector<Person64bH *> persons;
};

class PtrPerson64bHArena : public benchmark::Fixture {
public:
    PtrPerson64bHArena() = default;

    ~PtrPerson64bHArena() override {
        for (auto& i: persons) {
            i->destroy();
        }
    }

    void SetUp(const benchmark::State &state) override {
        persons.clear();
        arena.reset();
        rd::Uniform un(5, 100000, 20222022122);
        for (auto i = 0; i < state.range(0); i++) {
            persons.push_back(arena.alloc<Person64bH>(rd::genstring(10),
                                                      rd::genstring(10),
                                                      un()));
        }
    }

public:
    Arena arena;
    std::vector<Person64bH *> persons;
};

class PtrPerson128bH : public benchmark::Fixture {
public:
    PtrPerson128bH() = default;

    ~PtrPerson128bH() override {
        for (auto& i: persons) {
            delete i;
        }
    }

    void SetUp(const benchmark::State &state) override {
        persons.clear();
        rd::Uniform un(5, 100000, 20222022122);
        for (auto i = 0; i < state.range(0); i++) {
            persons.push_back(new Person128bH(rd::genstring(10),
                                              rd::genstring(10),
                                               un()));
        }
    }

public:
    std::vector<Person128bH *> persons;
};

class PtrPerson128bHArena : public benchmark::Fixture {
public:
    PtrPerson128bHArena() = default;

    ~PtrPerson128bHArena() override {
        for (auto& i: persons) {
            i->destroy();
        }
    }

    void SetUp(const benchmark::State &state) override {
        persons.clear();
        arena.reset();
        rd::Uniform un(5, 100000, 20222022122);
        for (auto i = 0; i < state.range(0); i++) {
            persons.push_back(arena.alloc<Person128bH>(rd::genstring(10),
                                                       rd::genstring(10),
                                                       un()));
        }
    }

public:
    Arena arena;
    std::vector<Person128bH *> persons;
};

BENCHMARK_DEFINE_F(PtrPerson64bH, ForwardAccess)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PtrPerson64bH, RandomAccess)(benchmark::State& state) {
    rd::RandomAccess ra(persons.size());
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (std::size_t i = 0; i < persons.size(); i++) {
            persons[ra.next()]->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PtrPerson128bH, ForwardAccess)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PtrPerson128bH, RandomAccess)(benchmark::State& state) {
    rd::RandomAccess ra(persons.size());
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (std::size_t i = 0; i < persons.size(); i++) {
            persons[ra.next()]->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PtrPerson64bHArena, ForwardAccess)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PtrPerson64bHArena, RandomAccess)(benchmark::State& state) {
    rd::RandomAccess ra(persons.size());
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (std::size_t i = 0; i < persons.size(); i++) {
            persons[ra.next()]->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PtrPerson128bHArena, ForwardAccess)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PtrPerson128bHArena, RandomAccess)(benchmark::State& state) {
    rd::RandomAccess ra(persons.size());
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (std::size_t i = 0; i < persons.size(); i++) {
            persons[ra.next()]->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_REGISTER_F(PtrPerson64bH, ForwardAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PtrPerson128bH, ForwardAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PtrPerson64bHArena, ForwardAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PtrPerson128bHArena, ForwardAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);

BENCHMARK_REGISTER_F(PtrPerson64bH, RandomAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PtrPerson128bH, RandomAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PtrPerson64bHArena, RandomAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PtrPerson128bHArena, RandomAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);

BENCHMARK_MAIN();