#include <iostream>
#include <vector>
#include <queue>

class Edge {
 public:
  int start, finish;
  long long w;
  Edge(int a, int b, long long c) {
    start = a;
    finish = b;
    w = c;
  }
};

class Comp {
 public:
  constexpr bool operator()(const Edge &lhs, const Edge &rhs) const
  {
    return lhs.w > rhs.w;
  }
};

class Dsu {
 private:
  std::vector<int> par;
  std::vector<int> r;
 public:
  void makeSet(int v) {
    par[v] = v;
    r[v] = 0;
  }

  Dsu(int n) {
    par.reserve(n);
    r.reserve(n);
    for (int i = 0; i < n; ++i) {
      makeSet(i);
    }
  }

  int findSet(int v) {
    if (v == par[v])
      return v;
    par[v] = findSet(par[v]);
    return par[v];
  }

  void unionSets(int a, int b) {
    a = findSet(a);
    b = findSet(b);
    if (a != b) {
      if (r[a] < r[b]) {
        std::swap(a, b);
      }
      par[b] = a;
      if (r[a] == r[b]) {
        ++r[a];
      }
    }
  }
};

void getEdges(int m, std::priority_queue<Edge, std::vector<Edge>, Comp> &edges, int indx, std::vector<long long> &costs) {
  int a, b;
  long long w;
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b >> w;
    --a;
    --b;
    edges.push(Edge(a, b, w));
  }
  for (int i = 0; i < costs.size(); ++i) {
    if (i != indx){
      edges.push(Edge(i, indx, costs[i] + costs[indx]));
    }
  }
}

long long mstWeight(int n, std::priority_queue<Edge, std::vector<Edge>, Comp> &edges) {
  Dsu trees(n);
  long long ans = 0;
  while (!edges.empty()){
    Edge temp = edges.top();
    edges.pop();
    if(trees.findSet(temp.finish) != trees.findSet(temp.start)){
      ans += temp.w;
      trees.unionSets(trees.findSet(temp.finish), trees.findSet(temp.start));
    }
  }
  return ans;
}

int main() {
  int n, m, indx = 0;
  std::cin >> n >> m;
  std::vector<long long> costs(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> costs[i];
    if (costs[i] < costs[indx]){
      indx = i;
    }
  }
  std::priority_queue<Edge, std::vector<Edge>, Comp> edges;
  getEdges(m, edges, indx, costs);
  std::cout << mstWeight(n, edges);
  return 0;
}