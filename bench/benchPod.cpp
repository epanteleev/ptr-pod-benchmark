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

class PodPerson64bHFlatArray : public benchmark::Fixture {
public:
    PodPerson64bHFlatArray() = default;

    ~PodPerson64bHFlatArray() override {
        reset();
    }

    void reset() {
        if (persons != nullptr) {
            delete[] persons;
        }
        len = 0;
    }

    void SetUp(const benchmark::State &state) override {
        reset();
        len = state.range(0);
        persons = new std::uint8_t[len * sizeof(Person64bH)];
        for (auto i = 0; i < state.range(0); i++) {
            new (persons + (i * sizeof(Person64bH))) Person64bH(rd::genstring(10), rd::genstring(10), rd::genbalance());
        }
    }

    std::size_t* getBalance(std::size_t i) {
        return reinterpret_cast<std::size_t *>(reinterpret_cast<std::size_t*>(persons) + 3 + (i * 4));
    }
public:
    std::uint8_t* persons{};
    std::size_t len;
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

BENCHMARK_DEFINE_F(PodPerson64bH, BenchPod)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (auto &i: persons) {
            i.updateBalance(10);
        }
    }
    benchmark::DoNotOptimize(persons);
}

BENCHMARK_DEFINE_F(PodPerson64bHFlatArray, BenchPod)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (std::size_t i = 0; i < len; i++) {
            auto* b = getBalance(i);
            *b = *b + (std::size_t)((double)(*b) * 0.1);
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

BENCHMARK_REGISTER_F(PodPerson128bH, BenchPod)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PodPerson64bH, BenchPod)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK_REGISTER_F(PodPerson64bHFlatArray, BenchPod)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);