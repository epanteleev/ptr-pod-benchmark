#include <benchmark/benchmark.h>
#include "../src/Person.h"

class PodPerson64bH : public benchmark::Fixture {
public:
    PodPerson64bH() = default;

    ~PodPerson64bH() override = default;

    void SetUp(const benchmark::State &state) override {
        persons.clear();
        for (auto i = 0; i < state.range(0); i++) {
            persons.emplace_back(random::genstring(10),random::genstring(10), random::genbalance());
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
            persons.emplace_back(random::genstring(10),random::genstring(10), random::genbalance());
        }
    }

public:
    std::vector<Person128bH> persons;
};

BENCHMARK_DEFINE_F(PodPerson64bH, BenchPod)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i.updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PodPerson128bH, BenchPod)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i.updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_REGISTER_F(PodPerson128bH, BenchPod)->Arg(1000)->Arg(10000)->Arg(100000)->Iterations(10000);
BENCHMARK_REGISTER_F(PodPerson64bH, BenchPod)->Arg(1000)->Arg(10000)->Arg(100000)->Iterations(10000);