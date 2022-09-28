// Paint by Rectangles
// http://www.usaco.org/index.php?page=viewproblem2&cpid=1212
#include <bits/stdc++.h>
using namespace std;

const int N = 100000;

int ds[N];

int find(int i) {
  return ds[i] < 0 ? i : ds[i] = find(ds[i]);
}
int join(int i, int j) {
  i = find(i), j = find(j);
  if (i != j) {
    if (ds[i] > ds[j])
      swap(i, j);
    ds[i] += ds[j], ds[j] = i;
  }
  return i;
}

int id[N * 2], st[N * 2 * 4], lz[N * 2 * 4];

void pull(int k, int, int) {
  int lc = k << 1 | 0, rc = k << 1 | 1;
  st[k] = st[lc] + st[rc];
}
void put(int k, int l, int r, int x) {
  if (st[k] > 0) {
    if (l == r)
      join(id[l], x);
    else
      if (lz[k] == -1)
        lz[k] = x;
      else
        lz[k] = join(lz[k], x);
  }
}
void push(int k, int l, int r) {
  if (lz[k] != -1) {
    if (l != r) {
      int m = (l + r) / 2;
      put(k << 1 | 0, l, m, lz[k]);
      put(k << 1 | 1, m + 1, r, lz[k]);
    }
    lz[k] = -1;
  }
}
void build(int k, int l, int r) {
  lz[k] = -1;
  if (l != r) {
    int m = (l + r) / 2;
    build(k << 1 | 0, l, m);
    build(k << 1 | 1, m + 1, r);
  }
}
void update(int k, int l, int r, int i, int x) {
  if (l == r)
    st[k] += x;
  else {
    int m = (l + r) / 2;
    push(k, l, r);
    if (i <= m)
      update(k << 1 | 0, l, m, i, x);
    else
      update(k << 1 | 1, m + 1, r, i, x);
    pull(k, l, r);
  }
}
void update(int k, int l, int r, int ql, int qr, int x) {
  if (ql <= l && qr >= r)
    put(k, l, r, x);
  else if (qr >= l && ql <= r) {
    int m = (l + r) / 2;
    push(k, l, r);
    update(k << 1 | 0, l, m, ql, qr, x);
    update(k << 1 | 1, m + 1, r, ql, qr, x);
  }
}

int ft[N * 2];

void update(int n, int i, int x) {
  while (i < n) {
    ft[i] += x;
    i |= i + 1;
  }
}
int query(int i) {
  int sum = 0;
  while (i >= 0) {
    sum += ft[i];
    i &= i + 1, --i;
  }
  return sum;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  static tuple<int, int, int, int> ev[N * 2];
  static vector<int> xx[N];
  static int x1[N], x2[N], flipped[N];
  long long total_black, total_white;
  int n, t;
  cin >> n >> t;
  for (int i = 0; i < n; i++) {
    int y1, y2;
    cin >> x1[i] >> y1 >> x2[i] >> y2, x1[i]--, y1--, x2[i]--, y2--;
    ev[x1[i]] = {+1, y1, y2, i};
    ev[x2[i]] = {-1, y1, y2, i};
    id[y1] = id[y2] = i;
  }
  memset(ds, -1, n * sizeof *ds);
  build(1, 0, n * 2 - 1);
  for (int x = 0; x < 2 * n; x++) {
    auto [t, y1, y2, i] = ev[x];
    if (t == +1)
      update(1, 0, n * 2 - 1, y1, y2, i);
    update(1, 0, n * 2 - 1, y1, t);
    update(1, 0, n * 2 - 1, y2, t);
  }
  memset(flipped, -1, n * sizeof *flipped);
  for (int x = 0; x < 2 * n; x++) {
    auto [t, y1, y2, i] = ev[x];
    if (flipped[find(i)] == -1)
      flipped[find(i)] = query(y1) % 2;
    update(n * 2, y1, t), update(n * 2, y2, t);
  }
  for (int i = 0; i < n; i++) {
    xx[find(i)].push_back(x1[i]);
    xx[find(i)].push_back(x2[i]);
  }
  total_black = 0, total_white = 1;
  for (int i = 0; i < n; i++)
    if (!xx[i].empty()) {
      long long all, black;
      sort(xx[i].begin(), xx[i].end());
      all = 1, black = 0;
      for (int x : xx[i]) {
        auto [t, y1, y2, i] = ev[x];
        int l, r;
        if (t == +1) {
          update(n * 2, y1, +1), update(n * 2, y2, +1);
          l = query(y1), r = query(y2);
          all += r - l - 1, black += r / 2 - l / 2;
        } else {
          l = query(y1), r = query(y2);
          all += r - l - 1, black += r / 2 - l / 2 - 1;
          update(n * 2, y1, -1), update(n * 2, y2, -1);
        }
      }
      if (!flipped[i])
        total_white += all - black, total_black += black;
      else
        total_white += black, total_black += all - black;
    }
  if (t == 1)
    cout << total_white + total_black << '\n';
  else
    cout << total_white << ' ' << total_black << '\n';
  return 0;
}
