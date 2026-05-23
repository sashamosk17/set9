#pragma once
#include <string>
#include <vector>
#include <random>

class StringGenerator {
 public:
  static const std::string ALPHABET;

  static std::vector<std::string> generateRandom(int size);
  static std::vector<std::string> generateReversed(int size);
  static std::vector<std::string> generateNearlySorted(int size, int swaps = 10);
  static std::vector<std::string> getSubArray(const std::vector<std::string>& source, int newSize);
  static std::vector<std::string> getFullRandomArray();
  static std::vector<std::string> getFullReversedArray();
  static std::vector<std::string> getFullNearlySortedArray();
  static std::vector<std::string> generateWithCommonPrefix(int size, int prefixLen);

 private:
  static std::string generateOneRandomString();
  static std::mt19937& getRng();
};