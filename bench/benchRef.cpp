#include <benchmark/benchRef.h>
#include "../src/Person.h"

class RefPerson64bH : public benchmark::Fixture {
public:
    RefPerson64bH() = default;

    ~RefPerson64bH() override {
        for (auto& i: persons) {
            delete i;
        }
    }

    void SetUp(const benchmark::State &state) override {
        persons.clear();
        for (auto i = 0; i < state.range(0); i++) {
            persons.push_back(new Person64bH(random::genstring(10),
                                             random::genstring(10),
                                             random::genbalance()));
        }
    }

public:
    std::vector<Person64bH *> persons;
};

class RefPerson128bH : public benchmark::Fixture {
public:
    RefPerson128bH() = default;

    ~RefPerson128bH() override {
        for (auto& i: persons) {
            delete i;
        }
    }

    void SetUp(const benchmark::State &state) override {
        persons.clear();
        for (auto i = 0; i < state.range(0); i++) {
            persons.push_back(new Person128bH(random::genstring(10),
                                             random::genstring(10),
                                             random::genbalance()));
        }
    }

public:
    std::vector<Person128bH *> persons;
};

BENCHMARK_DEFINE_F(RefPerson64bH, BenchPtr)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(RefPerson128bH, BenchPtr)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i->updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_REGISTER_F(RefPerson64bH, BenchPtr)->Arg(1000)->Arg(10000)->Arg(100000)->Iterations(10000);
BENCHMARK_REGISTER_F(RefPerson128bH, BenchPtr)->Arg(1000)->Arg(10000)->Arg(100000)->Iterations(10000);

BENCHMARK_MAIN();