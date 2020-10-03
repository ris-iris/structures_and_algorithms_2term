#include <iostream>
#include <vector>
#include <cmath>

#define elem std::pair<int, int>

std::pair<elem, elem > getSecond(std::pair<elem, elem > &p1, std::pair<elem, elem > &p2) {
  if(p2.first.first >= p1.second.first) return p1;
  if(p1.first.first >= p2.second.first) return p2;
  if(p1.first.first > p2.first.first) return std::make_pair(p2.first, p1.first);
  if(p2.first.first > p1.first.first) return std::make_pair(p1.first, p2.first);
  if(p1.first == p2.first) return std::make_pair(p1.first, std::min(p1.second, p2.second));
  if(p1.first.first == p2.first.first) return std::make_pair(std::min(p1.first, p2.first), std::max(p1.first, p2.first));
}

void makeSparseTable(int n,
                     std::vector<elem > &a,
                     std::vector<std::vector<std::pair<elem, elem > > > &sTable) {
  std::vector<std::pair<elem, elem > > helpVec;
  for (int i = 0; i < n - 1; ++i) {
    helpVec.push_back(std::make_pair(std::min(a[i], a[i + 1]), std::max(a[i], a[i + 1])));
  }
  sTable.push_back(helpVec);
  int pow = 2;
  for (int i = 1; pow < n; ++i) {
    helpVec.assign(0, std::make_pair(std::make_pair(0, 0), std::make_pair(0, 0)));
    for (int k = 0; k < n - pow * 2 + 1; ++k) {
      std::pair<elem, elem > temp_pair1 = sTable[i - 1][k];
      std::pair<elem, elem > temp_pair2 = sTable[i - 1][k + pow];
      helpVec.push_back(getSecond(temp_pair1, temp_pair2));
    }
    pow *= 2;
    sTable.push_back(helpVec);
  }
}

int floor(int length){
  if(length == 1){
    return 0;
  } else {
    return floor(length/2) + 1;
  }
}

int getStat(int l, int r, std::vector<std::vector<std::pair<elem, elem > > > &sTable){
  int k = floor(r - l + 1);
  return getSecond(sTable[k - 1][l], sTable[k - 1][r + 1 - std::pow(2, k)]).second.first;
}

int main() {
  int n, m, x, y;
  std::cin >> n >> m;
  std::vector<elem > a;
  std::vector<std::vector<std::pair<elem, elem > > > sTable;
  for (int i = 0; i < n; ++i) {
    std::cin >> x;
    a.push_back(std::make_pair(x, i));
  }
  makeSparseTable(n, a, sTable);

  for (int i = 0; i < m; ++i) {
    std::cin >> x >> y;
    --x, --y;
    std:: cout << getStat(x, y, sTable) << std::endl;
  }
  return 0;
}
