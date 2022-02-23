#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using my_float = long double;
using namespace std;

typedef struct {
    size_t large_chunk;
    size_t small_chunk;
    size_t split_item;
} chunk_info;

constexpr chunk_info

split_evenly(size_t N, size_t threads)
{
    return { N / threads + 1, N / threads, N % threads };
}


std::pair<size_t, size_t>
get_chunk_begin_end(const chunk_info& ci, size_t index)
{
    size_t begin = 0, end = 0;
    if (index < ci.split_item) {
        begin = index * ci.large_chunk;
        end = begin + ci.large_chunk;
    }
    else {
        begin = ci.split_item * ci.large_chunk + (index - ci.split_item) * ci.small_chunk;
        end = begin + ci.small_chunk;
    }
    return std::make_pair(begin, end);
}


void pi_taylor_chunk(std::vector<my_float>& output, std::vector<my_float>& times, size_t thread_id, size_t start_step, size_t stop_step) {
    my_float sumatorio = 0;
    auto start_chunk = std::chrono::high_resolution_clock::now();
    for (size_t i = start_step; i < stop_step; i += 1) {
        int power = i & 0x1 ? -1 : 1;
        sumatorio += static_cast<my_float>(power) / (2 * i + 1);
    }
    output[thread_id] = sumatorio;
    auto stop_chunk = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = stop_chunk - start_chunk;
    times[thread_id] = elapsed_seconds.count();
}


int main() {

    // Initialization of parameters
    size_t threads = 2;
    size_t steps = 20;
    my_float pi = 0;

    std::vector<my_float> output(threads);
    std::vector<my_float> times(threads);
    std::vector<std::thread> thread_vector(threads);
    
    // Do the split of the input vector and assign numbers from that vector to each thread
    auto chunks = split_evenly(steps, threads);

    // Check that the number of steps in the series is larger than the number of threads
    if (steps < threads) {
        std::cerr << "The number of steps must be larger than the number of threads" << std::endl;
        exit(1);
    }

    // Task parallelization, each thread computes its corresponding values of the Taylor series
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < threads; ++i) {
        auto begin_end = get_chunk_begin_end(chunks, i);
        thread_vector[i] = std::thread(pi_taylor_chunk, std::ref(output), std::ref(times), i, begin_end.first, begin_end.second);
        std::cout << "The thread " << i + 1 << " computed the sum from the element " << begin_end.first << " to " << begin_end.second << std::endl;
    }

    // Wait for all threads to finish their task
    for (size_t i = 0; i < threads; ++i) {
        thread_vector[i].join();
    }

    // Sum all terms of each thread computed to approximate pi and print the execution time of each thread
    my_float max_time = 0, min_time = 100;
    for (size_t i = 0; i < threads; i++) {
        pi += output[i];
        std::cout << "" << std::endl;
        std::cout << "Thread ID: " << i + 1 << ". Execution time: " << times[i] << std::endl;
        if (times[i] < min_time) min_time = times[i];
        if (times[i] > max_time) max_time = times[i];
    }
    std::cout << "The maximum time of the execution times of the threads was: " << max_time << std::endl;
    std::cout << "The minimum time of the execution times of the threads was: " << min_time << std::endl;
    std::cout << "The ratio was: " << max_time << "/" << min_time << " = " << max_time / min_time << std::endl;

    pi = pi * 4;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "" << std::endl;
    std::cout << "Execution time: " << elapsed_seconds.count() << "s\n";
    std::cout << "" << std::endl;
    std::cout << "For " << steps << ", pi value: " << std::setprecision(std::numeric_limits<long double>::digits10 + 1) << pi << std::endl;
}
