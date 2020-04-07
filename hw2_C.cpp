#include <iostream>
#include <vector>
#include <queue>

#define INF 1000000000000000

double dijkstra(std::vector<std::vector<std::pair<int, double> > > g, int n, int start, int end) {
  double dp[n];
  std::priority_queue<std::pair<double, int> > q;
  for (int i = 0; i < n; ++i) {
    dp[i] = INF;
  }
  dp[start] = 0;
  q.push(std::make_pair(0, start));
  while (!q.empty()) {
    int v = q.top().second;
    double temp_dp = -q.top().first;
    q.pop();
    if (temp_dp <= dp[v]) {
      for (auto w = g[v].begin(); w != g[v].end(); ++w) {
        if (dp[v] + w->second - dp[v] * w->second < dp[w->first]) {
          dp[w->first] = dp[v] + w->second - dp[v] * w->second;
          q.push(std::make_pair(-dp[w->first], w->first));
        }
      }
    }
  }
  return dp[end];
}

int main() {
  int n, m, s, f, a, b;
  double c;
  std::cin >> n >> m >> s >> f;
  std::vector<std::vector<std::pair<int, double> > > graph(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b >> c;
    graph[a - 1].push_back(std::make_pair(b - 1, c / 100));
    graph[b - 1].push_back(std::make_pair(a - 1, c / 100));
  }

  std::cout << dijkstra(graph, n, s - 1, f - 1);
  return 0;
}