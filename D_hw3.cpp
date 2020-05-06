#include <iostream>
#include <vector>
#include <queue>

//Строим граф, где вершины - пстые клетки, а ребра соединяют соседние по стороне
int getEdges(int n, int m, std::vector<std::vector<int> > &edges) {
  std::vector<std::vector<int> > bridge(n);
  char ch;
  int vNum = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      std::cin>> ch;
      if(ch == '.')
        bridge[i].push_back(-1);
      else{
        bridge[i].push_back(vNum);
        ++vNum;
      }
    }
  }
  edges.resize(vNum);
  for (int i = 1; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if(bridge[i][j] > -1){
        if (bridge[i - 1][j] > -1){
          edges[bridge[i][j]].push_back(bridge[i - 1][j]);
          edges[bridge[i - 1][j]].push_back(bridge[i][j]);
        }
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 1; j < m; ++j) {
      if(bridge[i][j] > -1){
        if (bridge[i][j - 1] > -1){
          edges[bridge[i][j]].push_back(bridge[i][j - 1]);
          edges[bridge[i][j - 1]].push_back(bridge[i][j]);
        }
      }
    }
  }
  return vNum;
}

bool dfs(int v, std::vector<std::vector<int> >& graph, std::vector<int>& match, std::vector<bool>& visited){
  if(visited[v])
    return false;
  visited[v] = true;
  for (int i = 0; i < graph[v].size(); ++i) {
    int to = graph[v][i];
    if(match[to] == -1 || dfs(match[to], graph, match, visited)){
      match[to] = v;
      return true;
    }
  }
  return false;
}

int main() {
  int n, m, a, b;
  std::cin >> n >> m >> a >> b;
  std::vector<std::vector<int> > graph;
  int size = getEdges(n, m, graph);
  std::vector<int> match(size, -1);
  std::vector<bool> visited(size);
  for (int i = 0; i < size; ++i) {
    visited.assign(size,false);
    dfs(i, graph, match, visited);
  }
  int countB = 0;
  for (int i = 0; i < size; ++i) {
    if(match[i] == -1) ++countB;
  }
  if(a >= 2*b) countB = size;
  std::cout << countB * b + (size - countB) * a / 2;
  return 0;
}