#include <iostream>
#include <cmath>
#include <stdexcept>
#include <random>
#include <complex>

// Шаблонная структура Point для представления точки на плоскости с координатами x и y
template<typename T>
struct Point {
    T x, y;  // координаты точки
    static constexpr double EPSILON = 1e-5;  // Точность сравнения для вещественных чисел

    // Конструктор по умолчанию, с возможностью задать начальные значения x и y
    Point(T x_ = T(), T y_ = T()) : x(x_), y(y_) {}

    // Перегрузка оператора == для сравнения точек
    bool operator==(const Point& other) const {
        // Если тип данных T ? вещественное число или комплексное число,
        // то используется сравнение с точностью EPSILON
        if constexpr (std::is_floating_point<T>::value || 
                      std::is_same<T, std::complex<float>>::value || 
                      std::is_same<T, std::complex<double>>::value) {
            return std::abs(x - other.x) < EPSILON && std::abs(y - other.y) < EPSILON;
        } else {
            // Для целых чисел и других типов сравнение по равенству
            return x == other.x && y == other.y;
        }
    }

    // Перегрузка оператора != для проверки на неравенство
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

// Шаблонный класс Line для представления ломаной линии с произвольным числом точек
template<typename T>
class Line {
    Point<T>* vertices; // массив точек (вершин ломаной)
    size_t size;        // количество точек в ломаной

public:
    static constexpr double EPSILON = 1e-5;  // точность для сравнения ломаных

    // Конструктор для создания ломаной с заданным количеством точек
    Line(size_t n) : size(n) {
        if (n <= 0) throw std::invalid_argument("Number of points must be positive."); // проверка
        vertices = new Point<T>[n]; // выделение памяти для точек
    }

    // Конструктор с инициализатором, создающий ломаную из заданного списка точек
    Line(const std::initializer_list<Point<T>>& points) : Line(points.size()) {
        std::copy(points.begin(), points.end(), vertices); // копируем точки из списка
    }

    // Конструктор для создания ломаной с точками в случайном диапазоне [m1, m2]
    Line(const T& m1, const T& m2, size_t n) : Line(n) {
        std::random_device rd;
        std::mt19937 gen(rd());  // генератор случайных чисел
        std::uniform_real_distribution<> dist(std::real(m1), std::real(m2)); // диапазон для вещественных частей
        for (size_t i = 0; i < n; ++i) {
            vertices[i] = Point<T>(T(dist(gen)), T(dist(gen))); // создание точек с координатами в диапазоне [m1, m2]
        }
    }

    // Конструктор копирования
    Line(const Line& other) : Line(other.size) {
        std::copy(other.vertices, other.vertices + other.size, vertices); // копируем точки из другой ломаной
    }

    // Оператор присваивания
    Line& operator=(const Line& other) {
        if (this == &other) return *this; // проверка на самоприсваивание
        delete[] vertices; // освобождаем старую память
        size = other.size;
        vertices = new Point<T>[size]; // выделяем новую память
        std::copy(other.vertices, other.vertices + size, vertices); // копируем данные
        return *this;
    }

    // Деструктор для освобождения памяти
    ~Line() { delete[] vertices; }

    // Перегрузка оператора [], позволяющая обращаться к точке по индексу
    Point<T>& operator[](size_t idx) {
        if (idx >= size) throw std::out_of_range("Index out of range."); // проверка индекса
        return vertices[idx];
    }

    const Point<T>& operator[](size_t idx) const {
        if (idx >= size) throw std::out_of_range("Index out of range.");
        return vertices[idx];
    }

    // Оператор + для конкатенации двух ломаных (добавление одной ломаной в конец другой)
    Line operator+(const Line& other) const {
        Line result(size + other.size);
        std::copy(vertices, vertices + size, result.vertices); // копируем первую ломаную
        std::copy(other.vertices, other.vertices + other.size, result.vertices + size); // копируем вторую ломаную
        return result;
    }

    // Оператор + для добавления точки в конец ломаной
    Line operator+(const Point<T>& point) const {
        Line result(size + 1);
        std::copy(vertices, vertices + size, result.vertices); // копируем существующие точки
        result.vertices[size] = point; // добавляем новую точку
        return result;
    }

    // Оператор + для добавления точки в начало ломаной (дружественный метод)
    friend Line operator+(const Point<T>& point, const Line& line) {
        Line result(line.size + 1);
        result.vertices[0] = point; // первая точка ? новая точка
        std::copy(line.vertices, line.vertices + line.size, result.vertices + 1); // копируем оставшиеся точки
        return result;
    }

    // Метод для вычисления длины ломаной
    double length() const {
        double totalLength = 0.0;
        for (size_t i = 1; i < size; ++i) {
            if constexpr (std::is_same<T, std::complex<float>>::value || 
                          std::is_same<T, std::complex<double>>::value) {
                totalLength += std::abs(vertices[i] - vertices[i - 1]); // для комплексных точек используем abs
            } else {
                // Для вещественных точек используем Евклидову метрику
                double dx = static_cast<double>(vertices[i].x) - static_cast<double>(vertices[i - 1].x);
                double dy = static_cast<double>(vertices[i].y) - static_cast<double>(vertices[i - 1].y);
                totalLength += std::sqrt(dx * dx + dy * dy);
            }
        }
        return totalLength;
    }

    // Оператор == для сравнения двух ломаных
    bool operator==(const Line& other) const {
        if (size != other.size) return false; // ломаные разного размера не равны
        for (size_t i = 0; i < size; ++i) {
            if (vertices[i] != other.vertices[i]) return false; // проверка точек по очереди
        }
        return true;
    }

    // Оператор != для проверки неравенства ломаных
    bool operator!=(const Line& other) const {
        return !(*this == other);
    }

    // Дружественный оператор << для вывода ломаной
    friend std::ostream& operator<<(std::ostream& os, const Line& line) {
        os << "Line with " << line.size << " points:\n";
        for (size_t i = 0; i < line.size; ++i) {
            os << "(" << line.vertices[i].x << ", " << line.vertices[i].y << ")\n"; // выводим координаты каждой точки
        }
        return os;
    }

    // Метод для создания ломаной в виде буквы "H" с заданной высотой и шириной
    static Line createHShape(T width, T height) {
        return Line({
            Point<T>(0, 0),             // левая нижняя точка
            Point<T>(0, height),        // левая верхняя точка
            Point<T>(width / 2, height / 2), // центр соединения
            Point<T>(width, height),    // правая верхняя точка
            Point<T>(width, 0)          // правая нижняя точка
        });
    }
};

int main() {
    // Создаем ломаную в форме буквы "H" с высотой 10 и шириной 6
    Line<int> hShape = Line<int>::createHShape(6, 10);

    // Выводим созданную ломаную в форме буквы "H"
    std::cout << "Line in the shape of H:\n" << hShape << "\n";

    std::cout << "Length of H-shaped line: " << hShape.length() << "\n";

    return 0;
}
