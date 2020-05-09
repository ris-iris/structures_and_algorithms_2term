#include <iostream>
#include <vector>

#define INF 1000000000

class Edge {
 public:
  int to = 0;
  int cap = 0;
  int flow = 0;
  Edge *reverse = nullptr;
  Edge(int a, int b) {
    to = a;
    cap = b;
  }
  int tempCap() {
    return cap - flow;
  }
};

void getEdges(int m, std::vector<std::vector<Edge *> > &graph) {
  int a, b;
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b;
    a--;
    b--;
    Edge *e = new Edge(b, 1);
    Edge *rev = new Edge(a, 0);
    e->reverse = rev;
    rev->reverse = e;
    graph[a].push_back(e);
    graph[b].push_back(rev);
  }
}

int dfs(int s, int t, int flow, std::vector<std::vector<Edge *> > &graph, std::vector<bool> &used) {
  if (s == t) return flow;
  used[s] = true;
  for (int i = 0; i < graph[s].size(); ++i) {
    Edge *edge = graph[s][i];
    if (!used[edge->to] && edge->tempCap() > 0) {
      int flow1 = dfs(edge->to, t, std::min(edge->tempCap(), flow), graph, used);
      if (flow1 > 0) {
        edge->flow += flow1;
        edge->reverse->flow -= flow1;
        return flow1;
      }
    }
  }
  return 0;
}

void dfs(int s, int t, std::vector<std::vector<Edge *> > &graph, std::vector<int> &par, std::vector<bool> &used) {
  used[s] = true;
  if (s == t) return;
  for (int i = 0; i < graph[s].size(); ++i) {
    Edge *edge = graph[s][i];
    if (!used[edge->to] && !used[t] && edge->cap > 0 && edge->flow > 0) {
      par[edge->to] = s;
      edge->cap = 0;
      dfs(edge->to, t, graph, par, used);
    }
  }
}

void printPath(int s, int t, std::vector<std::vector<Edge *> > &graph) {
  std::vector<int> path;
  std::vector<int> par;
  std::vector<bool> used;
  used.assign(graph.size(), false);
  par.assign(graph.size(), -1);
  par[s] = -2;
  dfs(s, t, graph, par, used);
  int temp = t;
  while (temp != s) {
    path.push_back(temp);
    temp = par[temp];
  }
  path.push_back(temp);
  for (int i = path.size() - 1; i > -1; --i) {
    std::cout << path[i] + 1 << " ";
  }
  std::cout << std::endl;
}

int main() {
  int n, m, s, t;
  std::cin >> n >> m >> s >> t;
  s--;
  t--;
  std::vector<std::vector<Edge *> > graph(n);
  std::vector<bool> used(n);
  used.assign(n, false);
  getEdges(m, graph);
  int tempFlow = INF;
  int maxFlow = 0;
  do {
    tempFlow = dfs(s, t, INF, graph, used);
    maxFlow += tempFlow;
    used.assign(n, false);
  } while (tempFlow != 0 && maxFlow < 2);
  if (maxFlow < 2) {
    std::cout << "NO";
  } else {
    std::cout << "YES" << std::endl;
    printPath(s, t, graph);
    printPath(s, t, graph);
  }
  return 0;
}