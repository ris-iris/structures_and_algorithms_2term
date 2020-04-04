#include <iostream>
#include <vector>

enum color { WHITE, GRAY, BLACK };

void dfs(int v,
         std::vector<std::vector<int> > &graph,
         std::vector<color> &color,
         std::vector<int> &sorted,
         bool &hasCycle) {
  color[v] = GRAY;
  for (int i = 0; i < graph[v].size(); ++i) {
    int to = graph[v][i];
    if (color[to] == WHITE) {
      dfs(to, graph, color, sorted, hasCycle);
    } else if (color[to] == GRAY) {
      hasCycle = true;
    }
  }
  color[v] = BLACK;
  sorted.push_back(v);
}

void setHasCycle(std::vector<std::vector<int> > &graph,
                 std::vector<color> &color,
                 std::vector<int> &sorted,
                 bool &hasCycle) {
  for (int i = 0; i < color.size(); ++i) {
    if (hasCycle)
      break;
    if (color[i] == 0)
      dfs(i, graph, color, sorted, hasCycle);
  }
}

void getGraph(std::vector<std::vector<int> > &graph, int edgeAm, std::istream &is) {
  int a, b;
  for (int i = 0; i < edgeAm; ++i) {
    is >> a >> b;
    graph[a].push_back(b);
  }
}

int main() {
  int n, m, a, b;
  std::cin >> n >> m;
  std::vector<std::vector<int> > graph(n);
  getGraph(graph, m, std::cin);

  std::vector<color> color(n, WHITE);
  std::vector<int> sorted;

  bool hasCycle = false;
  setHasCycle(graph, color, sorted, hasCycle);

  if (hasCycle) {
    std::cout << "NO";
  } else {
    std::cout << "YES" << std::endl;
    for (int i = n - 1; i > -1; --i) {
      std::cout << sorted[i] << " ";
    }
  }

  return 0;
}
