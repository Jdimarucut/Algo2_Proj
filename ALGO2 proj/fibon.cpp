#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <numeric>
#include <iomanip>

using namespace std;
using namespace chrono;

// Function to generate a random array with a uniform distribution
vector<int> generateUniformArray(int size) {
    vector<int> array(size);
    iota(array.begin(), array.end(), 1); // Fill with 1, 2, 3, ..., size
    return array;
}

// Function to generate a random array with a non-uniform distribution
vector<int> generateNonUniformArray(int size) {
    vector<int> array(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, size * 10);
    for (int i = 0; i < size; ++i) {
        array[i] = dis(gen);
    }
    sort(array.begin(), array.end()); // Array needs to be sorted for Fibonacci Search
    return array;
}

// Function to perform Fibonacci Search
int fibonacciSearch(const vector<int>& arr, int x) {
    int n = arr.size();
    int fibMMm2 = 0;  // (m-2)'th Fibonacci No.
    int fibMMm1 = 1;  // (m-1)'th Fibonacci No.
    int fibM = fibMMm2 + fibMMm1;  // m'th Fibonacci

    while (fibM < n) {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    int offset = -1;

    while (fibM > 1) {
        int i = min(offset + fibMMm1, n - 1);

        if (arr[i] < x) {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        } else if (arr[i] > x) {
            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        } else {
            return i;
        }
    }

    if (fibMMm1 && arr[offset + 1] == x) {
        return offset + 1;
    }

    return -1;
}

// Function to measure the runtime of Fibonacci Search
void measureFibonacciSearch(int arraySize, int numTrials, const string& distributionType) {
    vector<double> runtimes;
    for (int i = 0; i < numTrials; ++i) {
        vector<int> array;
        if (distributionType == "uniform") {
            array = generateUniformArray(arraySize);
        } else {
            array = generateNonUniformArray(arraySize);
        }
        int target = array[arraySize / 2]; // Target is the middle element for consistency

        auto start = high_resolution_clock::now();
        fibonacciSearch(array, target);
        auto end = high_resolution_clock::now();
        double duration = duration_cast<nanoseconds>(end - start).count();
        runtimes.push_back(duration);
    }

    double avgRuntime = accumulate(runtimes.begin(), runtimes.end(), 0.0) / runtimes.size();
    double minRuntime = *min_element(runtimes.begin(), runtimes.end());
    double maxRuntime = *max_element(runtimes.begin(), runtimes.end());

    cout << setw(12) << arraySize << setw(15) << distributionType
         << setw(15) << avgRuntime << " ns" << setw(15) << minRuntime << " ns"
         << setw(15) << maxRuntime << " ns" << endl;
}

int main() {
    vector<int> arraySizes = {100, 1000, 10000, 100000}; // Different array sizes
    int numTrials = 100; // Number of trials for each array size
    vector<string> distributionTypes = {"uniform", "nonuniform"}; // Distribution types

    cout << setw(12) << "Array Size" << setw(15) << "Distribution"
         << setw(15) << "Avg. Runtime" << setw(15) << "Min. Runtime"
         << setw(15) << "Max. Runtime" << endl;

    for (int size : arraySizes) {
        for (const string& type : distributionTypes) {
            measureFibonacciSearch(size, numTrials, type);
        }
    }

    return 0;
}
