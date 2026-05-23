#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

struct StringWithLCP {
    std::string str;
    int lcp;
};

std::pair<int, int> lcpCompare(const std::string& a, const std::string& b, int knownLcp) {
    int i = knownLcp;
    int minSize = std::min(a.size(), b.size());
    
    while (i < minSize) {
        if (a[i] < b[i]) return {-1, i};
        if (a[i] > b[i]) return {1, i};
        i++;
    }
    
    if (a.size() == b.size()) return {0, i};
    if (a.size() < b.size()) return {-1, i};
    return {1, i};
}

std::vector<StringWithLCP> stringMerge(
    const std::vector<StringWithLCP>& P,
    const std::vector<StringWithLCP>& Q
) {
    std::vector<StringWithLCP> result;
    int i = 0, j = 0;
    int n = P.size(), m = Q.size();
    
    auto P_copy = P;
    auto Q_copy = Q;
    
    while (i < n && j < m) {
        int ki = P_copy[i].lcp;
        int pj = Q_copy[j].lcp;
        
        if (ki > pj) {
            result.push_back(P_copy[i]);
            i++;
        } else if (ki < pj) {
            result.push_back(Q_copy[j]);
            j++;
        } else {
            auto [cmp, lcp] = lcpCompare(P_copy[i].str, Q_copy[j].str, ki);
            
            if (cmp <= 0) {
                result.push_back(P_copy[i]);
                i++;
                if (j < m) Q_copy[j].lcp = lcp;
            } else {
                result.push_back(Q_copy[j]);
                j++;
                if (i < n) P_copy[i].lcp = lcp;
            }
        }
    }
    
    while (i < n) {
        result.push_back(P_copy[i]);
        i++;
    }
    
    while (j < m) {
        result.push_back(Q_copy[j]);
        j++;
    }
    
    return result;
}

std::vector<StringWithLCP> stringMergeSort(const std::vector<std::string>& arr, int left, int right) {
    if (left == right) {
        return {{arr[left], 0}};
    }
    
    int mid = (left + right) / 2;
    auto leftSorted = stringMergeSort(arr, left, mid);
    auto rightSorted = stringMergeSort(arr, mid + 1, right);
    
    return stringMerge(leftSorted, rightSorted);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n;
    std::cin >> n;
    
    std::vector<std::string> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    
    auto sorted = stringMergeSort(arr, 0, n - 1);
    
    for (const auto& item : sorted) {
        std::cout << item.str << "\n";
    }
    
    return 0;
}