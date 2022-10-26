#include <functional>
#include <iostream>
#include <list>
#include <random>
#include <vector>

using namespace std;
std::mt19937 mt_generator((std::random_device())());

using domain_t = std::vector<double>;

std::ostream &operator<<(std::ostream &o, domain_t &d) {
    o << d[0] << " " << d[1];
    return o;
}

domain_t hill_climbing(
        const std::function<double(domain_t)> &f, domain_t start_point,
        std::function<std::vector<domain_t>(domain_t)> get_close_points,
        int max_iterations) {
    domain_t best_p = start_point;
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        auto close_points = get_close_points(best_p);
        auto best_neighbour =
                *std::min_element(close_points.begin(), close_points.end(),
                                  [f](auto a, auto b) { return f(a) > f(b); });
        if (f(best_neighbour) < f(best_p)){
            best_p = best_neighbour;
        }
    }
    return best_p;
}

domain_t tabu_method(
        const std::function<double(domain_t)> &f, domain_t start_point,
        std::function<std::vector<domain_t>(domain_t)> get_close_points,
        int max_iterations) {
    using namespace std;
    domain_t best_point = start_point;
    list<domain_t> tabu_list = {start_point};
    for (int iteration = 0; iteration < max_iterations; iteration++) {
//        cout << iteration << " " << tabu_list.back() << " " << f(tabu_list.back())
//             << endl;
        for (auto tabu_i = tabu_list.rbegin(); tabu_i != tabu_list.rend();
             tabu_i++) {
            auto close_points_all = get_close_points(*tabu_i);
            vector<domain_t> close_points;
            copy_if(close_points_all.begin(), close_points_all.end(),
                    back_inserter(close_points), [&](auto p) {
                        return !count(tabu_list.begin(), tabu_list.end(), p);
                    });
            if (close_points.size() != 0) {
                tabu_list.push_back(
                        *min_element(close_points.begin(), close_points.end(),
                                     [f](auto a, auto b) { return f(a) < f(b); }));
                break;
            }
        }
        if (f(best_point) > f(tabu_list.back())) best_point = tabu_list.back();
    }
    return best_point;
}

domain_t wyzarzanie(
        const std::function<double(domain_t)> &f, domain_t start_point,
        std::function<std::vector<domain_t>(domain_t)> get_close_points,
        int max_iterations){
    using namespace std;
    domain_t best_point = start_point;
    list<domain_t> wyzarzanie_list = {best_point};

    for (int iteration = 0; iteration < max_iterations; iteration++){

    }
}

int main() {

    auto holderTable_f_v = [](domain_t x){
        return -abs(sin(x[0]) * cos(x[1]) * exp(abs(1 - sqrt(pow(x[0], 2) + pow(x[1], 2))) / std::numbers::pi));
    };

    auto eggholder_f_v = [](domain_t x){
        return -(x[1] + 47) * abs(sin(sqrt(x[0]/2 + (x[1] + 47)))) - x[0] * sin(sqrt(abs(x[0] - (x[1] + 47))));
    };

    auto mcCormick_f_v = [](domain_t x){
        return sin(x[0] + x[1]) + pow(x[0] - x[1], 2) - 1.5*x[0] + 2.5*x[1] + 1;
    };

    auto get_random_point = []() -> domain_t {
        std::uniform_real_distribution<double> distr(-512, 512);
        return {distr(mt_generator), distr(mt_generator)};
    };

    auto get_close_points = [](domain_t p0) -> std::vector<domain_t> {
        std::vector<domain_t> ret;
        for (int i = 0; i < p0.size(); i++) {
            domain_t v = p0;
            v[i] += 1.0 / 128.0;
            ret.push_back(v);
            v = p0;
            v[i] -= 1.0 / 128.0;
            ret.push_back(v);
        }
        return ret;
    };

//    auto best_tab0 = tabu_method(holderTable_f_v, get_random_point(), get_close_points, 10000);
//    std::cout << "# tabu x = " << best_tab0[0] << " " << best_tab0[1] << std::endl;
//    auto best_tab1 = hill_climbing(holderTable_f_v, get_random_point(), get_close_points, 10000);
//    std::cout << "# hill x = " << best_tab1[0] << " " << best_tab1[1] << std::endl;
//    auto best_tab2 = wyzarzanie(holderTable_f_v, get_random_point(), get_close_points, 10000);
//    std::cout << "# wyzarzanie x = " << best_tab2[0] << " " << best_tab2[1] << std::endl;

    auto best_egg0 = tabu_method(eggholder_f_v, get_random_point(), get_close_points, 10000);
    std::cout << "# tabu x = " << best_egg0[0] << " " << best_egg0[1] << std::endl;
    auto best_egg1 = hill_climbing(eggholder_f_v, get_random_point(), get_close_points, 10000);
    std::cout << "# hill x = " << best_egg1[0] << " " << best_egg1[1] << std::endl;
//    auto best_egg2 = wyzarzanie(eggholder_f_v, get_random_point(), get_close_points, 10000);
//    std::cout << "# wyzarzanie x = " << best_egg2[0] << " " << best_egg2[1] << std::endl;

//    auto best_corm0 = tabu_method(mcCormick_f_v, get_random_point(), get_close_points, 10000);
//    std::cout << "# tabu x = " << best_corm0[0] << " " << best_corm0[1] << std::endl;
//    auto best_corm1 = hill_climbing(mcCormick_f_v, get_random_point(), get_close_points, 10000);
//    std::cout << "# hill x = " << best_corm1[0] << " " << best_corm1[1] << std::endl;
//    auto best_corm2 = wyzarzanie(mcCormick_f_v, get_random_point(), get_close_points, 10000);
//    std::cout << "# wyzarzanie x = " << best_corm2[0] << " " << best_corm2[1] << std::endl;

    return 0;
}
