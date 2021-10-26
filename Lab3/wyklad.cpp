#include <iostream>
#include <vector>
#include <functional>
#include <random>


using namespace std;

random_device rd;
mt19937 gen(rd());

/*
x = losowy pkt dziedziny funkcji

 powtarzaj n razy
    x' = x+rand()
    y=f(x')
    jezeli y > f(x)
        x=x'
 */
//function<double(vector<double>)> -- funkcja zwracaj typ double  a przyjmuje wektor
//p0- punkt startu wspiinaczki

ostream &operator<<(ostream &o, vector<double> v){
    for(auto e :v){
        o << e << ",";
    }
    return o;
}

vector<double> hill_climbing(function<double(vector<double>)> f,function<bool(vector<double>)> f_domain, vector<double> p0, int iteration){
    auto p=p0;

    uniform_int_distribution<> distrib(0,p.size()-1);
    uniform_real_distribution<> distrib_r(-0.1,0.1);

    if(!f_domain(p)) throw std::invalid_argument("The p0 must be in domain");
    for(int i=0; i < iteration; i++){
        auto p2= p;
        p[distrib(gen)] += distrib_r(gen); //na losowo wybranej wspolrzednej wektora
        //zmieniamy wartosc o maly wpolczynnik wylosowany
        double y2= f(p2); // oblicza wartosc funkcji optymalizowanej "celu" dla tego punktu
        if(y2 < f(p)) {
            p = p2;
        }
    }
    return p;
}

int main() {
    auto function1 = [](vector<double> v){
        return 100 - v[0]*v[0];
    };
    auto akley = [](vector<double> v){
        double x= v.at(0);
        double y= v.at(1);
        //wektor dwuwymiarowy
        return -20*exp(-0.2* sqrt(0.5*(x*x+y*y)))
        -exp(0.5*(cos(2*M_PI*x)+cos(2*M_PI*y))) + M_E +20.0;
    };
    auto akley_domain = [](vector<double> v){
        return (abs(v[0]) <= 5) && (abs(v[1]) <= 5);
    };

    uniform_real_distribution<> distrib_r(-5,5);
    vector<double> akley_p0 = {
            distrib_r(gen),
            distrib_r(gen),
    };
    auto result = hill_climbing(akley,  akley_domain, akley_p0, 1000);
    cout << result << "-->" << akley(result) << endl;
    return  0;
}

