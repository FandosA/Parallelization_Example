#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <random>

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


// Function to determine if a number is prime or not
bool is_prime(int n)
{
    // Corner case
    if (n <= 1)
	return false;

    // Check from 2 to n-1
    for (int i = 2; i < n; i++)
	if (n % i == 0)
            return false;

    return true;
}


// Function to find the prime numbers larger than a given value in an array
void find_primes_larger(std::vector<std::vector<int>> &output, std::vector<int> vector,
			int limit_value, size_t thread_id, size_t start_step, size_t stop_step)
{
    for (size_t i = start_step; i < stop_step; i++)
	if (vector[i] > limit_value && is_prime(vector[i]))
	    output[thread_id].push_back(vector[i]);
}


int main() {

    // Initialize parmeters
    size_t threads = 2;
    int limit_value = 7;
    int vector_size = 10;
    int vector_range = 30;
    
    std::vector<std::vector<int>> output(threads);
    std::vector<std::thread> thread_vector(threads);

    // Do the split of the input vector and assign numbers from that vector to each thread
    auto chunks = split_evenly(vector_size, threads);

    // Check that the number of elements in the input array is larger than the number of threads
    if (vector_size < threads) {
	std::cerr << "The number of the input array must be larger than the number of threads" << std::endl;
	exit(1);
    }

    // Fill the vector in which the program will find the prime numbers larger than the given value randomly
    // With the command srand(time(NULL)) the seed is changed to generate different numbers each time the program is executed
    srand(time(NULL));
    std::vector<int> vector(vector_size);
    for (int i = 0; i < vector_size; ++i) {
	vector[i] = rand() % vector_range;
    }

    // Task parallelization, each thread looks for the prime values larger than the given one in its
    // corresponding set of numbers from the input array
    for (size_t i = 0; i < threads; i++) {
	auto begin_end = get_chunk_begin_end(chunks, i);
	thread_vector[i] = std::thread(find_primes_larger, std::ref(output), vector, limit_value, i, begin_end.first, begin_end.second);
    }

    // Wait for all threads to finish their task
    for (size_t i = 0; i < threads; i++) {
	thread_vector[i].join();
    }
	
    // Print the numbers from the input array
    std::cout << "The values of the vector are:  ";
    for (size_t i = 0; i < vector_size; i++) {
	if (i < vector_size - 2)
	    std::cout << vector[i] << ", ";
	else if (i == vector_size - 2)
	    std::cout << vector[i];
	else
	    std::cout << " and " << vector[i] << std::endl;
    }

    // Print the prime numbers larger than the given value found in the input array
    std::cout << "The prime values bigger than " << limit_value << " of the input array are: ";
    for (int i = 0; i < output.size(); i++)
	for (int j = 0; j < output[i].size(); j++)
	    if(output.size() == 1)
		std::cout << output[i][j] << std::endl;
	    else
		if (i < output.size() - 2)
		    std::cout << output[i][j] << ", ";
		else
		    if (j < output[i].size() - 2)
			std::cout << output[i][j] << ", ";
		    else if(j == output[i].size() - 2)
			std::cout << output[i][j];
		    else
			std::cout << " and " << output[i][j] << std::endl;
}
