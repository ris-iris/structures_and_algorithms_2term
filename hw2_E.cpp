#include <iostream>
#include <vector>

#define INF -1

void getEdges(std::vector<std::vector<double> > &edges) {
  int a, b;
  double rab, cab, rba, cba;
  for (int i = 0; i < edges.size() / 2; ++i) {
    std::cin >> a >> b >> rab >> cab >> rba >> cba;
    --a;
    --b;
    edges[2 * i].push_back(a);
    edges[2 * i].push_back(b);
    edges[2 * i].push_back(rab);
    edges[2 * i].push_back(cab);
    edges[2 * i + 1].push_back(b);
    edges[2 * i + 1].push_back(a);
    edges[2 * i + 1].push_back(rba);
    edges[2 * i + 1].push_back(cba);
  }
}

bool getAns(std::vector<std::vector<double> > &edges, int n, int s, double v) {
  std::vector<double> d(n, INF);
  d[s] = v;
  bool wasRelaxed = false;
  for (int i = 0; i < n; ++i) {
    wasRelaxed = false;
    for (int j = 0; j < edges.size(); ++j) {
      if (d[edges[j][0]] > INF)
        if (d[edges[j][1]] < edges[j][2] * (d[edges[j][0]] - edges[j][3])) {
          d[edges[j][1]] = edges[j][2] * (d[edges[j][0]] - edges[j][3]);
          wasRelaxed = true;
        }
    }
  }
  return wasRelaxed;
}

int main() {
  int n, m, s;
  double v;
  std::cin >> n >> m >> s >> v;
  std::vector<std::vector<double> > edges(2 * m);
  getEdges(edges);
  std::cout << (getAns(edges, n, s - 1, v) ? "YES" : "NO");
  return 0;
}