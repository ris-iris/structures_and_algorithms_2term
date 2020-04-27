#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <stdio.h>
#define myPI 3.14159265
#define EPS 10e-3

bool eq(double a, double b) {
  return std::abs(a - b) < EPS;
}

struct Point {
 public:
  double x;
  double y;
  ~Point() = default;
  Point(double _x, double _y) {
    x = _x;
    y = _y;
  }
  Point(Point const &a) {
    x = a.x;
    y = a.y;
  }
};
bool operator==(const Point &a, const Point &b) {
  return (a.x == b.x) && (a.y == b.y);
}
bool operator!=(const Point &a, const Point &b) {
  return !(a == b);
}

struct Vector {
 public:
  double x, y;
  ~Vector() = default;
  Vector(const Point &a, const Point &b) {
    x = b.x - a.x;
    y = b.y - a.y;
  }
  Vector() {
    x = 0;
    y = 0;
  }
  Vector(double a, double b) {
    x = a;
    y = b;
  }
  Vector(Vector const &a) {
    x = a.x;
    y = a.y;
  }
  Vector &operator+=(const Vector &a) {
    x += a.x;
    y += a.y;
    return *this;
  }
  Vector &operator-=(const Vector &a) {
    x -= a.x;
    y -= a.y;
    return *this;
  }
  Vector &operator*=(const double &k) {
    x *= k;
    y *= k;
    return *this;
  }
  double dot(const Vector &b) const {
    return x * b.x + y * b.y;
  }
  double abs() const {
    return sqrt(x * x + y * y);
  }
  void rot(double angle) {
    double _x = x * cos(angle) - y * sin(angle);
    double _y = x * sin(angle) + y * cos(angle);
    x = _x;
    y = _y;
  }
  Vector pr(Vector &b) {
    Vector v = b;
    v *= 1 / b.abs();
    double len = (x * b.x + y * b.y) / b.abs();
    v *= len;
    return v;
  }
};
bool operator==(const Vector &a, const Vector &b) {
  return a.x == b.x && a.y == b.y;
}
bool isCollinear(const Vector &a, const Vector &b) {
  return eq(a.x / b.x, a.y / b.y);
}
double operator*(const Vector &a, const Vector &b) {
  return a.x * b.y - a.y * b.x;
}
Vector operator+(const Vector &a, const Vector &b) {
  Vector res = a;
  res += b;
  return res;
}
Vector operator-(const Vector &a, const Vector &b) {
  Vector res = a;
  res -= b;
  return res;
}
Vector operator*(const Vector &a, const double &k) {
  Vector res = a;
  res *= k;
  return res;
}

class Line {
 public:
  Vector r;
  Point a = Point(0, 0);
  Line(const Point &p, const Point &q) {
    a = p;
    r = Vector(p, q);
  }
  Line(const Point &p, const Vector &q) {
    a = p;
    r = q;
  }
  Line(double k, double b) {
    a = Point(0, b);
    r = Vector(1, k);
  }
  Line(const Point &p, double k) {
    a = p;
    r = Vector(1, k);
  }
};
bool operator==(const Line &l1, const Line &l2) {
  return (isCollinear(l1.r, l2.r) && isCollinear(Vector(l1.a, l2.a), l1.r));
}
bool operator!=(const Line &l1, const Line &l2) {
  return !(l1 == l2);
}
Vector ref(Vector &v, Line &axis) {
  Vector ans = v.pr(axis.r);
  ans += ans - v;
  return ans;
}

class Shape {
 public:
  virtual double area() = 0;
  virtual double perimeter() = 0;
  virtual bool operator==(const Shape &another) = 0;
  virtual bool isCongruentTo(const Shape &another) = 0;
  virtual bool isSimilarTo(const Shape &another) = 0;
  virtual bool containsPoint(Point point) = 0;
  virtual void rotate(Point center, double angle) = 0;
  virtual void reflex(Point center) = 0;
  virtual void reflex(Line axis) = 0;
  virtual void scale(Point center, double coefficient) = 0;
  virtual ~Shape() = default;
};

class Polygon : public Shape {
 protected:
  std::vector<Vector> vec;
  Point zeroPoint = Point(0, 0);
  static bool getSign(Vector &v, Vector &w) {
    double sign = v * w;
    return sign > 0;
  }
  void normalize() {
    /*Point p0 = zeroPoint;
    Point minp = p0;
    unsigned int mini = 0;
    for (unsigned int i = 0; i < vec.size(); ++i) {
      if (p0.x < minp.x || (p0.x == minp.x && p0.y < minp.y)) {
        minp = p0;
        mini = i;
      }
      p0.x += vec[i].x;
      p0.y += vec[i].y;
    }
    zeroPoint = minp;
    std::vector<Vector> helpVec;
    unsigned int n = vec.size();
    if (minp.x + vec[mini].x > minp.x - vec[(n + mini - 1) % n].x
        || (minp.x + vec[mini].x == minp.x - vec[(n + mini - 1) % n].x
            && minp.y + vec[mini].y < minp.y - vec[(n + mini - 1) % n].y)) {
      for (unsigned int i = 0; i < mini; ++i) {
        helpVec.push_back(vec[i]);
      }
      for (unsigned int i = 0; i < n; ++i) {
        if (i < n - mini)
          vec[i] = vec[i + mini];
        else
          vec[i] = helpVec[i - n + mini];
      }
    } else {
      for (int i = static_cast<int>(mini) - 1; i > -1; --i) {
        helpVec.push_back(vec[i]);
      }
      for (int i = static_cast<int>(n) - 1; i > static_cast<int>(mini) - 1; --i) {
        helpVec.push_back(vec[i]);
      }
      for (unsigned int i = 0; i < n; ++i) {
        vec[i] = helpVec[i] * -1;
      }
    } */
  }
 public:
  ~Polygon() override = default;
  Polygon() {
    vec.reserve(0);
  }
  Polygon(std::vector<Point> &points) {
    zeroPoint = points[0];
    for (unsigned int i = 0; i < points.size() - 1; ++i) {
      vec.push_back(Vector(points[i], points[i + 1]));
    }
    vec.push_back(Vector(points[points.size() - 1], points[0]));
    normalize();
  }
  template<class... T>
  Polygon(T... t) {
    std::vector<Point> p = {t...};
    zeroPoint = p[0];
    for (unsigned int i = 0; i < p.size() - 1; ++i) {
      if (i > 0 && Vector(p[i], p[i + 1]) * vec[i - 1] == 0) {
        vec[i - 1] += Vector(p[i], p[i + 1]);
      } else vec.push_back(Vector(p[i], p[i + 1]));
    }
    vec.push_back(Vector(p[p.size() - 1], p[0]));
    normalize();
  }
  int verticesCount() const {
    return vec.size();
  }
  std::vector<Point> getVertices() {
    std::vector<Point> ans;
    ans.push_back(zeroPoint);
    for (unsigned int i = 0; i < vec.size() - 1; ++i) {
      ans.push_back(Point(ans[i].x + vec[i].x, ans[i].y + vec[i].y));
    }
    return ans;
  }
  bool isConvex() {
    bool sign = getSign(vec[vec.size() - 1], vec[0]);
    bool sign1;
    for (unsigned int i = 0; i < vec.size() - 1; ++i) {
      sign1 = getSign(vec[i], vec[i + 1]);
      if (sign != sign1) return false;
      sign = sign1;
    }
    return true;
  }
  double area() override {
    double area = 0;
    Vector v = vec[0];
    for (unsigned int i = 1; i < vec.size(); ++i) {
      area += v * (v + vec[i]);
      v += vec[i];
    }
    return std::abs(area) / 2;
  }
  double perimeter() override {
    double ans = 0;
    for (unsigned int i = 0; i < vec.size(); ++i) {
      ans += vec[i].abs();
    }
    return ans;
  }
  bool isCongruentTo(const Shape &another) override {
    unsigned int n = vec.size();
    auto shape = dynamic_cast<Polygon const *>(&another);
    if (shape == nullptr || shape->verticesCount() != verticesCount()) return false;
    bool ans = false;
    for (unsigned int i = 0; i < vec.size(); ++i) {
      if (eq(vec[i].abs(), shape->vec[0].abs())) {
        ans = true;
        for (unsigned int j = 0; j < n; ++j) {
          if (!eq(vec[(i + j + 1) % n].abs(), shape->vec[(j + 1) % n].abs())
              || !eq(std::abs(vec[(i + j + 1) % n] * vec[(i + j) % n]),
                     std::abs(shape->vec[j] * shape->vec[(j + 1) % n]))) {
            ans = false;
            break;
          }
        }
        if (ans) return ans;
        ans = true;
        for (unsigned int j = 0; j < n; ++j) {
          if (!eq(vec[(n + i - j - 1) % n].abs(), shape->vec[(j + 1) % n].abs())
              || !eq(std::abs(vec[(n + i - j - 1) % n] * vec[(n + i - j) % n]),
                     std::abs(shape->vec[j] * shape->vec[(j + 1) % n]))) {
            ans = false;
            break;
          }
        }
      }
      if (ans) return ans;
    }
    return false;
  }
  bool operator==(const Shape &another) override {
    unsigned int n = vec.size();
    auto shape = dynamic_cast<Polygon const *>(&another);
    if (shape == nullptr || shape->verticesCount() != verticesCount()) return false;
    bool ans = false;
    for (unsigned int i = 0; i < vec.size(); ++i) {
      if (vec[i].abs() == shape->vec[0].abs()) {
        ans = true;
        for (unsigned int j = 0; j < n; ++j) {
          if (!eq(vec[(i + j + 1) % n].abs(), shape->vec[(j + 1) % n].abs())
              || !eq(abs(vec[(i + j + 1) % n] * vec[(i + j) % n]), abs(shape->vec[j] * shape->vec[(j + 1) % n]))) {
            ans = false;
            break;
          }
        }
        if (ans) return ans;
        ans = true;
        for (unsigned int j = 0; j < n; ++j) {
          if (!eq(vec[(n + i - j - 1) % n].abs(), shape->vec[(j + 1) % n].abs())
              || !eq(abs(vec[(n + i - j - 1) % n] * vec[(n + i - j) % n]),
                     abs(shape->vec[j] * shape->vec[(j + 1) % n]))) {
            ans = false;
            break;
          }
        }
      }
      if (ans) return ans;
    }
    return false;
  }
  bool operator!=(const Shape &another) {
    return !(*this == another);
  }

  bool isSimilarTo(const Shape &another) override {
    unsigned int n = vec.size();
    auto shape = dynamic_cast<Polygon const *>(&another);
    if (shape == nullptr || shape->verticesCount() != verticesCount()) return false;
    bool ans = false;
    double k;
    for (unsigned int i = 0; i < vec.size(); ++i) {
      ans = true;
      k = vec[i].abs() / shape->vec[0].abs();
      for (unsigned int j = 0; j < n; ++j) {
        if (!eq(vec[(i + j + 1) % n].abs(), k * shape->vec[(j + 1) % n].abs())
            || !eq(abs(vec[(i + j + 1) % n] * vec[(i + j) % n]),
                   k * k * abs(shape->vec[j] * shape->vec[(j + 1) % n]))) {
          ans = false;
          break;
        }
      }
      if (ans) return ans;
      ans = true;
      for (unsigned int j = 0; j < n; ++j) {
        if (!eq(vec[(n + i - j - 1) % n].abs(), k * shape->vec[(j + 1) % n].abs())
            || !eq(abs(vec[(n + i - j - 1) % n] * vec[(n + i - j) % n]),
                   k * k * abs(shape->vec[j] * shape->vec[(j + 1) % n]))) {
          ans = false;
          break;
        }
      }
      if (ans) return ans;
    }
    return false;
  }
  bool containsPoint(Point point) override {
    Vector v(point, zeroPoint);
    double newArea = 0;
    for (unsigned int i = 0; i < vec.size(); ++i) {
      newArea += std::abs(v * vec[i]);
      v += vec[i];
    }
    newArea /= 2;
    return newArea <= area();
  }
  void rotate(Point center, double angle) override {
    Vector v(center, zeroPoint);
    v.rot(angle * myPI / 180);
    zeroPoint = Point(center.x + v.x, center.y + v.y);
    for (unsigned int i = 0; i < vec.size(); ++i) {
      vec[i].rot(angle * myPI / 180);
    }
    normalize();
  }
  void reflex(Point center) override {
    Vector v(zeroPoint, center);
    v *= 2;
    zeroPoint = Point(zeroPoint.x + v.x, zeroPoint.y + v.y);
    for (unsigned int i = 0; i < vec.size(); ++i) {
      vec[i] *= -1;
    }
    normalize();
  }
  void reflex(Line axis) override {
    Vector v(axis.a, zeroPoint);
    v = ref(v, axis);
    zeroPoint = Point(axis.a.x + v.x, axis.a.y + v.y);
    for (unsigned int i = 0; i < vec.size(); ++i) {
      vec[i] = ref(vec[i], axis);
    }
    normalize();
  }
  void scale(Point center, double coefficient) override {
    Vector v(center, zeroPoint);
    Vector nev = v * coefficient;
    zeroPoint = Point(nev.x + center.x, nev.y + center.y);
    for (unsigned int i = 0; i < vec.size(); ++i) {
      vec[i] *= coefficient;
    }
    normalize();
  }
};

class Ellipse : public Shape {
 protected:
  double a, b, c;
  Point f1 = Point(0, 0), f2 = Point(0, 0);
 public:
  Ellipse(const Point &p1, const Point &p2, double r) {
    f1 = p1;
    f2 = p2;
    a = r / 2;
    c = Vector(f1, f2).abs() / 2;
    b = sqrt(a * a - c * c);
  }

  std::pair<Point, Point> focuses() {
    return std::make_pair(f1, f2);
  }

  Point center() {
    return Point(0.5 * f1.x + 0.5 * f2.x, 0.5 * f1.y + 0.5 * f2.y);
  }

  std::pair<Line, Line> directrices() {
    Vector r = Vector(f1, f2);
    r *= (0.5);
    r *= (a * a);
    double x = r.x;
    double y = r.y;
    Vector n = Vector(-y, x);
    Point ctr = center();
    Point p1 = Point(ctr.x + r.x, ctr.y + r.y);
    Point p2 = Point(ctr.x - r.x, ctr.y - r.y);
    return std::make_pair(Line(p1, n), Line(p2, n));
  }

  double eccentricity() {
    return c / a;
  }

  double area() override {
    double area = myPI * a * b;
    return area;
  }
  double perimeter() override {
    double ans = myPI * (3 * (a + b) - sqrt((3 * a + b) * (a + 3 * b)));
    return ans;
  }
  bool isCongruentTo(const Shape &another) override {
    auto shape = dynamic_cast<Ellipse const *>(&another);
    return !(shape == nullptr || a != shape->a || b != shape->b);
  }
  bool operator==(const Shape &another) override {
    auto shape = dynamic_cast<Ellipse const *>(&another);
    if (shape == nullptr) return false;
    return isCongruentTo(another) && shape->f1 == f1;
  }
  bool isSimilarTo(const Shape &another) override {
    auto shape = dynamic_cast<Ellipse const *>(&another);
    if (shape == nullptr) return false;
    return !(shape == nullptr || a / shape->a != b / shape->b);
  }
  bool containsPoint(Point point) override {
    double r;
    r = Vector(point, f1).abs() + Vector(point, f2).abs();
    return r <= 2 * a;
  }
  void rotate(Point center, double angle) override {
    Vector v1(center, f1);
    Vector v2(center, f2);
    v1.rot(angle);
    v2.rot(angle);
    f1.x = center.x + v1.x;
    f1.y = center.y + v1.y;
    f2.x = center.x + v2.x;
    f2.y = center.y + v2.y;
  }
  void reflex(Point center) override {
    Vector v1(center, f1);
    Vector v2(center, f2);
    v1 *= -1;
    v2 *= -1;
    f1.x = center.x + v1.x;
    f1.y = center.y + v1.y;
    f2.x = center.x + v2.x;
    f2.y = center.y + v2.y;
  }
  void reflex(Line axis) override {
    Point center = axis.a;
    Vector v1(center, f1);
    Vector v2(center, f2);
    Vector pr1 = v1.pr(axis.r);
    Vector pr2 = v2.pr(axis.r);
    v1 -= pr1;
    v2 -= pr2;
    f1.x = center.x - 2 * v1.x + pr1.x;
    f1.y = center.y - 2 * v1.y + pr1.y;
    f2.x = center.x - 2 * v2.x + pr2.x;
    f2.y = center.y - 2 * v2.y + pr2.y;
  }
  void scale(Point center, double coefficient) override {
    Vector v1(center, f1);
    Vector v2(center, f2);
    v1 *= coefficient;
    v2 *= coefficient;
    a *= coefficient;
    b *= coefficient;
    c *= coefficient;
    f1.x = center.x + v1.x;
    f1.y = center.y + v1.y;
    f2.x = center.x + v2.x;
    f2.y = center.y + v2.y;
  }
};
class Circle : public Ellipse {
 public:
  Circle(const Point &c, double r) : Ellipse(c, c, r * 2) {}
  double radius() {
    return a;
  }
};
class Rectangle : public Polygon {
 public:
  Rectangle(const Point &a, const Point &c, double coef) : Polygon() {
    if (coef < 1) coef = 1 / coef;
    double angle = atan(coef);
    zeroPoint = a;
    Vector v(a, c);
    v *= (1 / sqrt(1 + coef * coef));
    v.rot(angle);
    Point d(v.x + a.x, v.y + a.y);
    v = Vector(d, Point(0.5 * (a.x + c.x), 0.5 * (a.y + c.y)));
    Point b(0.5 * (a.x + c.x) + v.x, 0.5 * (a.y + c.y) + v.y);
    vec.push_back(Vector(a, b));
    vec.push_back(Vector(b, c));
    vec.push_back(Vector(c, d));
    vec.push_back(Vector(d, a));
    normalize();
  }
  ~Rectangle() override = default;
  Point center() {
    Vector v = vec[0] + vec[1];
    v *= 0.5;
    return Point(zeroPoint.x + v.x, zeroPoint.y + v.y);
  }
  std::pair<Line, Line> diagonals() {
    return std::make_pair(Line(zeroPoint, vec[0] + vec[1]), Line(center(), vec[1] + vec[2]));
  }
};
class Square : public Rectangle {
 public:
  Square(Point &a, Point &c) : Rectangle(a, c, 1) {}
  Circle circumscribedCircle() {
    return Circle(center(), Vector(center(), zeroPoint).abs());
  }
  Circle inscribedCircle() {
    return Circle(center(), vec[0].abs() / 2);
  }
};
class Triangle : public Polygon {
 public:
  Triangle(Point &a, Point &b, Point &c) : Polygon(a, b, c) {}
  Circle circumscribedCircle() {
    Point a = zeroPoint;
    Point b(a.x + vec[0].x, a.y + vec[0].y);
    Point c(b.x + vec[1].x, b.y + vec[1].y);
    Line l1(Point(0.5 * (a.x + b.x), 0.5 * (a.y + b.y)), Vector(-vec[0].y, vec[0].x));
    Line l2(Point(0.5 * (c.x + b.x), 0.5 * (c.y + b.y)), Vector(-vec[1].y, vec[1].x));
    double a1 = l1.r.y;
    double b1 = -l1.r.x;
    double c1 = -l1.a.x * l1.r.y + l1.a.y * l1.r.x;
    double a2 = l2.r.y;
    double b2 = -l2.r.x;
    double c2 = -l2.a.x * l2.r.y + l2.a.y * l2.r.x;
    Point ctr((b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1), (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1));
    return Circle(ctr, Vector(ctr, a).abs());
  }
  Circle inscribedCircle() {
    double r = 2 * area() / perimeter();
    Point a = zeroPoint;
    Point b(a.x + vec[0].x, a.y + vec[0].y);
    Vector v1 = vec[0];
    Vector v2 = vec[1];
    double ang1 = std::asin(std::abs((vec[2] * -1) * vec[0]) / (vec[0].abs() * vec[2].abs())) / 2;
    double ang2 = std::asin(std::abs((vec[1] * -1) * vec[0]) / (vec[0].abs() * vec[1].abs())) / 2;
    v1.rot(ang1);
    v2.rot(ang2);
    Line l1(a, v1);
    Line l2(b, v2);
    double a1 = l1.r.y;
    double b1 = -l1.r.x;
    double c1 = -l1.a.x * l1.r.y + l1.a.y * l1.r.x;
    double a2 = l2.r.y;
    double b2 = -l2.r.x;
    double c2 = -l2.a.x * l2.r.y + l2.a.y * l2.r.x;
    Point ctr((b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1), (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1));
    return Circle(ctr, r);
  }
  Point centroid() {
    Vector am = (vec[0] - vec[2]) * (1 / 3.0);
    Point ctr(zeroPoint.x + am.x, zeroPoint.y + am.y);
    return ctr;
  }
  Point orthocenter() {
    Point a = zeroPoint;
    Point b(a.x + vec[0].x, a.y + vec[0].y);
    Point c(b.x + vec[1].x, b.y + vec[1].y);
    Point o = circumscribedCircle().center();
    Vector oh = Vector(o, a) + Vector(o, b) + Vector(o, c);
    return Point(o.x + oh.x, o.y + oh.y);
  }
  Line EulerLine() {
    return Line(circumscribedCircle().center(), orthocenter());
  }
  Circle ninePointsCircle() {
    Point ctr(0.5 * (circumscribedCircle().center().x + orthocenter().x),
              0.5 * (circumscribedCircle().center().y + orthocenter().y));
    return Circle(ctr, circumscribedCircle().radius() / 2);
  }
};
