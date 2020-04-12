#include <iostream>
#include <vector>

void dfs(int v, std::vector<bool> &visited, std::vector<int> &outTime, std::vector<std::vector<int> > &graph) {
  visited[v] = true;
  for (int i = 0; i < graph[v].size(); ++i) {
    int to = graph[v][i];
    if (!visited[to]) {
      dfs(to, visited, outTime, graph);
    }
  }
  outTime.push_back(v);
}

void dfs2(int v, int col, std::vector<bool> &visited, std::vector<int> &color, std::vector<std::vector<int> > &graph) {
  visited[v] = true;
  color[v] = col;
  for (int i = 0; i < graph[v].size(); ++i) {
    int to = graph[v][i];
    if (!visited[to]) {
      dfs2(to, col, visited, color, graph);
    }
  }
}

void getGraphs(std::vector<std::vector<int> > &graph1,
               std::vector<std::vector<int> > &graph2,
               int edgeAm,
               std::istream &is) {
  int a, b;
  for (int i = 0; i < edgeAm; ++i) {
    is >> a >> b;
    --a;
    --b;
    graph1[a].push_back(b);
    graph2[b].push_back(a);
  }
}

int getAns(int numOfComp, std::vector<bool> &row, std::vector<bool> &col) {
  if (numOfComp == 1)
    return 0;
  int ansRow = numOfComp;
  int ansCol = numOfComp;

  for (int i = 0; i < numOfComp; ++i) {
    if (row[i])
      --ansRow;
    if (col[i])
      --ansCol;
  }
  return std::max(ansCol, ansRow);
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<std::vector<int> > graph(n);
  std::vector<std::vector<int> > invGraph(n);
  std::vector<bool> visited(n, false);
  std::vector<int> outTime;
  getGraphs(graph, invGraph, m, std::cin);

  for (int i = 0; i < n; ++i) {
    if (!visited[i]) {
      dfs(i, visited, outTime, invGraph);
    }
  }

  std::vector<int> color(n);
  int numOfComp = 0;
  for (int i = 0; i < n; ++i) {
    visited[i] = false;
  }
  for (int i = n - 1; i > -1; --i) {
    if (!visited[outTime[i]]) {
      dfs2(outTime[i], numOfComp, visited, color, graph);
      ++numOfComp;
    }
  }

  std::vector<bool> row(numOfComp, false);
  std::vector<bool> col(numOfComp, false);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < graph[i].size(); ++j) {
      int to = graph[i][j];
      if (color[i] != color[to]) {
        row[color[i]] = true;
        col[color[to]] = true;
      }
    }
  }

  std::cout << getAns(numOfComp, row, col);
  return 0;
}
