#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

using mojamapa_t = std::map<std::string, int>;
using moja_funkcja = std::function<double (mojamapa_t)>;

void wypisz(mojamapa_t mapa, moja_funkcja fun) {
    using namespace std;
    cout << fun(mapa) << endl;
}

int main(int argc, char **argv) {
    using namespace std;
    vector<string> argumenty(argv, argv + argc);
    map<string, int> mapa;
    map<string, moja_funkcja> formatery;

    for (int i = 2; i < argumenty.size(); i++) {
        mapa.insert({{"arg" + to_string(i - 1), stoi(argumenty.at(i))}});
    }

    formatery["add"] = [](mojamapa_t map) {return map.at("arg1") + map.at("arg2"); };
    formatery["mod"] = [](mojamapa_t map) {return map.at("arg1") % map.at("arg2"); };
    formatery["sin"] = [](mojamapa_t map) {
        double x = map.at("arg1");
        return sin(x);
    };


    try{
        auto selected_f = argumenty.at(1);
        wypisz(mapa, formatery.at(selected_f));
    } catch (std::out_of_range aor) {
        cout << "Liczy wartosc funkcji FUNKCJA z podanymi ARGUMENT-ami." << endl;
        cout << "Wynik jest wypisywany na standardowe wyjscie." << endl;
    }
    return 0;
}