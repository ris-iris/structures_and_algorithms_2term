#include <iostream>
#include <vector>


class LCA {
  std::vector<int64_t> timeIn;
  std::vector<int64_t> timeOut;
  std::vector<std::vector<int64_t> > ancestry;
  int64_t l = 0;
  int64_t time = 0;
 public:
  LCA(int64_t n){
    timeIn.assign(n, 0);
    timeOut.assign(n, 0);
    ancestry.resize(n);
    while ((1u << l) <= n){
      ++l;
    }
    for (int64_t i = 0; i < n; ++i) {
      ancestry[i].assign(l + 1, -1);
    }
  }

  void dfs(int64_t v, int64_t par, std::vector<std::vector<int64_t> >& graph){
    timeIn[v] = ++time;
    ancestry[v][0] = par;
    for (int64_t i = 1; i < l + 1; ++i) {
      ancestry[v][i] = ancestry[ancestry[v][i - 1]][i - 1];
    }
    for (int64_t i = 0; i < graph[v].size(); ++i) {
      int64_t to = graph[v][i];
      if(to != par){
        dfs(to, v, graph);
      }
    }
    timeOut[v] = ++time;
  }

  bool isAncestor(int64_t a, int64_t b){
    return timeIn[a] <= timeIn[b] && timeOut[a] >= timeOut[b];
  }

  int64_t get_LCA(std::pair<int64_t, int64_t> pair){
    int64_t a = pair.first;
    int64_t b = pair.second;
    if(isAncestor(a, b)){
      return a;
    }
    if(isAncestor(b, a)){
      return b;
    }
    for (int64_t i = l; i > -1; --i) {
      if(! isAncestor(ancestry[a][i], b)){
        a = ancestry[a][i];
      }
    }
    return ancestry[a][0];
  }
};

class Next{
 private:
  int64_t x, y, z, n;
 public:
  Next(int64_t _x, int64_t _y, int64_t _z, int64_t _n){
    x = _x;
    y = _y;
    z = _z;
    n = _n;
  }
  std::pair<int64_t, int64_t> next(int64_t& a, int64_t& b, int64_t v){
    std::pair<int64_t, int64_t> ans = std::make_pair((a + v) % n, b);
    a = (a * x + b * y + z) % n;
    b = (b * x + a * y + z) % n;
    return ans;
  }
};

int main(){
  int64_t n, m, par, a, b, x, y, z;
  std::cin >> n >> m;
  std::vector<std::vector<int64_t> > graph(n);
  for(int64_t i = 1; i < n; ++i){
    std::cin >> par;
    graph[par].push_back(i);
  }
  std::cin >> a >> b >> x >> y >> z;
  Next request(x, y, z, n);
  LCA lca(n);
  lca.dfs(0, 0, graph);
  int64_t v = 0;
  long long sum = 0;
  for (int64_t i = 0; i < m; ++i) {
    v = lca.get_LCA(request.next(a, b, v));
    sum += v;
  }
  std::cout << sum;
  return 0;
}