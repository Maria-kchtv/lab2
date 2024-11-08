#include <cassert>
#include <iostream>

template<typename T>
class Grid {
public:
    using value_type = T;
    using size_type = unsigned;
    T *data;
    size_type y_size, x_size;

public:

    Grid(T *data, size_type y_size, size_type x_size): data(data), y_size(y_size), x_size(x_size) {
    }

    // first

    ~Grid() {
        delete[] data;
    }

    Grid(const Grid &other): data(new T[other.x_size * other.y_size]), y_size(other.x_size), x_size(other.y_size) {
        std::copy(other.data, other.data + y_size * x_size, data);
    }

    Grid<T> &operator=(Grid &other) {
        if (this != &other) {
            std::copy(other.data, other.data + y_size * x_size, data);
        }
        return *this;
    } // copy operator

    Grid(Grid &&other): data(std::move(other.data)), y_size(other.y_size), x_size(other.x_size) {
        other.data = nullptr;
        other.y_size = 0;
        other.x_size = 0;
    } // move


    Grid<T> &operator=(Grid &&other) {
        if (this != &other) {
            data = nullptr;
        }
        data = std::move(other.data);
        y_size = other.get_y_size();
        x_size = other.get_x_size();
        other.data = nullptr;
        other.y_size = 0;
        other.x_size = 0;
        return *this;
    } // move operator


    Grid(T const &t): data(new T[1]), y_size(1), x_size(1) {
        data[0] = t;
    }

    Grid(size_type y_size, size_type x_size): data(new T[y_size * x_size]()), y_size(y_size), x_size(x_size) {

    }

    Grid(size_type y_size, size_type x_size, T const &t): data(new T[y_size * x_size]), y_size(y_size), x_size(x_size) {
        for (int i = 0; i < y_size * x_size; i++)
            data[i] = t;
    }

    //first

    //second

    T* operator[](size_type y) {
        return data + y * x_size;  // Возвращаем указатель на начало строки
    }


    //second

    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }

    T &operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }

    Grid<T> &operator=(T const &t) {
        for (
            auto it = data, end = data + x_size * y_size;
            it != end; ++it
        )
            *it = t;
        return *this;
    }

    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }
};


int main() {
    // Тест конструктора с одним элементом
    {
        int value = 5;
        Grid<int> grid_single(value);
        assert(grid_single(0, 0) == value);  // Проверка значения элемента (1x1 сетка)
    }


    // Тест конструктора с размерами и инициализацией по умолчанию
    {
        Grid<int> grid_default(3, 3);
        for (unsigned y = 0; y < 3; ++y) {
            for (unsigned x = 0; x < 3; ++x) {
                assert(grid_default(y, x) == int()); // Проверка на значение по умолчанию (0)
            }
        }
    }

    // Тест конструктора с размерами и заполнением заданным значением
    {
        int fill_value = 42;
        Grid<int> grid_fill(2, 2, fill_value);
        for (unsigned y = 0; y < 2; ++y) {
            for (unsigned x = 0; x < 2; ++x) {
                assert(grid_fill(y, x) == fill_value);  // Проверка значения всех ячеек
            }
        }
    }

    // Тест перемещающего конструктора
    {
        Grid<std::string> grid_move_source(2, 2, "test");  // Создаем исходную сетку
        Grid<std::string> grid_move_dest(std::move(grid_move_source));  // Явный вызов перемещающего конструктора

        // Проверяем, что перемещенная сетка имеет правильные размеры и данные
        assert(grid_move_dest.get_y_size() == 2);
        assert(grid_move_dest.get_x_size() == 2);
        for (unsigned y = 0; y < grid_move_dest.get_y_size(); ++y) {
            for (unsigned x = 0; x < grid_move_dest.get_x_size(); ++x) {
                assert(grid_move_dest(y, x) == "test");
            }
        }

        // Проверяем, что исходная сетка находится в корректном состоянии после перемещения
        assert(grid_move_source.data == nullptr);  // Данные должны быть освобождены
        std::cout << "Move constructor test passed.\n";
    }

    // Тест оператора присваивания копированием
    {
        Grid<int> grid1(2, 2, 1);
        Grid<int> grid2(2, 2, 5);
        grid2 = grid1;
        for (unsigned y = 0; y < 2; ++y) {
            for (unsigned x = 0; x < 2; ++x) {
                assert(grid2(y, x) == 1); // Проверка присвоенных значений
            }
        }
    }

    // Тест копирующего конструктора
    {
        Grid<int> grid_orig(3, 3, 7);  // Создаем исходную сетку, заполненную значением 7
        Grid<int> grid_copy(grid_orig);  // Явный вызов копирующего конструктора

        // Проверяем, что копия имеет такие же размеры и данные
        assert(grid_copy.get_y_size() == grid_orig.get_y_size());
        assert(grid_copy.get_x_size() == grid_orig.get_x_size());
        for (unsigned y = 0; y < grid_copy.get_y_size(); ++y) {
            for (unsigned x = 0; x < grid_copy.get_x_size(); ++x) {
                assert(grid_copy(y, x) == 7);
                assert(grid_copy(y, x) == grid_orig(y, x));
            }
        }
        std::cout << "Copy constructor test passed.\n";
    }


    // Тест оператора присваивания для заполнения всех ячеек значением
    {
        Grid<int> grid_assign(3, 3);
        grid_assign = 10;
        for (unsigned y = 0; y < 3; ++y) {
            for (unsigned x = 0; x < 3; ++x) {
                assert(grid_assign(y, x) == 10);  // Проверка, что все ячейки заполнены значением 10
            }
        }
    }


    Grid<float> g(3, 2, 0.0f);
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());
    using gsize_t = Grid<float>::size_type;
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            g[y_idx][x_idx] = 1.0f;
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(1.0f == g(y_idx, x_idx));
    printf("all test passed!!!");
    return 0;
}
