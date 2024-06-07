#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <climits>
#include <numeric>
#include <iomanip>

using namespace std;
using namespace chrono;

// Function to generate a uniform array
vector<int> generateUniformArray(int size) {
    vector<int> array(size);
    iota(array.begin(), array.end(), 1); // Fill with 1, 2, 3, ..., size
    return array;
}

// Function to generate a non-uniform random array
vector<int> generateNonUniformArray(int size) {
    vector<int> array(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, size * 10);
    for (int i = 0; i < size; ++i) {
        array[i] = dis(gen);
    }
    sort(array.begin(), array.end()); // Array needs to be sorted for Interpolation Search
    return array;
}

// Function to perform Interpolation Search
int interpolationSearch(const vector<int>& arr, int x) {
    int low = 0, high = arr.size() - 1;

    while (low <= high && x >= arr[low] && x <= arr[high]) {
        if (low == high) {
            if (arr[low] == x) return low;
            return -1;
        }

        int pos = low + ((double)(high - low) / (arr[high] - arr[low]) * (x - arr[low]));

        if (arr[pos] == x)
            return pos;

        if (arr[pos] < x)
            low = pos + 1;
        else
            high = pos - 1;
    }
    return -1;
}

// Function to measure the runtime of Interpolation Search
void measureInterpolationSearch(int arraySize, int numTrials, const string& distributionType) {
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
        interpolationSearch(array, target);
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
            measureInterpolationSearch(size, numTrials, type);
        }
    }

    return 0;
}
