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
        for (auto i = 0; i < state.range(0); i++) {
            persons.push_back(new Person64bH(rd::genstring(10),
                                             rd::genstring(10),
                                             rd::genbalance()));
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
        for (auto i = 0; i < state.range(0); i++) {
            persons.push_back(arena.alloc<Person64bH>(rd::genstring(10),
                                                      rd::genstring(10),
                                                      rd::genbalance()));
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
        for (auto i = 0; i < state.range(0); i++) {
            persons.push_back(new Person128bH(rd::genstring(10),
                                              rd::genstring(10),
                                              rd::genbalance()));
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
        for (auto i = 0; i < state.range(0); i++) {
            persons.push_back(arena.alloc<Person128bH>(rd::genstring(10),
                                                       rd::genstring(10),
                                                       rd::genbalance()));
        }
    }

public:
    Arena arena;
    std::vector<Person128bH *> persons;
};

BENCHMARK_DEFINE_F(PtrPerson64bH, BenchPtr)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PtrPerson128bH, BenchPtr)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PtrPerson64bHArena, BenchPtr)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PtrPerson128bHArena, BenchPtr)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_REGISTER_F(PtrPerson64bH, BenchPtr)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PtrPerson128bH, BenchPtr)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PtrPerson64bHArena, BenchPtr)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PtrPerson128bHArena, BenchPtr)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);

BENCHMARK_MAIN();