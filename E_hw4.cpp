#include <iostream>

class ImTreap{
 private:
  struct Node{
    Node(const std::string& val){
      value = val;
      left = nullptr;
      right = nullptr;
      sz = 1;
      priority = rand();
    }

    void update_sz(){
      sz = 1;
      if(left != nullptr){
        sz += left->sz;
      }
      if(right != nullptr){
        sz += right->sz;
      }
    }

    int sz;
    int priority;
    std::string value;

    Node* left;
    Node* right;
  };

  static int get_sz(Node* n){
    if(n == nullptr){
      return 0;
    }
    return n->sz;
  }

  std::pair<Node*, Node*> split(Node* n, size_t pos){
    if(n == nullptr){
      return std::make_pair(nullptr, nullptr);
    }
    if(get_sz(n->left) >= pos){
      std::pair<Node*, Node*> left_part = split(n->left, pos);
      n->left = left_part.second;
      n->update_sz();
      return std::make_pair(left_part.first, n);
    } else {
      std::pair<Node*, Node*> right_part = split(n->right, pos - 1 - get_sz(n->left));
      n->right = right_part.first;
      n->update_sz();
      return std::make_pair(n, right_part.second);
    }
  }

  Node* merge(Node* n1, Node* n2){
    if (n2 == nullptr)
      return n1;
    if (n1 == nullptr)
      return n2;
    if (n1->priority > n2->priority) {
      n1->right = merge(n1->right, n2);
      n1->update_sz();
      return n1;
    }
    else {
      n2->left = merge(n1, n2->left);
      n2->update_sz();
      return n2;
    }
  }

  void clear(Node* n){
    if(n != nullptr){
      clear(n->left);
      clear(n->right);
      delete n;
    }
  }

  Node* root;

 public:
  ImTreap(){
    root = nullptr;
  }
  ~ImTreap(){
    clear(root);
    root = nullptr;
  }

  void insertAt(size_t pos, const std::string& val){
    Node* temp = new Node(val);
    std::pair<Node*, Node*> help = split(root, pos);
    Node* right = merge(temp, help.second);
    root = merge(help.first, right);
  }

  void deleteAt(size_t pos){
    std::pair<Node*, Node*> help1 = split(root, pos);
    std::pair<Node*, Node*> help2 = split(help1.second, 1);
    delete help2.first;
    root = merge(help1.first, help2.second);
  }

  std::string getAt(size_t pos) const{
    Node* temp = root;
    int prev = get_sz(temp->left);
    while (prev != pos){
      if (pos < prev){
        temp = temp->left;
      } else {
        pos = pos - 1 - prev;
        temp = temp->right;
      }
      prev = get_sz(temp->left);
    }
    return temp->value;
  }
};

int main(){
  int k, pos, pos2;
  char command;
  std::string str;
  ImTreap treap;
  std::cin >> k;
  for (int i = 0; i < k; ++i) {
    std::cin >> command >> pos;
    switch (command) {
      case '+':
        std::cin >> str;
        treap.insertAt(pos, str);
        break;
      case '-':
        std::cin >> pos2;
        for (int i = pos; i <= pos2; ++i) {
          treap.deleteAt(pos);
        }
        break;
      case '?':
        std::cout << treap.getAt(pos) << std::endl;
        break;
    }
  }
  return 0;
}