#define _CRT_SECURE_NO_WARNINGS

#include <benchmark/benchmark.h>

#include <cstring>
#include <random>
#include <ranges>
#include <memory>
#include <xoshiro.hpp>

namespace {
    std::string random_string_after(char val, size_t random_start, size_t n) {
        std::minstd_rand gen(42);
        std::string result(n, val);
        using std::ranges::views::drop;
        std::ranges::generate(result | drop(random_start), [&] {
            return static_cast<char>(std::uniform_int_distribution<short>(1, 127)(gen));
        });
        return result;
    }
    std::string random_string(size_t n) {
        std::minstd_rand gen(42);
        std::string result(n, 0);
        std::ranges::generate(result, 
            [&] { return static_cast<char>(std::uniform_int_distribution<short>(1, 127)(gen)); });
        return result;
    }
    void strcpy_call(benchmark::State& state) {
        const size_t r0 = static_cast<size_t>(state.range(0));
        const auto in_string = random_string(r0);
        std::string out_string(r0, 0);
        for([[maybe_unused]] auto _ : state) {
            benchmark::DoNotOptimize(in_string.data());
            std::strcpy(out_string.data(), in_string.data());
            benchmark::DoNotOptimize(out_string.data());
        }
    }
    void strlen_memcpy_call(benchmark::State& state) {
        const size_t r0 = static_cast<size_t>(state.range(0));
        const auto in_string = random_string(r0);
        std::string out_string(r0, 0);
        for([[maybe_unused]] auto _ : state) {
            benchmark::DoNotOptimize(in_string.data());
            std::memcpy(out_string.data(), in_string.data(), std::strlen(in_string.data()));
            benchmark::DoNotOptimize(out_string.data());
        }
    }
    void string_copy(benchmark::State& state) {
        // basically strlen+memcpy
        const size_t r0 = static_cast<size_t>(state.range(0));
        const auto in_string = random_string(r0);
        std::string out_string(r0, 0);
        for([[maybe_unused]] auto _ : state) {
            benchmark::DoNotOptimize(in_string.data());
            out_string.assign(in_string.data());
            benchmark::DoNotOptimize(out_string.data());
        }
    }
    void strcmp_different(benchmark::State& state) {
        std::string string1(state.range(0), '&');
        std::string string2(state.range(0), 'U');
        for([[maybe_unused]] auto _ : state) {
            benchmark::DoNotOptimize(string1.data());
            benchmark::DoNotOptimize(string2.data());
            int cmp = std::strcmp(string1.data(), string2.data());
            benchmark::DoNotOptimize(cmp);
        }
    }
    void strcmp_same(benchmark::State& state) {
        std::string string1(state.range(0), '&');
        std::string string2(state.range(0), '&');
        for([[maybe_unused]] auto _ : state) {
            benchmark::DoNotOptimize(string1.data());
            benchmark::DoNotOptimize(string2.data());
            int cmp = std::strcmp(string1.data(), string2.data());
            benchmark::DoNotOptimize(cmp);
        }
    }
    void memcmp_different(benchmark::State& state) {
        std::string string1(state.range(0), '&');
        std::string string2(state.range(0), 'U');
        for([[maybe_unused]] auto _ : state) {
            benchmark::DoNotOptimize(string1.data());
            benchmark::DoNotOptimize(string2.data());
            int cmp = std::memcmp(string1.data(), string2.data(), 1<<18);
            benchmark::DoNotOptimize(cmp);
        }
    }
    void memcmp_same(benchmark::State& state) {
        std::string string1(state.range(0), '&');
        std::string string2(state.range(0), '&');
        for([[maybe_unused]] auto _ : state) {
            benchmark::DoNotOptimize(string1.data());
            benchmark::DoNotOptimize(string2.data());
            int cmp = std::memcmp(string1.data(), string2.data(), state.range(0));
            benchmark::DoNotOptimize(cmp);
        }
    }
    void strlen_call(benchmark::State& state) {
        const auto in_string = random_string(state.range(0));
        for([[maybe_unused]] auto _ : state) {
            benchmark::DoNotOptimize(in_string.data());
            size_t len = std::strlen(in_string.data());
            benchmark::DoNotOptimize(len);
        }
    }
}

BENCHMARK(strcpy_call)->Range(0, 1 << 18);
BENCHMARK(strlen_memcpy_call)->Range(0, 1 << 18);
BENCHMARK(string_copy)->Range(0, 1 << 18);
BENCHMARK(strcmp_different)->Range(0, 1 << 18);
BENCHMARK(strcmp_same)->Range(0, 1 << 18);
BENCHMARK(memcmp_different)->Range(0, 1 << 18);
BENCHMARK(memcmp_same)->Range(0, 1 << 18);
BENCHMARK(strlen_call)->Range(0, 1 << 18);
BENCHMARK_MAIN();