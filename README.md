### АИСД
## Описание
# Конструкторы:

Конструктор Line(size_t n) создаёт ломаную с n точками.
Конструктор с initializer_list для инициализации от списка точек.
Конструктор Line(const T& m1, const T& m2, size_t n) создаёт n случайных точек в диапазоне [m1, m2].
# Операторы:

Оператор []: возвращает ссылку на точку по индексу (с проверкой границ).
Оператор +: конкатенация двух ломаных или добавление точки в начало/конец.
Операторы == и != для сравнения ломаных.
Оператор вывода << для отображения.
Метод length() вычисляет длину ломаной
