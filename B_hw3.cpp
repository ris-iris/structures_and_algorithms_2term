#include <iostream>
#include <vector>
#include <queue>

class Edge {
 public:
  int start, finish, w;
  Edge(int a, int b, int c) {
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
  void make_set(int v) {
    par[v] = v;
    r[v] = 0;
  }

  Dsu(int n) {
    par.reserve(n);
    r.reserve(n);
    for (int i = 0; i < n; ++i) {
      make_set(i);
    }
  }

  int find_set(int v) {
    if (v == par[v])
      return v;
    par[v] = find_set(par[v]);
    return par[v];
  }

  void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
      if (r[a] < r[b])
        std::swap(a, b);
      par[b] = a;
      if (r[a] == r[b]) {
        ++r[a];
      }
    }
  }
};

void getEdges(int m, std::priority_queue<Edge, std::vector<Edge>, Comp> &edges) {
  int a, b, w;
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b >> w;
    --a;
    --b;
    edges.push(Edge(a, b, w));
    edges.push(Edge(b, a, w));
  }
}

long long mstWeight(int n, std::priority_queue<Edge, std::vector<Edge>, Comp> &edges) {
  Dsu trees(n);
  long long ans = 0;
  while (!edges.empty()){
    Edge temp = edges.top();
    edges.pop();
    if(trees.find_set(temp.finish) != trees.find_set(temp.start)){
      ans += temp.w;
      trees.union_sets(trees.find_set(temp.finish), trees.find_set(temp.start));
    }
  }
  return ans;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::priority_queue<Edge, std::vector<Edge>, Comp> edges;
  getEdges(m, edges);
  std::cout << mstWeight(n, edges);
  return 0;
}