#include <iostream>
#include <vector>
#include <queue>

#define INF 1000000000
void getEdges(int n, std::vector<std::vector<int> >& graph){
  char c;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> c;
      graph[i].push_back(c == '1'? 1: 0);
    }
  }
}

void minCut(int n, std::vector<std::vector<int> > &graph, std::vector<int> &cut){
  std::vector<bool> exists(n), in_a(n);
  std::vector<std::vector<int> > vert_compress(n);
  std::vector<int> weight(n);
  for (int i = 0; i < n; ++i) {
    vert_compress[i].push_back(i);
  }
  exists.assign(n, true);
  int max_v, best_cost = INF;
  for (int i = 0; i < n - 1; ++i) {
    in_a.assign(n, false);
    weight.assign(n, 0);
    int prev;
    for (int j = 0; j < n - i; ++j) {
      max_v = -1;
      for (int kI = 0; kI < n; ++kI) {
        if(exists[kI] && !in_a[kI] && (max_v == -1 || weight[kI] > weight[max_v]))
          max_v = kI;
      }
      if(j == n - i - 1){
        if(weight[max_v] < best_cost){
          best_cost = weight[max_v];
          cut = vert_compress[max_v];
        }
        vert_compress[prev].insert(vert_compress[prev].end(), vert_compress[max_v].begin(), vert_compress[max_v].end());
        for (int kI = 0; kI <n; ++kI) {
          graph[kI][prev] += graph[max_v][kI];
          graph[prev][kI] = graph[kI][prev];
        }
        exists[max_v] = false;
      } else {
        in_a[max_v] = true;
        for (int kI = 0; kI < n; ++kI) {
          weight[kI] += graph[max_v][kI];
        }
        prev = max_v;
      }
    }
  }
}

void getAns(int n, std::vector<int> & cut){
  std::vector<bool> in_a(n);
  in_a.assign(n, true);
  for(int i = 0; i < cut.size(); ++i){
    in_a[cut[i]] = false;
    std::cout << cut[i] + 1 << " ";
  }
  std::cout << std::endl;
  for (int i = 0; i < n; ++i) {
    if(in_a[i])
      std::cout << i + 1 << " ";
  }
}

int main() {
  int n;
  std::cin >> n;
  std::vector<std::vector<int> > graph(n);
  std::vector<int> cut;
  getEdges(n, graph);
  minCut(n, graph, cut);
  getAns(n, cut);
  return 0;
}
