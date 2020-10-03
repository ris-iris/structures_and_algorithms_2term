#include <iostream>
#include <vector>

class SegmentTree {
  std::vector<std::pair<int, int> > tree;
 public:
  SegmentTree(int n) {
    tree.assign(n * 4, std::make_pair(0, 0));
  }

  void build(std::vector<int> &a, int l, int r, int v) {
    if (l == r) {
      tree[v].first = a[l];
    } else {
      build(a, l, (l + r) / 2, v * 2 + 1);
      build(a, (l + r) / 2 + 1, r, v * 2 + 2);
      tree[v] = std::max(tree[v * 2 + 1], tree[v * 2 + 2]);
    }
  }

  void update(int v, int bordLeft, int bordRight, int l, int r, int val) {
    if (l > r) {
      return;
    }
    if (bordLeft == l && bordRight == r) {
      tree[v].second += val;
    } else {
      int mid = (bordLeft + bordRight) / 2;
      update(v * 2 + 1, bordLeft, mid, l, std::min(mid, r), val);
      update(v * 2 + 2, mid + 1, bordRight, std::max(mid + 1, l), r, val);
      tree[v].first =
          std::max(tree[v * 2 + 1].first + tree[v * 2 + 1].second, tree[v * 2 + 2].first + tree[v * 2 + 2].second);
    }
  }

  int get(int v, int bordLeft, int bordRight, int l, int r) {
    if (l > r) {
      return -1;
    }
    if (bordLeft == l && bordRight == r) {
      return tree[v].first + tree[v].second;
    }
    int mid = (bordLeft + bordRight) / 2;
    return std::max(get(v * 2 + 1, bordLeft, mid, l, std::min(mid, r)),
                    get(v * 2 + 2, mid + 1, bordRight, std::max(mid + 1, l), r)) + tree[v].second;
  }
};

int main() {
  int n;
  std::cin >> n;
  --n;
  std::vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
  }
  int capacity, m, l, r, add;
  std::cin >> capacity >> m;
  SegmentTree segment_tree(n);
  segment_tree.build(a, 0, n - 1, 0);
  for (int i = 0; i < m; ++i) {
    std::cin >> l >> r >> add;
    if (segment_tree.get(0, 0, n - 1, l, r - 1) + add <= capacity) {
      segment_tree.update(0, 0, n - 1, l, r - 1, add);
    } else {
      std::cout << i << " ";
    }
  }
  return 0;
}