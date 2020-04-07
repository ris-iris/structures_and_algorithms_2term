#include <iostream>
#include <vector>
#include <queue>

#define INF 1000000000000000

long long dijkstra(std::vector<std::vector<std::pair<int, int> > > g, int n, int start, int end, int l) {
  long long dp[n];
  int len[n];
  std::priority_queue<std::pair<int, int> > q;
  for (int i = 0; i < n; ++i) {
    dp[i] = INF;
    len[i] = 0;
  }
  dp[start] = 0;
  q.push(std::make_pair(0, start));
  while (!q.empty()) {
    int v = q.top().second;
    int temp_dp = -q.top().first;
    q.pop();
    if (temp_dp <= dp[v]) {
      for (auto w = g[v].begin(); w != g[v].end(); ++w) {
        if (dp[v] + w->second < dp[w->first]) {
          dp[w->first] = dp[v] + w->second;
          len[w->first] = len[v] + 1;
          q.push(std::make_pair(-dp[w->first], w->first));
        }
      }
    }
  }
  return len[end] <= l ? dp[end] : -1;
}

int main() {
  int n, m, k, s, f, a, b, c;
  std::cin >> n >> m >> k >> s >> f;
  std::vector<std::vector<std::pair<int, int> > > graph(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b >> c;
    graph[a - 1].push_back(std::make_pair(b - 1, c));
  }

  std::cout << dijkstra(graph, n, s - 1, f - 1, k);
  return 0;
}