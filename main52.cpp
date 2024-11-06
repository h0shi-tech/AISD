#include <iostream>
#include <cmath>
#include <stdexcept>
#include <random>
#include <complex>

template<typename T>
struct Point {
    T x, y;

    Point(T x_ = T(), T y_ = T()) : x(x_), y(y_) {}

    bool operator==(const Point& other) const {
        if constexpr (std::is_floating_point<T>::value || std::is_same<T, std::complex<float>>::value || std::is_same<T, std::complex<double>>::value) {
            return std::abs(x - other.x) < Line::EPSILON && std::abs(y - other.y) < Line::EPSILON;
        } else {
            return x == other.x && y == other.y;
        }
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

template<typename T>
class Line {
    Point<T>* vertices;
    size_t size;
    static constexpr double EPSILON = 1e-5;

public:
    Line(size_t n) : size(n) {
        if (n <= 0) throw std::invalid_argument("Number of points must be positive.");
        vertices = new Point<T>[n];
    }

    Line(const std::initializer_list<Point<T>>& points) : Line(points.size()) {
        std::copy(points.begin(), points.end(), vertices);
    }

    Line(const T& m1, const T& m2, size_t n) : Line(n) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(std::real(m1), std::real(m2));
        for (size_t i = 0; i < n; ++i) {
            vertices[i] = Point<T>(T(dist(gen)), T(dist(gen)));
        }
    }

    Line(const Line& other) : Line(other.size) {
        std::copy(other.vertices, other.vertices + other.size, vertices);
    }

    Line& operator=(const Line& other) {
        if (this == &other) return *this;
        delete[] vertices;
        size = other.size;
        vertices = new Point<T>[size];
        std::copy(other.vertices, other.vertices + size, vertices);
        return *this;
    }

    ~Line() { delete[] vertices; }

    Point<T>& operator[](size_t idx) {
        if (idx >= size) throw std::out_of_range("Index out of range.");
        return vertices[idx];
    }

    const Point<T>& operator[](size_t idx) const {
        if (idx >= size) throw std::out_of_range("Index out of range.");
        return vertices[idx];
    }

    Line operator+(const Line& other) const {
        Line result(size + other.size);
        std::copy(vertices, vertices + size, result.vertices);
        std::copy(other.vertices, other.vertices + other.size, result.vertices + size);
        return result;
    }

    Line operator+(const Point<T>& point) const {
        Line result(size + 1);
        std::copy(vertices, vertices + size, result.vertices);
        result.vertices[size] = point;
        return result;
    }

    friend Line operator+(const Point<T>& point, const Line& line) {
        Line result(line.size + 1);
        result.vertices[0] = point;
        std::copy(line.vertices, line.vertices + line.size, result.vertices + 1);
        return result;
    }

    double length() const {
        double totalLength = 0.0;
        for (size_t i = 1; i < size; ++i) {
            if constexpr (std::is_same<T, std::complex<float>>::value || std::is_same<T, std::complex<double>>::value) {
                totalLength += std::abs(vertices[i] - vertices[i - 1]);
            } else {
                double dx = static_cast<double>(vertices[i].x) - static_cast<double>(vertices[i - 1].x);
                double dy = static_cast<double>(vertices[i].y) - static_cast<double>(vertices[i - 1].y);
                totalLength += std::sqrt(dx * dx + dy * dy);
            }
        }
        return totalLength;
    }

    bool operator==(const Line& other) const {
        if (size != other.size) return false;
        for (size_t i = 0; i < size; ++i) {
            if (vertices[i] != other.vertices[i]) return false;
        }
        return true;
    }

    bool operator!=(const Line& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Line& line) {
        os << "Line with " << line.size << " points:\n";
        for (size_t i = 0; i < line.size; ++i) {
            os << "(" << line.vertices[i].x << ", " << line.vertices[i].y << ")\n";
        }
        return os;
    }
};

int main() {
    Line<int> line1{{Point<int>(0, 0), Point<int>(0, 5), Point<int>(5, 5), Point<int>(5, 0)}};
    std::cout << line1 << "\n";

    Point<int> p(3, 3);
    Line<int> line2 = line1 + p;
    std::cout << "After adding point (3, 3):\n" << line2 << "\n";

    std::cout << "Length of line1: " << line1.length() << "\n";
    std::cout << "Length of line2: " << line2.length() << "\n";

    return 0;
}
