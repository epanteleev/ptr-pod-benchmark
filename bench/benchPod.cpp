#include <benchmark/benchmark.h>
#include "../src/Person.h"

class PodPerson64bH : public benchmark::Fixture {
public:
    PodPerson64bH() = default;

    ~PodPerson64bH() override = default;

    void SetUp(const benchmark::State &state) override {
        persons.clear();
        for (auto i = 0; i < state.range(0); i++) {
            persons.emplace_back(rd::genstring(10), rd::genstring(10), rd::genbalance());
        }
    }

public:
    std::vector<Person64bH> persons;
};

class PodPerson128bH : public benchmark::Fixture {
public:
    PodPerson128bH() = default;

    ~PodPerson128bH() override = default;

    void SetUp(const benchmark::State &state) override {
        persons.clear();
        for (auto i = 0; i < state.range(0); i++) {
            persons.emplace_back(rd::genstring(10), rd::genstring(10), rd::genbalance());
        }
    }

public:
    std::vector<Person128bH> persons;
};

BENCHMARK_DEFINE_F(PodPerson64bH, ForwardAccess)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i.updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PodPerson64bH, RandomAccess)(benchmark::State& state) {
    rd::RandomAccess ra(persons.size());
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (std::size_t i = 0; i < persons.size(); i++) {
            persons[ra.next()].updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PodPerson128bH, ForwardAccess)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i.updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PodPerson128bH, RandomAccess)(benchmark::State& state) {
    rd::RandomAccess ra(persons.size());
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (std::size_t i = 0; i < persons.size(); i++) {
            persons[ra.next()].updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_REGISTER_F(PodPerson128bH, ForwardAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PodPerson128bH, RandomAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PodPerson64bH, ForwardAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PodPerson64bH, RandomAccess)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);