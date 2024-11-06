#include <iostream>
#include <cmath>
#include <stdexcept>
#include <random>
#include <complex>

// ��������� ������� Point ��� �।�⠢����� �窨 �� ���᪮�� � ���न��⠬� x � y
template<typename T>
struct Point {
    T x, y;  // ���न���� �窨
    static constexpr double EPSILON = 1e-5;  // ��筮��� �ࠢ����� ��� ����⢥���� �ᥫ

    // ��������� �� 㬮�砭��, � ������������ ������ ��砫�� ���祭�� x � y
    Point(T x_ = T(), T y_ = T()) : x(x_), y(y_) {}

    // ��ॣ�㧪� ������ == ��� �ࠢ����� �祪
    bool operator==(const Point& other) const {
        // �᫨ ⨯ ������ T ? ����⢥���� �᫮ ��� �������᭮� �᫮,
        // � �ᯮ������ �ࠢ����� � �筮���� EPSILON
        if constexpr (std::is_floating_point<T>::value || 
                      std::is_same<T, std::complex<float>>::value || 
                      std::is_same<T, std::complex<double>>::value) {
            return std::abs(x - other.x) < EPSILON && std::abs(y - other.y) < EPSILON;
        } else {
            // ��� 楫�� �ᥫ � ��㣨� ⨯�� �ࠢ����� �� ࠢ�����
            return x == other.x && y == other.y;
        }
    }

    // ��ॣ�㧪� ������ != ��� �஢�ન �� ��ࠢ���⢮
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

// �������� ����� Line ��� �।�⠢����� ������� ����� � �ந������ �᫮� �祪
template<typename T>
class Line {
    Point<T>* vertices; // ���ᨢ �祪 (���設 �������)
    size_t size;        // ������⢮ �祪 � �������

public:
    static constexpr double EPSILON = 1e-5;  // �筮��� ��� �ࠢ����� �������

    // ��������� ��� ᮧ����� ������� � ������� ������⢮� �祪
    Line(size_t n) : size(n) {
        if (n <= 0) throw std::invalid_argument("Number of points must be positive."); // �஢�ઠ
        vertices = new Point<T>[n]; // �뤥����� ����� ��� �祪
    }

    // ��������� � ���樠�����஬, ᮧ���騩 ������� �� ��������� ᯨ᪠ �祪
    Line(const std::initializer_list<Point<T>>& points) : Line(points.size()) {
        std::copy(points.begin(), points.end(), vertices); // �����㥬 �窨 �� ᯨ᪠
    }

    // ��������� ��� ᮧ����� ������� � �窠�� � ��砩��� ��������� [m1, m2]
    Line(const T& m1, const T& m2, size_t n) : Line(n) {
        std::random_device rd;
        std::mt19937 gen(rd());  // ������� ��砩��� �ᥫ
        std::uniform_real_distribution<> dist(std::real(m1), std::real(m2)); // �������� ��� ����⢥���� ��⥩
        for (size_t i = 0; i < n; ++i) {
            vertices[i] = Point<T>(T(dist(gen)), T(dist(gen))); // ᮧ����� �祪 � ���न��⠬� � ��������� [m1, m2]
        }
    }

    // ��������� ����஢����
    Line(const Line& other) : Line(other.size) {
        std::copy(other.vertices, other.vertices + other.size, vertices); // �����㥬 �窨 �� ��㣮� �������
    }

    // ������ ��ᢠ������
    Line& operator=(const Line& other) {
        if (this == &other) return *this; // �஢�ઠ �� ᠬ���ᢠ������
        delete[] vertices; // �᢮������� ����� ������
        size = other.size;
        vertices = new Point<T>[size]; // �뤥�塞 ����� ������
        std::copy(other.vertices, other.vertices + size, vertices); // �����㥬 �����
        return *this;
    }

    // �������� ��� �᢮�������� �����
    ~Line() { delete[] vertices; }

    // ��ॣ�㧪� ������ [], ���������� �������� � �窥 �� �������
    Point<T>& operator[](size_t idx) {
        if (idx >= size) throw std::out_of_range("Index out of range."); // �஢�ઠ ������
        return vertices[idx];
    }

    const Point<T>& operator[](size_t idx) const {
        if (idx >= size) throw std::out_of_range("Index out of range.");
        return vertices[idx];
    }

    // ������ + ��� �����⥭�樨 ���� ������� (���������� ����� ������� � ����� ��㣮�)
    Line operator+(const Line& other) const {
        Line result(size + other.size);
        std::copy(vertices, vertices + size, result.vertices); // �����㥬 ����� �������
        std::copy(other.vertices, other.vertices + other.size, result.vertices + size); // �����㥬 ����� �������
        return result;
    }

    // ������ + ��� ���������� �窨 � ����� �������
    Line operator+(const Point<T>& point) const {
        Line result(size + 1);
        std::copy(vertices, vertices + size, result.vertices); // �����㥬 �������騥 �窨
        result.vertices[size] = point; // ������塞 ����� ���
        return result;
    }

    // ������ + ��� ���������� �窨 � ��砫� ������� (��㦥�⢥��� ��⮤)
    friend Line operator+(const Point<T>& point, const Line& line) {
        Line result(line.size + 1);
        result.vertices[0] = point; // ��ࢠ� �窠 ? ����� �窠
        std::copy(line.vertices, line.vertices + line.size, result.vertices + 1); // �����㥬 ��⠢訥�� �窨
        return result;
    }

    // ��⮤ ��� ���᫥��� ����� �������
    double length() const {
        double totalLength = 0.0;
        for (size_t i = 1; i < size; ++i) {
            if constexpr (std::is_same<T, std::complex<float>>::value || 
                          std::is_same<T, std::complex<double>>::value) {
                totalLength += std::abs(vertices[i] - vertices[i - 1]); // ��� ���������� �祪 �ᯮ��㥬 abs
            } else {
                // ��� ����⢥���� �祪 �ᯮ��㥬 ��������� ���ਪ�
                double dx = static_cast<double>(vertices[i].x) - static_cast<double>(vertices[i - 1].x);
                double dy = static_cast<double>(vertices[i].y) - static_cast<double>(vertices[i - 1].y);
                totalLength += std::sqrt(dx * dx + dy * dy);
            }
        }
        return totalLength;
    }

    // ������ == ��� �ࠢ����� ���� �������
    bool operator==(const Line& other) const {
        if (size != other.size) return false; // ������ ࠧ���� ࠧ��� �� ࠢ��
        for (size_t i = 0; i < size; ++i) {
            if (vertices[i] != other.vertices[i]) return false; // �஢�ઠ �祪 �� ��।�
        }
        return true;
    }

    // ������ != ��� �஢�ન ��ࠢ���⢠ �������
    bool operator!=(const Line& other) const {
        return !(*this == other);
    }

    // ��㦥�⢥��� ������ << ��� �뢮�� �������
    friend std::ostream& operator<<(std::ostream& os, const Line& line) {
        os << "Line with " << line.size << " points:\n";
        for (size_t i = 0; i < line.size; ++i) {
            os << "(" << line.vertices[i].x << ", " << line.vertices[i].y << ")\n"; // �뢮��� ���न���� ������ �窨
        }
        return os;
    }

    // ��⮤ ��� ᮧ����� ������� � ���� �㪢� "H" � �������� ���⮩ � �ਭ��
    static Line createHShape(T width, T height) {
        return Line({
            Point<T>(0, 0),             // ����� ������ �窠
            Point<T>(0, height),        // ����� ������ �窠
            Point<T>(width / 2, height / 2), // 業�� ᮥ�������
            Point<T>(width, height),    // �ࠢ�� ������ �窠
            Point<T>(width, 0)          // �ࠢ�� ������ �窠
        });
    }
};

int main() {
    // ������� ������� � �ଥ �㪢� "H" � ���⮩ 10 � �ਭ�� 6
    Line<int> hShape = Line<int>::createHShape(6, 10);

    // �뢮��� ᮧ������ ������� � �ଥ �㪢� "H"
    std::cout << "Line in the shape of H:\n" << hShape << "\n";

    std::cout << "Length of H-shaped line: " << hShape.length() << "\n";

    return 0;
}
