#include <cstdio>
#include <type_traits>
template<class T>
class List {
 public:
  typedef T value_type;
 private:
  class NodeBase {
   public:
    NodeBase *prev = nullptr;
    NodeBase *next = nullptr;

    NodeBase() = default;

    ~NodeBase() {
      /*if(prev != nullptr) {
        delete prev;
      }*/
      if (next != nullptr) {
        delete next;
      }
    }

    friend class base_iter;
  };

  class Node : public NodeBase {
   public:
    T val;
    Node(Node const &it) {
      NodeBase::prev = it.prev;
      NodeBase::next = it.next;
      val = it.val;
    }

    Node(T &t) {
      val = t;
    }
    Node(const T &t) {
      val = t;
    }
    Node(T&& t) : val(std::move(t)) {}

    Node &operator=(const Node &other) {
      NodeBase::prev = other.prev;
      NodeBase::next = other.next;
      val = other.val;
    }
  };

  template<bool Const = false>
  class base_iter : public std::iterator<std::bidirectional_iterator_tag, NodeBase> {
    template<bool WasConst> friend
    class base_iter;
   private:
    NodeBase *ptr = nullptr;
   public:
    template<bool WasConst>
    base_iter(base_iter<WasConst> it) {
      ptr = it.ptr;
    }

    base_iter(NodeBase *n) {
      ptr = n;
    }

    /*~base_iter(){
      if(ptr != nullptr){
        delete ptr;
      }
    }*/

    base_iter &operator=(base_iter<Const> const &other) {
      ptr = other.ptr;
      return *this;
    }

    template<bool _Const = Const>
    std::enable_if_t<!_Const, NodeBase *>
    node() {
      return ptr;
    }

    template<bool _Const = Const>
    std::enable_if_t<_Const, const NodeBase *>
    node() const {
      return const_cast<const NodeBase *>(ptr);
    }

    template<bool _Const = Const>
    std::enable_if_t<!_Const, T &>
    operator*() {
      return static_cast<Node*>(ptr)->val;
    }

    template<bool _Const = Const>
    std::enable_if_t<_Const, const T &>
    operator*() const {
      return const_cast<const T &>(static_cast<Node*>(ptr)->val);
    }

    base_iter<Const> &operator++() {
      ptr = ptr->next;
      return *this;
    }

    const base_iter<Const> operator++(int) {
      base_iter<Const> temp = *this;
      ++(*this);
      return temp;
    }

    base_iter<Const> &operator--() {
      ptr = ptr->prev;
      return *this;
    }

    const base_iter<Const> operator--(int) {
      base_iter<Const> temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(base_iter const &other) {
      return ptr == other.ptr;
    }
    bool operator!=(base_iter const &other) {
      return ptr != other.ptr;
    }
  };

  NodeBase *_first = nullptr;
  NodeBase *_last = nullptr;
  int sz = 0;
//_____________________________________________________________________________________________
 public:
  using iterator = base_iter<false>;
  using const_iterator = base_iter<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  List() {
    sz = 0;
    _first = new NodeBase;
    _last = new NodeBase;
    _first->next = _last;
    _last->prev = _first;
  }
  List(size_t count, const T &value = T()) {
    if (count > 0) {
      sz = 0;
      _first = new Node(value);
      _last = new Node(value);
      _first->next = _last;
      _last->prev = _first;
      for (size_t i = 0; i < count; ++i) {
        push_back(value);
      }
    }
  }
  List(List<T> &list) {
    _first = new NodeBase;
    _last = new NodeBase;
    sz = 0;
    _first->next = _last;
    _last->prev = _first;
    for (auto x = list.cbegin(); x != list.cend(); ++x) {
      push_back(*x);
    }
  }
  List(List<T> &&other) {
    if (other.empty()) {
      init();
    } else {
      init();
      sz = other.sz;
      _first->next = other._first->next;
      _last->prev = other._last->prev;

      _first->next->prev = _first;
      _last->prev->next = _last;
      other.init();
    }
  }
  ~List() {
    if (_first != nullptr) {
      delete _first;
    }
    /*
    if(_last != nullptr){
      delete _last;
    } */
  }
  List<T> &operator=(const List<T> &list) {
    if (this == &list) return *this;

    clear();

    for (auto &item : list) {
      emplace_back(item);
    }

    return *this;
  }

  List<T> &operator=(List<T> &&list) noexcept {
    if (list.empty()) {
      clear();
    } else {
      sz = list.sz;
      _first->next = list._first->next;
      _last->prev = list._last->prev;

      _first->next->prev = _first;
      _last->prev->next = _last;
      list.init();
    }

    return *this;
  }
  void init() {
    sz = 0;
    _first = new NodeBase;
    _last = new NodeBase;
    _first->next = _last;
    _last->prev = _first;
  }
  int size() {
    return sz;
  }
  T& front() {
    iterator it(_first);
    return (*(++it));
  }
  T& back() {
    iterator it(_last);
    return (*(--it));
  }
  void clear() {
    NodeBase *temp = _first->next;
    NodeBase *next = temp->next;
    for (int i = 0; i < sz; ++i) {
      temp->prev = nullptr;
      temp->next = nullptr;
      delete temp;
      temp = next;
      next = temp->next;
    }
    _first->next = _last;
    _last->prev = _first;
    sz = 0;
  }
  bool empty() {
    return sz == 0;
  }
  iterator begin() {
    iterator it(_first);
    ++it;
    return it;
  }
  const_iterator begin() const {
    const_iterator it(_first);
    ++it;
    return it;
  }
  iterator end() {
    return iterator(_last);
  }
  const_iterator end() const {
    return const_iterator(_last);
  }

  const_iterator cbegin() const {
    const_iterator it(_first);
    ++it;
    return it;
  }
  const_iterator cend() const {
    return const_iterator(_last);
  }

  iterator insert(const_iterator it, const T &value) {
    ++sz;
    NodeBase *temp = new Node(value);
    iterator prev = it;
    --prev;
    iterator next = it;
    (prev.node())->next = temp;
    temp->prev = (prev.node());
    temp->next = (next.node());
    (next.node())->prev = temp;
    return iterator(temp);
  }
  iterator insert(const_iterator it, T &&value) {
    ++sz;
    auto temp = new Node(std::move(value));
    iterator prev = it;
    --prev;
    iterator next = it;
    (prev.node())->next = temp;
    temp->prev = (prev.node());
    temp->next = (next.node());
    (next.node())->prev = temp;
    return iterator((next.node())->prev);
  }

  template<typename InputIter, typename>
  iterator insert(List::const_iterator position, InputIter first, InputIter last) {
    for (; first != last; ++first, ++position) {
      insert(position, std::move(*first));
    }
  }

  void erase(const_iterator it) {
    --sz;
    iterator prev = it;
    iterator next = it;
    --prev;
    ++next;
    (prev.node())->next->prev = nullptr;
    (prev.node())->next->next = nullptr;
    (prev.node())->next = (next.node());
    (next.node())->prev = (prev.node());
    delete it.node();
  }
  void erase(const_iterator it1, const_iterator it2) {
    iterator temp = it1;
    iterator next = temp;
    ++next;
    while (temp != it2) {
      erase(temp);
      temp = next;
      ++next;
    }
  }
  void push_back(T &&value) {
    iterator it(_last);
    insert(it, std::move(value));
  }
  void push_back(const T &value) {
    iterator it(_last);
    insert(it, value);
  }
  void pop_back() {
    if (!empty()) {
      iterator it(_last);
      --it;
      erase(it);
    }
  }
  void push_front(T &&value) {
    iterator it(_first);
    ++it;
    insert(it, std::move(value));
  }
  void push_front(const T &value) {
    iterator it(_first);
    ++it;
    insert(it, value);
  }
  void pop_front() {
    if (!empty()) {
      iterator it(_first);
      ++it;
      erase(it);
    }
  }

  template<class... Args>
  void emplace(const_iterator it, Args... args) {
    insert(it, T(std::move(args)...));
  }
  template<class... Args>
  void emplace_front(Args... args) {
    push_front(T(std::move(args)...));
  }
  template<class... Args>
  void emplace_back(Args... args) {
    push_back(T(std::move(args)...));
  }

  void reverse() {
    iterator temp = iterator(_first);
    ++temp;
    iterator help = temp;
    _first->prev = (temp.node());
    _first->next = (nullptr);
    NodeBase *tempNext = (temp.node())->next;
    for (int i = 0; i < sz; ++i) {
      temp = help;
      tempNext = (temp.node())->next;
      ++help;
      (temp.node())->next = (temp.node())->prev;
      (temp.node())->prev = (tempNext);
    }
    _last->next = (temp.node());
    _last->prev = (nullptr);
    tempNext = _last;
    _last = _first;
    _first = tempNext;
  }

  void unique() {
    iterator temp = iterator(_first);
    ++temp;
    iterator lastButOne = iterator(_last);
    --lastButOne;
    while (temp != lastButOne && temp != iterator(_last)) {
      while (*temp == static_cast<Node*>((temp.node())->next)->val) {
        erase(const_iterator((temp.node())->next));
      }
      ++temp;
    }
  }
};