#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip> // for fixed and setprecision

using namespace std;
using namespace chrono;

/* =====================================================
   SORTING ALGORITHMS
   ===================================================== */

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[minIndex])
                minIndex = j;
        swap(arr[i], arr[minIndex]);
    }
}

void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; j++)
        if (arr[j] < pivot) swap(arr[++i], arr[j]);
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/* =====================================================
   DATA GENERATION
   ===================================================== */

vector<int> generateRandomData(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++) arr[i] = rand();
    return arr;
}

vector<int> generateSortedData(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++) arr[i] = i;
    return arr;
}

vector<int> generateReverseData(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++) arr[i] = size - i;
    return arr;
}

/* =====================================================
   PERFORMANCE MEASUREMENT
   ===================================================== */

struct Result {
    double timeSec;
    double memoryMB;
};

// Estimate memory usage for each algorithm
double estimateMemoryMB(const string& algo, int n) {
    size_t bytes = 0;
    if (algo == "Bubble" || algo == "Selection" || algo == "Insertion") {
        bytes = n * sizeof(int); // in-place sort
    }
    else if (algo == "Merge") {
        bytes = n * sizeof(int) * 2; // original + temp arrays
    }
    else if (algo == "Quick") {
        bytes = n * sizeof(int); // array
        bytes += log2(n) * sizeof(int); // recursion stack
    }
    return bytes / (1024.0 * 1024.0); // MB
}

Result measurePerformance(void (*sortFunc)(vector<int>&), vector<int> arr, const string& algoName) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto stop = high_resolution_clock::now();
    double elapsedSec = duration<double>(stop - start).count(); // seconds
    double memMB = estimateMemoryMB(algoName, arr.size());
    return { elapsedSec, memMB };
}

/* =====================================================
   PRINT ARRAY
   ===================================================== */

void printArray(const vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++)
        cout << arr[i] << " ";
    cout << endl;
}

/* =====================================================
   MAIN FUNCTION
   ===================================================== */

int main() {
    srand(time(0));
    char again;

    do {
        int size;
        cout << "Enter array size (up to 100000): ";
        cin >> size;

        int typeChoice;
        cout << "Select array type:\n1. Random\n2. Sorted\n3. Reverse\nEnter choice: ";
        cin >> typeChoice;

        vector<int> data;
        if (typeChoice == 1) data = generateRandomData(size);
        else if (typeChoice == 2) data = generateSortedData(size);
        else if (typeChoice == 3) data = generateReverseData(size);
        else {
            cout << "Invalid choice. Exiting.\n";
            return 0;
        }

        cout << "\nOriginal Array:\n";
        if (size <= 50) printArray(data);
        else cout << "(Array too large to display)\n";

        vector<int> arrCopy;

        cout << fixed << setprecision(6); // readable decimal for s and MB

        // Bubble Sort
        arrCopy = data;
        cout << "\nBubble Sort:\n";
        Result res = measurePerformance(bubbleSort, arrCopy, "Bubble");
        if (size <= 50) printArray(arrCopy);
        cout << "Time: " << res.timeSec << " s | Memory: " << res.memoryMB << " MB\n";

        // Selection Sort
        arrCopy = data;
        cout << "\nSelection Sort:\n";
        res = measurePerformance(selectionSort, arrCopy, "Selection");
        if (size <= 50) printArray(arrCopy);
        cout << "Time: " << res.timeSec << " s | Memory: " << res.memoryMB << " MB\n";

        // Insertion Sort
        arrCopy = data;
        cout << "\nInsertion Sort:\n";
        res = measurePerformance(insertionSort, arrCopy, "Insertion");
        if (size <= 50) printArray(arrCopy);
        cout << "Time: " << res.timeSec << " s | Memory: " << res.memoryMB << " MB\n";

        // Merge Sort
        arrCopy = data;
        res = measurePerformance([](vector<int>& a) { mergeSort(a, 0, a.size() - 1); }, arrCopy, "Merge");
        cout << "\nMerge Sort:\n";
        if (size <= 50) printArray(arrCopy);
        cout << "Time: " << res.timeSec << " s | Memory: " << res.memoryMB << " MB\n";

        // Quick Sort
        arrCopy = data;
        res = measurePerformance([](vector<int>& a) { quickSort(a, 0, a.size() - 1); }, arrCopy, "Quick");
        cout << "\nQuick Sort:\n";
        if (size <= 50) printArray(arrCopy);
        cout << "Time: " << res.timeSec << " s | Memory: " << res.memoryMB << " MB\n";

        cout << "\nDo you want to run another round? (y/n): ";
        cin >> again;
        cout << endl;

    } while (again == 'y' || again == 'Y');

    return 0;
}
