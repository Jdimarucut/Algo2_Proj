#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>

// Function to perform Fibonacci Search
int fibonacciSearch(const std::vector<double>& arr, double x, int left, int right) {
    int n = right - left + 1;
    int fibMMm2 = 0; // (m-2)'th Fibonacci Number
    int fibMMm1 = 1; // (m-1)'th Fibonacci Number
    int fibM = fibMMm2 + fibMMm1; // m'th Fibonacci Number

    while (fibM < n) {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    int offset = -1;

    while (fibM > 1) {
        int i = std::min(offset + fibMMm2, n - 1);
        if (arr[left + i] < x) {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        } else if (arr[left + i] > x) {
            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        } else {
            return left + i;
        }
    }

    if (fibMMm1 && arr[left + offset + 1] == x) {
        return left + offset + 1;
    }

    return -1; // Element not found
}

// Function to perform Interpolation Search
int interpolationSearch(const std::vector<double>& arr, double x, int left, int right) {
    while (left <= right && x >= arr[left] && x <= arr[right]) {
        if (left == right) {
            if (arr[left] == x) return left;
            return -1;
        }

        // Estimate the position using interpolation formula
        int pos = left + ((double)(x - arr[left]) * (right - left) / (arr[right] - arr[left]));

        if (arr[pos] == x) {
            return pos;
        }

        if (arr[pos] < x) {
            left = pos + 1;
        } else {
            right = pos - 1;
        }
    }
    return -1; // Element not found within the bounds
}

int hybridSearch(const std::vector<double>& arr, double x, int low, int high, 
                 std::chrono::duration<double>& runtime,
                 int initialLow = 0, int initialHigh = -1) {

    if (initialHigh == -1) { 
        initialHigh = arr.size() - 1; }

     if (arr.empty() || x < arr[low] || x > arr[high]) {
        runtime = std::chrono::duration<double>::zero(); // No runtime if invalid input
        return -1; // Not found
    }
        
    
    auto start = std::chrono::high_resolution_clock::now();

    if (low <= high && x >= arr[low] && x <= arr[high]) { // Valid search range
        // Ensure mid is within bounds using std::clamp
        int mid = std::clamp(static_cast<int>(low + ((x - arr[low]) * (high - low)) / (arr[high] - arr[low])), low, high);
        if (arr[mid] == x) {
            auto end = std::chrono::high_resolution_clock::now();
            runtime = end - start;
            return mid; 
        }

        double expectedMid = (double)(x - arr[initialLow]) / (arr[initialHigh] - arr[initialLow]) * (initialHigh - initialLow) + initialLow;
        double deviation = std::abs(expectedMid - mid);

        if (deviation < (initialHigh - initialLow) * 0.2) { 
            if (arr[mid] < x) {
                return hybridSearch(arr, x, mid + 1, high, runtime, initialLow, initialHigh);
            } else {
                return hybridSearch(arr, x, low, mid - 1, runtime, initialLow, initialHigh);
            }
        } else {  
            return fibonacciSearch(arr, x, low, high);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    runtime = end - start;
    return -1; // Not found
}



int main() {
    std::random_device rd;
    std::mt19937 gen(rd()); // Seed the random number generator

    std::vector<int> arraySizes = {1000, 10000, 100000, 1000000};
    std::vector<std::string> distributionTypes = {"uniform", "random"}; // Or add more types
    const int NUM_TRIALS = 10; // Number of trials per size and distribution

    std::cout << std::setw(12) << "Array Size" << std::setw(15) << "Distribution" 
              << std::setw(12) << "Avg. Runtime" << std::setw(12) << "Min. Runtime"
              << std::setw(12) << "Max. Runtime" << std::endl;

    for (int size : arraySizes) {
        for (const std::string& type : distributionTypes) {
            double totalRuntime = 0.0;
            double minRuntime = std::numeric_limits<double>::max();
            double maxRuntime = 0.0;

            for (int trial = 0; trial < NUM_TRIALS; ++trial) {
                std::vector<double> arr(size);
                if (type == "uniform") {
                    std::iota(arr.begin(), arr.end(), 1); 
                } else {
                    std::uniform_real_distribution<> dis(1, 1000); 
                    std::generate(arr.begin(), arr.end(), [&]() { return dis(gen); });
                    std::sort(arr.begin(), arr.end());
                }

                double x = arr[gen() % size]; // Pick a random element
                std::chrono::duration<double> runtime;
                hybridSearch(arr, x, 0, arr.size() - 1, runtime);

                totalRuntime += runtime.count();
                minRuntime = std::min(minRuntime, runtime.count());
                maxRuntime = std::max(maxRuntime, runtime.count());
            }

            double avgRuntime = totalRuntime / NUM_TRIALS;

            std::cout << std::setw(12) << size << std::setw(15) << type
                      << std::setw(12) << std::setprecision(8) << avgRuntime 
                      << std::setw(12) << std::setprecision(8) << minRuntime
                      << std::setw(12) << std::setprecision(8) << maxRuntime << std::endl;
        }
    }

    return 0;
}