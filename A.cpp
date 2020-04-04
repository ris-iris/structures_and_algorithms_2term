#include <iostream>
#include <vector>
#include <queue>

void bfs(int v, std::vector<int> &timeFromV, std::vector<std::vector<int> > graph) {
  std::queue<int> q;
  int temp;
  int to;
  q.push(v);
  timeFromV[v] = 0;
  while (!q.empty()) {
    temp = q.front();
    q.pop();
    for (int i = 0; i < graph[temp].size(); ++i) {
      to = graph[temp][i];
      if (timeFromV[to] == -1) {
        timeFromV[to] = timeFromV[temp] + 1;
        q.push(to);
      }
    }
  }
}

int getMinLength(std::vector<int> &t1, std::vector<int> &t2, std::vector<int> &t3) {
  int lenght = 1000000;

  for (int i = 0; i < t1.size(); ++i) {
    lenght = std::min(t1[i] + t2[i] + t3[i], lenght);
  }

  return lenght;
}

void getGraph(std::vector<std::vector<int> > &graph, int edgeAm, std::istream &is) {
  int a, b;
  for (int i = 0; i < edgeAm; ++i) {
    is >> a >> b;
    a--;
    b--;
    graph[a].push_back(b);
    graph[b].push_back(a);
  }
}

int main() {
  int n, m, leon, matilda, moloko;

  std::cin >> n >> m >> leon >> matilda >> moloko;
  leon--;
  matilda--;
  moloko--;
  std::vector<std::vector<int> > graph(n);
  std::vector<int> timeFromLeon(n);
  std::vector<int> timeFromMatilda(n);
  std::vector<int> timeFromMilk(n);

  for (int i = 0; i < n; ++i) {
    timeFromLeon[i] = -1;
    timeFromMatilda[i] = -1;
    timeFromMilk[i] = -1;
  }

  getGraph(graph, m, std::cin);

  bfs(leon, timeFromLeon, graph);
  bfs(matilda, timeFromMatilda, graph);
  bfs(moloko, timeFromMilk, graph);

  std::cout << getMinLength(timeFromLeon, timeFromMatilda, timeFromMilk);
  return 0;
}
