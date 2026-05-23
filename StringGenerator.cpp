#include "StringGenerator.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

const std::string StringGenerator::ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#%:;^&*()-.";

std::mt19937& StringGenerator::getRng() {
  static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
  return rng;
}

std::string StringGenerator::generateOneRandomString() {
  static std::uniform_int_distribution<int> lenDist(10, 200);
  static std::uniform_int_distribution<int> charDist(0, ALPHABET.size() - 1);

  int length = lenDist(getRng());
  std::string result;
  result.reserve(length);

  for (int i = 0; i < length; ++i) {
    result += ALPHABET[charDist(getRng())];
  }
  return result;
}

std::vector<std::string> StringGenerator::generateRandom(int size) {
  std::vector<std::string> result;
  result.reserve(size);
  for (int i = 0; i < size; ++i) {
    result.push_back(generateOneRandomString());
  }
  return result;
}

std::vector<std::string> StringGenerator::generateReversed(int size) {
  auto arr = generateRandom(size);
  std::sort(arr.begin(), arr.end());
  std::reverse(arr.begin(), arr.end());
  return arr;
}

std::vector<std::string> StringGenerator::generateNearlySorted(int size, int swaps) {
  auto arr = generateRandom(size);
  std::sort(arr.begin(), arr.end());

  std::uniform_int_distribution<int> indexDist(0, size - 1);
  for (int i = 0; i < swaps; ++i) {
    int a = indexDist(getRng());
    int b = indexDist(getRng());
    std::swap(arr[a], arr[b]);
  }
  return arr;
}

std::vector<std::string> StringGenerator::getSubArray(const std::vector<std::string>& source, int newSize) {
  if (newSize > (int)source.size())
    newSize = source.size();
  std::vector<std::string> result(source.begin(), source.begin() + newSize);
  return result;
}

std::vector<std::string> StringGenerator::getFullRandomArray() {
  return generateRandom(3000);
}

std::vector<std::string> StringGenerator::getFullReversedArray() {
  auto arr = getFullRandomArray();
  std::sort(arr.begin(), arr.end());
  std::reverse(arr.begin(), arr.end());
  return arr;
}

std::vector<std::string> StringGenerator::getFullNearlySortedArray() {
  auto arr = getFullRandomArray();
  std::sort(arr.begin(), arr.end());
  std::uniform_int_distribution<int> indexDist(0, 2999);
  for (int i = 0; i < 50; ++i) {
    int a = indexDist(getRng());
    int b = indexDist(getRng());
    std::swap(arr[a], arr[b]);
  }
  return arr;
}

std::vector<std::string> StringGenerator::generateWithCommonPrefix(int size, int prefixLen) {
  std::vector<std::string> result;
  result.reserve(size);

  std::uniform_int_distribution<int> charDist(0, ALPHABET.size() - 1);
  std::string commonPrefix;
  for (int i = 0; i < prefixLen; ++i) {
    commonPrefix += ALPHABET[charDist(getRng())];
  }

  std::uniform_int_distribution<int> lenDist(10, 200);
  for (int i = 0; i < size; ++i) {
    int suffixLen = lenDist(getRng()) - prefixLen;
    if (suffixLen < 0) suffixLen = 10;

    std::string str = commonPrefix;
    for (int j = 0; j < suffixLen; ++j) {
      str += ALPHABET[charDist(getRng())];
    }
    result.push_back(str);
  }

  return result;
}