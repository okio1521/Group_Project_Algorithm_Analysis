#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

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

/* ================= MERGE SORT ================= */

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

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

/* ================= QUICK SORT (FIXED) ================= */
/* Pivot changed to middle element to avoid stack overflow */

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[(low + high) / 2];
    int i = low, j = high;

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    return i;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi, high);
    }
}

/* =====================================================
   DATA GENERATION
   ===================================================== */

vector<int> generateRandomData(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++)
        arr[i] = i + 1;
    random_shuffle(arr.begin(), arr.end());
    return arr;
}

vector<int> generateSortedData(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++)
        arr[i] = i + 1;
    return arr;
}

vector<int> generateReverseData(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++)
        arr[i] = size - i;
    return arr;
}

/* =====================================================
   PERFORMANCE MEASUREMENT
   ===================================================== */

struct Result {
    double timeSec;
    double memoryMB;
};

double estimateMemoryMB(const string& algo, int n) {
    size_t bytes = 0;

    if (algo == "Bubble" || algo == "Selection" || algo == "Insertion") {
        bytes = n * sizeof(int);
    }
    else if (algo == "Merge") {
        bytes = n * sizeof(int) * 2;
    }
    else if (algo == "Quick") {
        bytes = n * sizeof(int);
        bytes += log2(n) * sizeof(int);
    }

    return bytes / (1024.0 * 1024.0);
}

Result measurePerformance(void (*sortFunc)(vector<int>&),
    vector<int>& arr,
    const string& algoName) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto stop = high_resolution_clock::now();

    double elapsed = duration<double>(stop - start).count();
    double mem = estimateMemoryMB(algoName, arr.size());

    return { elapsed, mem };
}

/* =====================================================
   PRINT ARRAY
   ===================================================== */

void printArray(const vector<int>& arr) {
    for (int v : arr)
        cout << v << " ";
    cout << endl;
}

/* =====================================================
   MAIN
   ===================================================== */

int main() {
    srand(time(0));
    char again;

    do {
        int size;
        cout << "Enter array size (up to 100000): ";
        cin >> size;

        int choice;
        cout << "Select array type:\n";
        cout << "1. Random\n2. Sorted\n3. Reverse\n";
        cout << "Enter choice: ";
        cin >> choice;

        vector<int> data;
        if (choice == 1) data = generateRandomData(size);
        else if (choice == 2) data = generateSortedData(size);
        else if (choice == 3) data = generateReverseData(size);
        else {
            cout << "Invalid choice.\n";
            return 0;
        }

        cout << "\nOriginal Array:\n";
        if (size <= 50) printArray(data);
        else cout << "(Array too large to display)\n";

        vector<int> arrCopy;
        cout << fixed << setprecision(6);

        arrCopy = data;
        cout << "\nBubble Sort:\n";
        Result res = measurePerformance(bubbleSort, arrCopy, "Bubble");
        if (size <= 50) printArray(arrCopy);
        cout << "Time: " << res.timeSec << " s | Memory: " << res.memoryMB << " MB\n";

        arrCopy = data;
        cout << "\nSelection Sort:\n";
        res = measurePerformance(selectionSort, arrCopy, "Selection");
        if (size <= 50) printArray(arrCopy);
        cout << "Time: " << res.timeSec << " s | Memory: " << res.memoryMB << " MB\n";

        arrCopy = data;
        cout << "\nInsertion Sort:\n";
        res = measurePerformance(insertionSort, arrCopy, "Insertion");
        if (size <= 50) printArray(arrCopy);
        cout << "Time: " << res.timeSec << " s | Memory: " << res.memoryMB << " MB\n";

        arrCopy = data;
        cout << "\nMerge Sort:\n";
        res = measurePerformance([](vector<int>& a) {
            mergeSort(a, 0, a.size() - 1);
            }, arrCopy, "Merge");
        if (size <= 50) printArray(arrCopy);
        cout << "Time: " << res.timeSec << " s | Memory: " << res.memoryMB << " MB\n";

        arrCopy = data;
        cout << "\nQuick Sort:\n";
        res = measurePerformance([](vector<int>& a) {
            quickSort(a, 0, a.size() - 1);
            }, arrCopy, "Quick");
        if (size <= 50) printArray(arrCopy);
        cout << "Time: " << res.timeSec << " s | Memory: " << res.memoryMB << " MB\n";

        cout << "\nDo you want to run another round? (y/n): ";
        cin >> again;
        cout << endl;

    } while (again == 'y' || again == 'Y');

    return 0;
}
