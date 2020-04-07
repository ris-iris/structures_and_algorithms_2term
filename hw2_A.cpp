#include <iostream>
#include <vector>
#include <queue>

#define INF 1000000000000

long long dijkstra(std::vector<std::vector<std::pair<int, int> > > g, int n, int start, int end) {
  long long dp[n];
  std::priority_queue<std::pair<int, int> > q;
  for (int i = 0; i < n; ++i) {
    dp[i] = INF;
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
          q.push(std::make_pair(-dp[w->first], w->first));
        }
      }
    }
  }
  return dp[end];
}

int main() {
  int a, b, x, y, m;
  std::cin >> a >> b >> m >> x >> y;
  std::vector<std::vector<std::pair<int, int> > > graph(m);
  for (long long i = 0; i < m; ++i) {
    graph[i].push_back(std::make_pair((i + 1) % m, a));
    graph[i].push_back(std::make_pair((i * i + 1) % m, b));
  }

  std::cout << dijkstra(graph, m, x, y);
  return 0;
}