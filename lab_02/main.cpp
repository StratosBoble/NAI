#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <map>

auto brute_force = [](auto f, auto domain) {
    auto current_p = domain();
    auto best_point = current_p;
    try {
        while (true) {
            if (f(current_p) < f(best_point)) {
                best_point = current_p;
            }
            current_p = domain();
        }
    } catch (std::exception &e) {
    }
    return best_point;
};

int main(int argc, char **argv) {
    std::map<std::string, std::function<double (double)>> brute_formatery;
    std::map<std::string, std::function<double (std::vector<double>)>> hill_formatery;
    brute_formatery["sphere"] = [](double x) {return x*x;};
    brute_formatery["rastrigin"] = [](double x) {return x * x - 10 * (cos(2 * std::numbers::pi * x) * std::numbers::pi/180);};
    brute_formatery["rosenbrock"] = [](double x) {return 100 * pow((x - (x * x)),2) + pow((1 - x),2);};
    brute_formatery["styblinskiTang"] = [](double x) {return (pow(x,4) - (16 * pow(x, 2)) + 5 * x)/2; };
    double current_sphere_x = std::stod(argv[2]);
    auto sphere_generator = [&]() {
        current_sphere_x+= 1.0/128.0;
        if (current_sphere_x >= std::stod(argv[3])) throw std::invalid_argument("finished");
        return current_sphere_x;
    };
    auto best_point = brute_force(brute_formatery.at(argv[1]), sphere_generator);
    std::cout << "best x = " << best_point << std::endl;
    return 0;
}
