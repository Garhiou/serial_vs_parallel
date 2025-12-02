#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <omp.h>
#include <chrono>


int main() {

    const int n = 10000;

    std::vector<float> input(n), output(n);

    const int availble_processors = omp_get_num_procs();
    std::cout << "Available processors: " << availble_processors << std::endl;

    const int num_threads = std::max(1, availble_processors -  2); // leave some processors free
    std::cout << "Using threads: " << num_threads << std::endl;

    omp_set_num_threads(num_threads); // set number of threads to use

    // Initialize input data
    for (int i = 0; i < n; ++i) {
        input[i] = static_cast<float>(std::rand()) / RAND_MAX;
    }

    // Parallel copy

    std::cout << "Starting parallel copy..." << std::endl;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for
        for (int i = 0; i < n; ++i) {
            output[i] = input[i];
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Parallel copy completed in " << elapsed_seconds.count() << " seconds." << std::endl;

    // Verify the copy
    bool success = true;
    for (int i = 0; i < n; i++) {
        if (output[i] != input[i]) {
            success = false;
            break;
        }
    }

    if (success) {
        std::cout << "\033[32mArrays are the same.\033[0m" << std::endl;
    } else {
        std::cout << "\033[31mArrays differ!\033[0m" << std::endl;
    }

    return 0;

    // To compile: g++ parallel_copy.cpp -std=c++20 -O3 -fopenmp -o parallelcopy and run: ./parallelcopy


}