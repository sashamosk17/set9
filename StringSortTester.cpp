#include "StringSortTester.h"
#include "StringGenerator.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <random>
#include <vector>
#include <string>

int StringSortTester::numRuns = 5;
static long long g_comparisonCount = 0;

void resetComparisons() {
  g_comparisonCount = 0;
}
long long getComparisons() {
  return g_comparisonCount;
}

int compareWithCount(const std::string& a, const std::string& b) {
  size_t i = 0;
  size_t minSize = std::min(a.size(), b.size());
  while (i < minSize) {
    g_comparisonCount++;
    if (a[i] < b[i])
      return -1;
    if (a[i] > b[i])
      return 1;
    i++;
  }
  g_comparisonCount++;
  if (a.size() == b.size())
    return 0;
  return (a.size() < b.size()) ? -1 : 1;
}

static int stdPartition(std::vector<std::string>& arr, int low, int high) {
  std::string pivot = arr[low + (high - low) / 2];
  int i = low - 1, j = high + 1;
  while (true) {
    do {
      i++;
    } while (compareWithCount(arr[i], pivot) < 0);
    do {
      j--;
    } while (compareWithCount(arr[j], pivot) > 0);
    if (i >= j)
      return j;
    std::swap(arr[i], arr[j]);
  }
}

static void stdQuickImpl(std::vector<std::string>& arr, int low, int high) {
  if (low < high) {
    int pi = stdPartition(arr, low, high);
    stdQuickImpl(arr, low, pi);
    stdQuickImpl(arr, pi + 1, high);
  }
}

void standardQuicksort(std::vector<std::string>& arr) {
  if (!arr.empty())
    stdQuickImpl(arr, 0, arr.size() - 1);
}

static void stdMerge(std::vector<std::string>& arr, int left, int mid, int right) {
  int n1 = mid - left + 1, n2 = right - mid;
  std::vector<std::string> L(n1), R(n2);
  for (int i = 0; i < n1; i++){
    L[i] = arr[left + i];
  }
  for (int j = 0; j < n2; j++){
    R[j] = arr[mid + 1 + j];
  }
  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (compareWithCount(L[i], R[j]) <= 0){arr[k++] = L[i++];}
    else{
      arr[k++] = R[j++];
    }
  }
  while (i < n1){arr[k++] = L[i++];}
  while (j < n2){
    arr[k++] = R[j++];}
}

static void stdMergeImpl(std::vector<std::string>& arr, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    stdMergeImpl(arr, left, mid);
    stdMergeImpl(arr, mid + 1, right);
    stdMerge(arr, left, mid, right);
  }
}

void standardMergesort(std::vector<std::string>& arr) {
  if (!arr.empty())
    stdMergeImpl(arr, 0, arr.size() - 1);
}

TestResult StringSortTester::runSingleTest(const std::vector<std::string>& data, const std::string& type,
const std::string& algorithm,
std::function<void(std::vector<std::string>&)> sortFunc) {

  double totalTime = 0.0;
  long long totalComparisons = 0;

  for (int run = 0; run < numRuns; run++) {
    std::vector<std::string> arr = data;
    resetComparisons();
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(arr);
    auto end = std::chrono::high_resolution_clock::now();
    totalTime += std::chrono::duration<double, std::milli>(end - start).count();
    totalComparisons += getComparisons();
  }

  TestResult result;
  result.size = data.size();
  result.type = type;
  result.algorithm = algorithm;
  result.comparisons = totalComparisons / numRuns;
  result.timeMs = totalTime / numRuns;

  return result;
}

void StringSortTester::saveResults(const std::string& filename, const std::vector<TestResult>& results) {
  std::ofstream file(filename);
  file << "size,type,algorithm,comparisons,time_ms\n";
  for (const auto& r : results) {
    file << r.size << "," << r.type << "," << r.algorithm << "," << r.comparisons << "," << r.timeMs << "\n";
  }
}

static int charAt(const std::string& s, int d) {
  if (d >= (int)s.size()) return -1;
  return (unsigned char)s[d];
}

static void ternaryStringQuicksortImpl(std::vector<std::string>& arr, int lo, int hi, int d) {
  if (hi <= lo) return;

  int lt = lo, gt = hi;
  int v = charAt(arr[lo], d);
  int i = lo + 1;

  while (i <= gt) {
    int t = charAt(arr[i], d);
    g_comparisonCount++;
    if (t < v) std::swap(arr[lt++], arr[i++]);
    else if (t > v) std::swap(arr[i], arr[gt--]);
    else i++;
  }

  ternaryStringQuicksortImpl(arr, lo, lt - 1, d);
  if (v >= 0) ternaryStringQuicksortImpl(arr, lt, gt, d + 1);
  ternaryStringQuicksortImpl(arr, gt + 1, hi, d);
}

void ternaryStringQuicksort(std::vector<std::string>& arr) {
  if (!arr.empty())
    ternaryStringQuicksortImpl(arr, 0, arr.size() - 1, 0);
}

struct StringWithLcp {
  std::string str;
  int lcp;
};

static std::pair<int, int> lcpCompare(const std::string& a, const std::string& b, int k) {
  int len = k;
  int minSize = std::min(a.size(), b.size());
  while (len < minSize) {
    g_comparisonCount++;
    if (a[len] < b[len]) return {-1, len};
    if (a[len] > b[len]) return {1, len};
    len++;
  }
  g_comparisonCount++;
  if (a.size() == b.size()) return {0, len};
  return (a.size() < b.size()) ? std::make_pair(-1, len) : std::make_pair(1, len);
}

static std::vector<StringWithLcp> stringMerge(std::vector<StringWithLcp>& P, std::vector<StringWithLcp>& Q) {
  std::vector<StringWithLcp> R;
  int i = 0, j = 0;
  int m = P.size(), f = Q.size();

  while (i < m && j < f) {
    if (P[i].lcp > Q[j].lcp) {
      R.push_back(P[i]);
      i++;
    } else if (P[i].lcp < Q[j].lcp) {
      R.push_back(Q[j]);
      j++;
    } else {
      auto result = lcpCompare(P[i].str, Q[j].str, P[i].lcp);
      int cmp = result.first;
      int h = result.second;
      if (cmp <= 0) {
        R.push_back(P[i]);
        i++;
        if (j < f) Q[j].lcp = h;
      } else {
        R.push_back(Q[j]);
        j++;
        if (i < m) P[i].lcp = h;
      }
    }
  }

  while (i < m) {
    R.push_back(P[i]);
    i++;
  }
  while (j < f) {
    R.push_back(Q[j]);
    j++;
  }

  return R;
}

static std::vector<StringWithLcp> stringMergesortImpl(std::vector<std::string>& arr, int left, int right) {
  if (left == right) {
    return {{arr[left], 0}};
  }

  int mid = left + (right - left) / 2;
  auto P = stringMergesortImpl(arr, left, mid);
  auto Q = stringMergesortImpl(arr, mid + 1, right);
  return stringMerge(P, Q);
}

void stringMergesort(std::vector<std::string>& arr) {
  if (arr.empty()) return;
  auto result = stringMergesortImpl(arr, 0, arr.size() - 1);
  for (size_t i = 0; i < arr.size(); i++) {
    arr[i] = result[i].str;
  }
}

static const int R = 256;

static void msdRadixSortImpl(std::vector<std::string>& arr, int lo, int hi, int d, std::vector<std::string>& aux) {
  if (hi <= lo) return;

  std::vector<int> count(R + 2, 0);

  for (int i = lo; i <= hi; i++) {
    int c = charAt(arr[i], d);
    g_comparisonCount++;
    count[c + 2]++;
  }

  for (int r = 0; r < R + 1; r++)
    count[r + 1] += count[r];

  for (int i = lo; i <= hi; i++) {
    int c = charAt(arr[i], d);
    aux[count[c + 1]++] = arr[i];
  }

  for (int i = lo; i <= hi; i++)
    arr[i] = aux[i - lo];

  for (int r = 0; r < R; r++)
    msdRadixSortImpl(arr, lo + count[r], lo + count[r + 1] - 1, d + 1, aux);
}

void msdRadixSort(std::vector<std::string>& arr) {
  if (arr.empty()) return;
  std::vector<std::string> aux(arr.size());
  msdRadixSortImpl(arr, 0, arr.size() - 1, 0, aux);
}

static const int CUTOFF = 74;

static void msdRadixSortHybridImpl(std::vector<std::string>& arr, int lo, int hi, int d, std::vector<std::string>& aux) {
  if (hi <= lo) return;

  if (hi - lo < CUTOFF) {
    ternaryStringQuicksortImpl(arr, lo, hi, d);
    return;
  }

  std::vector<int> count(R + 2, 0);

  for (int i = lo; i <= hi; i++) {
    int c = charAt(arr[i], d);
    g_comparisonCount++;
    count[c + 2]++;
  }

  for (int r = 0; r < R + 1; r++)
    count[r + 1] += count[r];

  for (int i = lo; i <= hi; i++) {
    int c = charAt(arr[i], d);
    aux[count[c + 1]++] = arr[i];
  }

  for (int i = lo; i <= hi; i++)
    arr[i] = aux[i - lo];

  for (int r = 0; r < R; r++)
    msdRadixSortHybridImpl(arr, lo + count[r], lo + count[r + 1] - 1, d + 1, aux);
}

void msdRadixSortHybrid(std::vector<std::string>& arr) {
  if (arr.empty()) return;
  std::vector<std::string> aux(arr.size());
  msdRadixSortHybridImpl(arr, 0, arr.size() - 1, 0, aux);
}

void StringSortTester::setNumRuns(int runs) {
  numRuns = runs;
}

void StringSortTester::testStandardQuicksort() {
  std::cout << "Testing Standard Quicksort...\n";
  std::vector<TestResult> results;

  auto fullRandom = StringGenerator::getFullRandomArray();
  auto fullReversed = StringGenerator::getFullReversedArray();
  auto fullNearlySorted = StringGenerator::getFullNearlySortedArray();

  for (int size = 100; size <= 3000; size += 100) {
    auto random = StringGenerator::getSubArray(fullRandom, size);
    auto reversed = StringGenerator::getSubArray(fullReversed, size);
    auto nearlySorted = StringGenerator::getSubArray(fullNearlySorted, size);

    results.push_back(runSingleTest(random, "random", "standard_quicksort", standardQuicksort));
    results.push_back(runSingleTest(reversed, "reversed", "standard_quicksort", standardQuicksort));
    results.push_back(runSingleTest(nearlySorted, "nearly_sorted", "standard_quicksort", standardQuicksort));

    std::cout << "  Size " << size << " completed\n";
  }

  saveResults("results_standard_quicksort.csv", results);
}

void StringSortTester::testStandardMergesort() {
  std::cout << "Testing Standard Mergesort...\n";
  std::vector<TestResult> results;

  auto fullRandom = StringGenerator::getFullRandomArray();
  auto fullReversed = StringGenerator::getFullReversedArray();
  auto fullNearlySorted = StringGenerator::getFullNearlySortedArray();

  for (int size = 100; size <= 3000; size += 100) {
    auto random = StringGenerator::getSubArray(fullRandom, size);
    auto reversed = StringGenerator::getSubArray(fullReversed, size);
    auto nearlySorted = StringGenerator::getSubArray(fullNearlySorted, size);

    results.push_back(runSingleTest(random, "random", "standard_mergesort", standardMergesort));
    results.push_back(runSingleTest(reversed, "reversed", "standard_mergesort", standardMergesort));
    results.push_back(runSingleTest(nearlySorted, "nearly_sorted", "standard_mergesort", standardMergesort));

    std::cout << "  Size " << size << " completed\n";
  }

  saveResults("results_standard_mergesort.csv", results);
}

void StringSortTester::testStringQuicksort() {
  std::cout << "Testing Ternary String Quicksort...\n";
  std::vector<TestResult> results;

  auto fullRandom = StringGenerator::getFullRandomArray();
  auto fullReversed = StringGenerator::getFullReversedArray();
  auto fullNearlySorted = StringGenerator::getFullNearlySortedArray();

  for (int size = 100; size <= 3000; size += 100) {
    auto random = StringGenerator::getSubArray(fullRandom, size);
    auto reversed = StringGenerator::getSubArray(fullReversed, size);
    auto nearlySorted = StringGenerator::getSubArray(fullNearlySorted, size);

    results.push_back(runSingleTest(random, "random", "ternary_string_quicksort", ternaryStringQuicksort));
    results.push_back(runSingleTest(reversed, "reversed", "ternary_string_quicksort", ternaryStringQuicksort));
    results.push_back(runSingleTest(nearlySorted, "nearly_sorted", "ternary_string_quicksort", ternaryStringQuicksort));

    std::cout << "  Size " << size << " completed\n";
  }

  saveResults("results_string_quicksort.csv", results);
}

void StringSortTester::testStringMergesort() {
  std::cout << "Testing String Mergesort with LCP...\n";
  std::vector<TestResult> results;

  auto fullRandom = StringGenerator::getFullRandomArray();
  auto fullReversed = StringGenerator::getFullReversedArray();
  auto fullNearlySorted = StringGenerator::getFullNearlySortedArray();

  for (int size = 100; size <= 3000; size += 100) {
    auto random = StringGenerator::getSubArray(fullRandom, size);
    auto reversed = StringGenerator::getSubArray(fullReversed, size);
    auto nearlySorted = StringGenerator::getSubArray(fullNearlySorted, size);

    results.push_back(runSingleTest(random, "random", "string_mergesort", stringMergesort));
    results.push_back(runSingleTest(reversed, "reversed", "string_mergesort", stringMergesort));
    results.push_back(runSingleTest(nearlySorted, "nearly_sorted", "string_mergesort", stringMergesort));

    std::cout << "  Size " << size << " completed\n";
  }

  saveResults("results_string_mergesort.csv", results);
}

void StringSortTester::testMSDRadixSort() {
  std::cout << "Testing MSD Radix Sort...\n";
  std::vector<TestResult> results;

  auto fullRandom = StringGenerator::getFullRandomArray();
  auto fullReversed = StringGenerator::getFullReversedArray();
  auto fullNearlySorted = StringGenerator::getFullNearlySortedArray();

  for (int size = 100; size <= 3000; size += 100) {
    auto random = StringGenerator::getSubArray(fullRandom, size);
    auto reversed = StringGenerator::getSubArray(fullReversed, size);
    auto nearlySorted = StringGenerator::getSubArray(fullNearlySorted, size);

    results.push_back(runSingleTest(random, "random", "msd_radix_sort", msdRadixSort));
    results.push_back(runSingleTest(reversed, "reversed", "msd_radix_sort", msdRadixSort));
    results.push_back(runSingleTest(nearlySorted, "nearly_sorted", "msd_radix_sort", msdRadixSort));

    std::cout << "  Size " << size << " completed\n";
  }

  saveResults("results_msd_radix.csv", results);
}

void StringSortTester::testMSDRadixSortHybrid() {
  std::cout << "Testing MSD Radix Sort Hybrid...\n";
  std::vector<TestResult> results;

  auto fullRandom = StringGenerator::getFullRandomArray();
  auto fullReversed = StringGenerator::getFullReversedArray();
  auto fullNearlySorted = StringGenerator::getFullNearlySortedArray();

  for (int size = 100; size <= 3000; size += 100) {
    auto random = StringGenerator::getSubArray(fullRandom, size);
    auto reversed = StringGenerator::getSubArray(fullReversed, size);
    auto nearlySorted = StringGenerator::getSubArray(fullNearlySorted, size);

    results.push_back(runSingleTest(random, "random", "msd_radix_sort_hybrid", msdRadixSortHybrid));
    results.push_back(runSingleTest(reversed, "reversed", "msd_radix_sort_hybrid", msdRadixSortHybrid));
    results.push_back(runSingleTest(nearlySorted, "nearly_sorted", "msd_radix_sort_hybrid", msdRadixSortHybrid));

    std::cout << "  Size " << size << " completed\n";
  }

  saveResults("results_msd_radix_hybrid.csv", results);
}

void StringSortTester::runAllTests() {
  testStandardQuicksort();
  testStandardMergesort();
  testStringQuicksort();
  testStringMergesort();
  testMSDRadixSort();
  testMSDRadixSortHybrid();
}