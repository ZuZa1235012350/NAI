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
            p = p2;
        }
    }
    return p;
}

int main(){
    string choice;
    double distance;
    int iterations;
    cout << "Choose matyas or easom" << endl;
    cin >> choice;
    cout << "Select distance" << endl;
    cin >> distance;
    cout << "iterations " << endl;
    cin >> iterations;
    if(choice == "matyas") {
        auto matyas = [](vector<double> v){
            double x= v.at(0);
            double y= v.at(1);

            return 0.26*(pow(x,2)+pow(y,2)-0.28*x*y);
        };
        auto matyas_domain = [](vector<double> v){
            return (abs(v[0]) <= 10) && (abs(v[1]) <= 10);
        };

        uniform_real_distribution<> distrib_r(-5,5);
        vector<double> matyas_p0 = {
                distrib_r(gen),
                distrib_r(gen),
        };
        auto result = hill_climbing(matyas, matyas_domain, matyas_p0, iterations, distance);
        cout << result << "-->" << matyas(result) << endl;
    }else if(choice == "easom"){
        auto easom = [](vector<double> v){
            double x= v.at(0);
            double y= v.at(1);

            return -cos(x)*cos(y)*exp(-(pow(x-M_PI,2) + pow(y-M_PI,2)));
        };
        auto easom_domain = [](vector<double> v){
            return (abs(v[0]) <= 100) && (abs(v[1]) <= 100);
        };

        uniform_real_distribution<> distrib_r(-100,100);
        vector<double> easom_p0 = {
                distrib_r(gen),
                distrib_r(gen),
        };

        cout << easom_p0 << endl;
        auto result = hill_climbing(easom, easom_domain, easom_p0, iterations, distance);
        cout << result << "-->" << easom(result) << endl;
    }else{
        cout<< "Wrong choice";
    }

    return 0;
}