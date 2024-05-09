#include <iostream>
#include <cmath>

using namespace std;

// функция, интеграл которой вычисляем
double f(const double x, const double y, const double z) {
    return pow((sin(x)*cos(y)*tanh(z)), 2);
}

// функция считает интеграл f(x, y, z) = (sin(x)*cos(y)*tanh(z))^2 в области x = y = z = [0, pi]
// трапециидальным способом, разбивая область на отрезки длиной d
double calc(const double d) {
    double result = 0;
    const int steps = M_PI / d;
    // запускаем расчёты в паралелльно все 3 цикла на 20 потоках
    #pragma omp parallel for collapse(3) num_threads(20) default(none) shared(d, steps) reduction(+:result)
    for (int xi = 0; xi <= steps; xi++) {
        for (int yi = 0; yi <= steps; yi++) {
            for (int zi = 0; zi <= steps; zi++) {
                const double x = xi * d;
                const double next_x = min(x + d, M_PI);
                const double y = yi * d;
                const double next_y = min(y + d, M_PI);
                const double z = zi * d;
                const double next_z = min(z + d, M_PI);

                const double area = (next_x - x) * (next_y - y) * (next_z - z);

                const double func_val =
                    (f(x, y, z) + f(x, y, next_z) + f(x, next_y, z) + f(x, next_y, next_z) +
                     f(next_x, y, z) + f(next_x, y, next_z) + f(next_x, next_y, z) + f(next_x, next_y, next_z)) / 8.0;

                result += func_val * area;
            }
        }
    }
    return result;
}

int main() {
    cout << "Enter the precision" << endl;
    double precision;
    cin >> precision;

    double h = 1; //шаг интегрирования
    double v = calc(h); // вычисляем первое значение
    // сравниваем текущее значение со значением с меньшим шагом интегрирования
    while (abs(v - calc(h / 2)) > precision) {
        h = h / 2;
        v = calc(h);
    }
    cout << "I = " << calc(h/2) << endl;
    cout << "h = " << h / 2;
    return 0;
}
