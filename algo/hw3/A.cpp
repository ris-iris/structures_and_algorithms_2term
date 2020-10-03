#include <iostream>
#include <vector>
#include <queue>

#define INF 1000000000
void getEdges(int m, std::vector<std::vector<std::pair<int, int> > >& graph){
  int a, b, w;
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b >> w;
    --a;
    --b;
    graph[a].push_back(std::make_pair(b, w));
    graph[b].push_back(std::make_pair(a, w));
  }
}

int mstWeight(int n, int m, std::vector<std::vector<std::pair<int, int> > >& graph){
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >, std::greater<std::pair<int, int> > > q;
  std::vector<int> par(n);
  std::vector<bool> used(n);
  std::vector<int> key(n);
  for (int i = 1; i < n; ++i) {
    used[i] = false;
    par[i] = -1;
    key[i] = INF;
    q.push(std::make_pair(INF, i));
  }
  key[0] = 0;
  used[0] = true;
  par[0] = -1;
  q.push(std::make_pair(0, 0));
  while(!q.empty()){
    std::pair<int, int> m = q.top();
    q.pop();
    used[m.second] = true;

    for (int i = 0; i < graph[m.second].size(); ++i) {
      int to = graph[m.second][i].first;
      if(!used[to] && key[to] > graph[m.second][i].second){
        par[to] = m.second;
        key[to] = graph[m.second][i].second;
        q.push(std::make_pair(key[to], to));
      }
    }
  }
  int ans = 0;
  for (int i = 1; i < n; ++i) {
    ans += key[i];
  }
  return ans;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<std::vector<std::pair<int, int> > > graph(n);
  getEdges(m, graph);
  std::cout << mstWeight(n, m, graph);
  return 0;
}
