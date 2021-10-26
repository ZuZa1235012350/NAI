#include <iostream>
#include <vector>
#include <functional>
#include <random>


using namespace std;

random_device rd;
mt19937 gen(rd());

ostream &operator<<(ostream &o, vector<double> v){
    for(auto e :v){
        o << e << ",";
    }
    return o;
}

vector<double> hill_climbing(function<double(vector<double>)> f,function<bool(vector<double>)> f_domain, vector<double> p0, int iteration, double dys){
    auto p=p0;

    uniform_int_distribution<> distrib(0,p.size()-1);
    uniform_real_distribution<> distrib_r(-dys,dys);

    if(!f_domain(p)) throw std::invalid_argument("The p0 must be in domain");
    for(int i=0; i < iteration; i++){
        auto p2= p;
        p[distrib(gen)] += distrib_r(gen);
        double y2= f(p2);
        if(y2 < f(p)) {
            if(f_domain(p2)){ p = p2;}

        }
    }
    return p;
}

vector<double> simulated_annealing(
        function<double(vector<double>)> f,
        function<bool(vector<double>)> f_domain,
        vector<double> p0,
        int iterations,
        function<vector<double>(vector<double>)> N,
        function<double(int)> T)
{
    auto s_current = p0;
    auto s_global_best = p0;

    //    uniform_int_distribution<> distrib(0, p.size() - 1);
    uniform_real_distribution<> u_k(0.0, 1.0);

    if (!f_domain(s_current)) throw std::invalid_argument("The p0 point must be in domain");

    for (int k = 0; k < iterations; k++) {
        auto s_next = N(s_current);
        if (f(s_next) < f(s_current)) {
            s_current = s_next;
        } else {
            double u = u_k(gen);
            if (u < exp(-abs(f(s_next) - f(s_current)) / T(k))) {
                s_current = s_next;
            } else {
                // nothing...
            }
        }
        if (f(s_current) < f(s_global_best)) {
            s_global_best = s_current;
        }
        //        cout << k << " " << f(s_current) << endl;

    }
    cout << s_current << " " << f(s_current) << endl;
    return s_global_best;
}

int main(){
    string choice;
    double distance;
    int iterations = 1000;
    string whichFunction;
    cout << "Choose hill_climbing or simulated_annealing" << endl;
    cin>> whichFunction;
    cout << "Choose matyas or easom" << endl;
    cin >> choice;
    cout << "Select distance" << endl;
    cin >> distance;

    if(whichFunction == "hill_climbing") {
        if (choice == "matyas") {
            auto matyas = [](vector<double> v) {
                double x = v.at(0);
                double y = v.at(1);

                return 0.26 * (pow(x, 2) + pow(y, 2) - 0.28 * x * y);
            };
            auto matyas_domain = [](vector<double> v) {
                return (abs(v[0]) <= 10) && (abs(v[1]) <= 10);
            };

            uniform_real_distribution<> distrib_r(-5, 5);
            vector<double> matyas_p0 = {
                    distrib_r(gen),
                    distrib_r(gen),
            };
            auto result = hill_climbing(matyas, matyas_domain, matyas_p0, iterations, distance);
            cout << result << "-->" << matyas(result) << endl;
        } else if (choice == "easom") {
            auto easom = [](vector<double> v) {
                double x = v.at(0);
                double y = v.at(1);

                return -cos(x) * cos(y) * exp(-(pow(x - M_PI, 2) + pow(y - M_PI, 2)));
            };
            auto easom_domain = [](vector<double> v) {
                return (abs(v[0]) <= 100) && (abs(v[1]) <= 100);
            };

            uniform_real_distribution<> distrib_r(-100, 100);
            vector<double> easom_p0 = {
                    distrib_r(gen),
                    distrib_r(gen),
            };

            cout << easom_p0 << endl;
            auto result = hill_climbing(easom, easom_domain, easom_p0, iterations, distance);
            cout << result << "-->" << easom(result) << endl;
        } else {
            cout << "Wrong choice";
        }
    }else if(whichFunction == "simulated_annealing"){
        if (choice == "matyas") {
            auto matyas = [](vector<double> v) {
                double x = v.at(0);
                double y = v.at(1);

                return 0.26 * (pow(x, 2) + pow(y, 2) - 0.28 * x * y);
            };
            auto matyas_domain = [](vector<double> v) {
                return (abs(v[0]) <= 10) && (abs(v[1]) <= 10);
            };

            uniform_real_distribution<> distrib_r(-5, 5);
            vector<double> matyas_p0 = {
                    distrib_r(gen),
                    distrib_r(gen),
            };
            cout << matyas_p0 << endl;
            auto result = simulated_annealing(
                matyas, matyas_domain, matyas_p0, iterations,
                [](auto p) {
                    normal_distribution<double> n(0.0, 0.3);
                    for (auto& e : p) {
                        e = e + n(gen);
                    }
                    return p;
                },
                [](int k) { return 1000.0 / k; });
        cout << result << " -> " << matyas(result) << endl;
        } else if (choice == "easom") {
            auto easom = [](vector<double> v) {
                double x = v.at(0);
                double y = v.at(1);

                return -cos(x) * cos(y) * exp(-(pow(x - M_PI, 2) + pow(y - M_PI, 2)));
            };
            auto easom_domain = [](vector<double> v) {
                return (abs(v[0]) <= 100) && (abs(v[1]) <= 100);
            };

            uniform_real_distribution<> distrib_r(-100, 100);
            vector<double> easom_p0 = {
                    distrib_r(gen),
                    distrib_r(gen),
            };

            cout << easom_p0 << endl;
            auto result = simulated_annealing(
                easom, easom_domain, easom_p0, iterations,
                [](auto p) {
                    normal_distribution<double> n(0.0, 0.3);
                    for (auto& e : p) {
                        e = e + n(gen);
                    }
                    return p;
                },
                [](int k) { return 1000.0 / k; });
        cout << result << " -> " << easom(result) << endl;
        } else {
            cout << "Wrong choice";
        }
    }

    return 0;
}