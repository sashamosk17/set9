#include <iostream>
#include <vector>
#include <string>
using namespace std;

static const int R = 256;
static const int CUTOFF = 74;
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

void msdRadixSortHybrid(vector<string>& arr, int lo, int hi, int d, vector<string>& aux) {
  if (hi <= lo) return;

  if (hi - lo < CUTOFF) {
    ternaryStringQuicksort(arr, lo, hi, d);
    return;
  }

  vector<int> count(R + 2, 0);

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
    msdRadixSortHybrid(arr, lo + count[r], lo + count[r + 1] - 1, d + 1, aux);
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

  if (!arr.empty()) {
    vector<string> aux(arr.size());
    msdRadixSortHybrid(arr, 0, arr.size() - 1, 0, aux);
  }

  for (const auto& s : arr) {
    cout << s << '\n';
  }

  return 0;
}
