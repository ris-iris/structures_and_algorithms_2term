#include <iostream>
#include <vector>

#include <iostream>
#include <vector>

#define INF 1000000000

class SegmentTree {
  std::vector<std::pair<int, int> > tree;
  void push(int v){
    if(tree[v].second != -1){
      tree[v * 2 + 1].second = tree[v].second;
      tree[v * 2 + 2].second = tree[v].second;
      tree[v * 2 + 1].first = tree[v].second;
      tree[v * 2 + 2].first = tree[v].second;
      tree[v].second = -1;
    }
  }
 public:
  SegmentTree(int n) {
    tree.assign(n * 4, std::make_pair(0, -1));
  }

  void build(std::vector<int> &a, int l, int r, int v) {
    if (l == r) {
      tree[v].first = a[l];
    } else {
      build(a, l, (l + r) / 2, v * 2 + 1);
      build(a, (l + r) / 2 + 1, r, v * 2 + 2);
      tree[v].first = std::min(tree[v * 2 + 1].first, tree[v * 2 + 2].first);
    }
  }

  void update(int v, int bordLeft, int bordRight, int l, int r, int val) {
    if (l > r) {
      return;
    }
    if (bordLeft == l && bordRight == r) {
      tree[v].second = val;
      tree[v].first = val;
      if(bordLeft == bordRight){
        tree[v].second = -1;
      }
    } else {
      push(v);
      int mid = (bordLeft + bordRight) / 2;
      update(v * 2 + 1, bordLeft, mid, l, std::min(mid, r), val);
      update(v * 2 + 2, mid + 1, bordRight, std::max(mid + 1, l), r, val);
      tree[v].first =
          std::min(tree[v * 2 + 1].first, tree[v * 2 + 2].first);
    }
  }

  int get(int v, int bordLeft, int bordRight, int l, int r) {
    if (l > r) {
      return INF;
    }
    if (bordLeft == l && bordRight == r) {
      return tree[v].first;
    }
    push(v);
    int mid = (bordLeft + bordRight) / 2;
    return std::min(get(v * 2 + 1, bordLeft, mid, l, std::min(mid, r)),
                    get(v * 2 + 2, mid + 1, bordRight, std::max(mid + 1, l), r));
  }
};


int main() {
  int n, r, g, b;
  std::cin >> n;
  std::vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> r >> g >> b;
    a[i] = r + g + b;
  }
  SegmentTree st(n);
  st.build(a, 0, n - 1, 0);
  int k, c, d, e, f;
  std::cin >> k;
  for (int i = 0; i < k; ++i) {
    std::cin >> c >> d >> r >> g >> b >> e >> f;
    st.update(0, 0, n - 1, c, d, r + g + b);
    std::cout << st.get(0, 0, n - 1, e, f) << " ";
  }
  return 0;
}