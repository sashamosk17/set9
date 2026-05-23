#include <iostream>
#include <vector>
#include <string>
using namespace std;

static long long g_comparisonCount = 0;

int charAt(const string& s, int d) {
  if (d >= (int)s.size()) return -1;
  return (unsigned char)s[d];
}

void ternaryStringQuicksort(vector<string>& arr, int lo, int hi, int d) {
  if (hi <= lo) return;

  int lt = lo, gt = hi;
  int v = charAt(arr[lo], d);
  int i = lo + 1;

  while (i <= gt) {
    int t = charAt(arr[i], d);
    g_comparisonCount++;
    if (t < v) swap(arr[lt++], arr[i++]);
    else if (t > v) swap(arr[i], arr[gt--]);
    else i++;
  }

  ternaryStringQuicksort(arr, lo, lt - 1, d);
  if (v >= 0) ternaryStringQuicksort(arr, lt, gt, d + 1);
  ternaryStringQuicksort(arr, gt + 1, hi, d);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  cin.ignore();

  vector<string> arr(n);
  for (int i = 0; i < n; i++) {
    getline(cin, arr[i]);
  }

  ternaryStringQuicksort(arr, 0, n - 1, 0);

  for (const auto& s : arr) {
    cout << s << '\n';
  }

  return 0;
}
