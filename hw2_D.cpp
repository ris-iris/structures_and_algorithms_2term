#include <iostream>
#include <vector>
#define BASE 32

int getMatrix(std::vector<std::vector<uint32_t> > &matrix) {
  int s = matrix.size() / BASE;
  uint32_t put;
  char c;
  for (int i = 0; i < matrix.size(); ++i) {
    for (int j = 0; j < s; ++j) {
      put = 0;
      for (int k = 0; k < BASE; ++k) {
        std::cin >> c;
        c == '1' ? put = put * 2 + 1 : put *= 2;
      }
      matrix[i].push_back(put);
    }
    put = 0;
    for (int k = 0; k < matrix.size() % BASE; ++k) {
      std::cin >> c;
      c == '1' ? put = put * 2 + 1 : put *= 2;
    }
    matrix[i].push_back(put);
  }
  return s;
}

bool bit(uint32_t mask, int pos) {
  bool ans = 0 < (mask & (1u << pos));
  return ans;
}

void transClosure(std::vector<std::vector<uint32_t> > &matrix) {
  for (int k = 0; k < matrix.size(); ++k) {
    for (int i = 0; i < matrix.size(); ++i) {
      int pos = BASE - 1 - k % BASE;
      if (k / BASE == matrix[i].size() - 1) pos = matrix.size() % BASE - 1 - k % BASE;
      if (bit(matrix[i][k / BASE], pos)) {
        for (int j = 0; j < matrix[i].size(); ++j) {
          matrix[i][j] = matrix[i][j] | matrix[k][j];
        }
      }
    }
  }
}

std::string byteToStr(uint32_t byte, int size) {
  std::string s = "";
  for (int pos = size - 1; pos > -1; --pos) {
    s += 0 < bit(byte, pos) ? '1' : '0';
  }
  return s;
}

void printRes(std::vector<std::vector<uint32_t> > &matrix, int n, int m) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      std::cout << byteToStr(matrix[i][j], BASE);
    }
    std::cout << byteToStr(matrix[i][m], n % BASE) << std::endl;
  }
}

int main() {
  int n;
  std::cin >> n;
  std::vector<std::vector<uint32_t> > matrix(n);
  int colNum = getMatrix(matrix);
  transClosure(matrix);
  printRes(matrix, n, colNum);
  return 0;
}