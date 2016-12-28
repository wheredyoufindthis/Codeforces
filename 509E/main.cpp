#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

using namespace std;

typedef int T;

vector<vector<T>> g;
vector<vector<T>> up;
vector<T> tin, tout;
int timer;
int k, n;

void dfs(int v = 0, int p = 0) {
  tin[v] = ++timer;
  up[v][0] = p;
  for (int i = 1; i <= k; ++i)
    up[v][i] = up[up[v][i - 1]][i - 1];
  for (int i = 0; i < g[v].size(); ++i) {
    int to = g[v][i];
    if (p != to) dfs(to, v);
  }
  tout[v] = ++timer;
}

bool upper (int a, int b) {
  return tin[a] <= tin[b] && tout[a] >= tout[b];
}

T lca(T u, T v) {
  if (upper(v, u)) return v;
  if (upper(u, v)) return u;
  for (int i = k; i >= 0; --i) {
    if (!upper(up[u][i], v)) {
      u = up[u][i];
    }
  }
  return up[u][0];
}

int distance(T v, T u) {
  if (u == v) return 0;
  assert(upper(v, u));
  size_t distance = 0;
  for (int i = k; i >= 0; --i) {
    if (!upper(up[u][i], v)) {
      u = up[u][i];
      distance += pow(2, i);
    }
  }
  return distance + 1;
}

int main() {
  timer = 0;
  cin >> n;
  k = 1;
  while ((1 << k) <= n)  ++k;

  g.resize(n);
  up.resize(n);
  tin.resize(n);
  tout.resize(n);
  timer = 0;

  for (auto &it: up)
    it.resize(k + 1);

  int u, v;
  for (int i = 0; i < n - 1; ++i) {
    cin >> u >> v;
    u -= 1;
    v -= 1;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  dfs();

  int m = 0;
  cin >> m;
  size_t qty;
  bool is_even;

  vector<pair<T, T>> queries(m);

  for (auto &query: queries)
    cin >> query.first >> query.second;

  for (auto &query: queries) {
    u = query.first;
    v = query.second;
    u -= 1;
    v -= 1;

    T ancestor = lca(u, v);
    is_even = ((distance(ancestor, u) + distance(ancestor, v)) % 2 == 0);

    cout << "distance ancestor u " << distance(ancestor, u) << endl;
    cout << "distance ancestor v " << distance(ancestor, v) << endl;

    if (ancestor == u || ancestor == v)
      cout << is_even;
    else if (is_even) {
      cout << distance(0, ancestor) + 1;
    }
    else cout << 0;
    cout << endl;
  }

  return 0;
}
