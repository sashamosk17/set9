#pragma once
#include <string>
#include <vector>
#include <functional>

struct TestResult {
  int size;
  std::string type;
  std::string algorithm;
  long long comparisons;
  double timeMs;
};

void resetComparisons();
long long getComparisons();
int compareWithCount(const std::string& a, const std::string& b);

void standardQuicksort(std::vector<std::string>& arr);
void standardMergesort(std::vector<std::string>& arr);

void ternaryStringQuicksort(std::vector<std::string>& arr);
void stringMergesort(std::vector<std::string>& arr);
void msdRadixSort(std::vector<std::string>& arr);
void msdRadixSortHybrid(std::vector<std::string>& arr);

class StringSortTester {
 public:
  static void setNumRuns(int runs);
  static void runAllTests();
  static void saveResults(const std::string& filename, const std::vector<TestResult>& results);

 private:
  static int numRuns;
  static void testStandardQuicksort();
  static void testStandardMergesort();
  static void testStringQuicksort();
  static void testStringMergesort();
  static void testMSDRadixSort();
  static void testMSDRadixSortHybrid();

  static TestResult runSingleTest(const std::vector<std::string>& data, const std::string& type, const std::string& algorithm,
                                  std::function<void(std::vector<std::string>&)> sortFunc);
};