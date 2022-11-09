#include <iostream>
#include <vector>
#include <functional>
#include <random>

std::random_device rd;
std::mt19937 mt_generator(rd());

using namespace std;
using chromosome_t = std::vector<int>;
using population_t = std::vector<chromosome_t>;

population_t generate_population(int size) {
    population_t pop;

    for (int i = 0; i < size; ++i) {
        chromosome_t ch;
        chromosome_t tmp;

        for (int j = 0; j < size ; ++j) {
            uniform_int_distribution<int> uni(0, 1);
            ch.push_back(uni(mt_generator));
        }
        pop.push_back(ch);
    }

    return pop;
}

pair<double, double> decode(chromosome_t chromosome) {
    double x = 0.0;
    double y = 0.0;

    for (int i = 0; i < chromosome.size() / 2; i++) {
        x =  x * 2 + chromosome[i];
    }

    for (int i = chromosome.size() / 2; i < chromosome.size(); i++) {
        y = y * 2 + chromosome[i];
    }

    x = x / pow(2.0, (chromosome.size() / 2 - 4)) ;
    y = y / pow(2.0, (chromosome.size() / 2 - 4)) ;

    return {x, y};
}

auto egg_f = [](pair<double, double> pair) {
    return -(pair.second + 47) * abs(sin(sqrt(pair.first/2 + (pair.second + 47)))) - pair.first * sin(sqrt(abs(pair.first - (pair.second + 47))));
};

double fitness_f(chromosome_t chromosome) {
    return 1.0 / (1.0 + abs(egg_f(decode(chromosome))));
}

int main() {
    population_t population = generate_population(100 + (22204 % 10) * 2);

    for (auto &chromosome: population) {
        auto decoded = decode(chromosome);
        cout << decoded.first << ", " << decoded.second << " | " << fitness_f(chromosome) << endl;
    }

   return 0;
}
