#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>
#include <fstream>
#include <filesystem>
using namespace std;
/*
  MATH PORTION OF THE CODE
*/
// reprezentacja punktu pint 2d czyli (x,y)
using point2d = array<double, 2>;
inline point2d operator+(const point2d a, const point2d b)
{
    return {a[0] + b[0], a[1] + b[1]};
}
// mozenie punktu razy stala
inline point2d operator*(const point2d a, const double b)
{
    return {a[0] * b, a[1] * b};
}
// odejmowanie punktow
inline point2d operator-(const point2d a, const point2d b)
{
    return a + (b * -1.0);
}
// mnożenie poszczegolnych wartosci wektora czyli punktow
inline point2d operator%(const point2d a, const point2d b)
{
    return {a[0] * b[0], a[1] * b[1]};
}
// iloczyn skalarny
inline double operator*(const point2d a, const point2d b)
{
    auto r = a % b;
    // sumowanie elementów [r.begin do r.end()) do init czyli tego 0.0
    return accumulate(r.begin(), r.end(), 0.0);
}
// oblicznie dlugosci
inline double length(const point2d a)
{
    return sqrt(a * a);
}
// wypisanie
inline ostream &operator<<(ostream &o, const point2d a)
{
    o << a[0] << " " << a[1];
    return o;
}

////To jest moje zadanie
double pDistance(point2d point, point2d targetStart, point2d targetEnd)
{

    double A = point[0] - targetStart[0];
    double B = point[1] - targetStart[1];
    double C = targetEnd[0] - targetStart[0];
    double D = targetEnd[1] - targetStart[1];

    auto dot = A * C + B * D;
    auto len_sq = C * C + D * D;
    auto param = -1;
    if (len_sq != 0)
    {
        // in case of 0 length line
        param = dot / len_sq;
    }

    double xx, yy;

    if (param < 0)
    {
        xx = targetStart[0];
        yy = targetStart[1];
    }
    else if (param > 1)
    {
        xx = targetEnd[0];
        yy = targetEnd[1];
    }
    else
    {
        xx = targetStart[0] + param * C;
        yy = targetStart[1] + param * D;
    }

    auto dx = point[0] - xx;
    auto dy = point[1] - yy;
    return sqrt(dx * dx + dy * dy);
}
// pochodna przyjmuje funkcje z ktorej chce liczyc pochodna, punkt dla danego punktu, d to mala roznica ktora dodawana jest do punktu
point2d derivative(function<double(point2d)> f, point2d x, double d = 1.52588e-05)
{
    // ze wzoru na pochodna
    point2d dx = {d, 0.0};
    point2d dy = {0.0, d};
    return {
            (f(x + dx * 0.5) - f(x - dx * 0.5)) / d,
            (f(x + dy * 0.5) - f(x - dy * 0.5)) / d};
}

int main(int argc, char **argv)
{
    point2d destination = {0.0, 0.0};      // cel
    point2d currentPosition = {10.0, 1.0}; // start
    double velocity = 0.1;                 // bedzie wykonywal ruch o 0.1
    double acceleration = 0.1;
    string option;
    cout << "Rectangle or segments" << endl;
    cin >> option;
    if(option == "rectangle"){
         //przeszkoda ktora musi omijać
         //jest to prostokat
         vector<pair<point2d, double>> obstacles = {};
         for (double x = 4.6; x < 5.7; x += 0.01)
         {
             for (double y = -3.6; y < 3.7; y += 0.01)
             {
//                 obstacles.push_back({{x, y}, (argc > 1) ? stod(argv[1]) : 1.0});
                 obstacles.push_back({{x, y}, (argc > 1) ? stod(argv[1]) : 0.001});
             }
         }
        //Wersja dla prostokąta
         //definicja pola potencjalow ktora dziala na wspolrzednych x,y a zwaraca liczbe, czyli wartosc pola
         //definicja pola po ktorym sie poruszamy.Zwraca watrosc pola potencjalow dla danego punktu
         auto field = [&obstacles, &destination](point2d p) -> double
         {
             double obstaclefield = 0;
             for (const auto &obstacle : obstacles)
             {
                 double distanceToObstacle = length(p - obstacle.first);
                 obstaclefield += obstacle.second / (distanceToObstacle * distanceToObstacle);
             }
             return length(destination - p) + obstaclefield;
         };

        //    currentPosition
        point2d currentVelocity = {0.0, 0.0};
        // wyznaczanie ścieżki
        ofstream outdata("result.txt");
        if (!outdata)
        {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
        for (int i = 0; i < 1000; i++)
        {
            // metoda gradientu
            //std::cout << i << "\n";
            point2d dp = derivative(field, currentPosition); //mamy dwie funkcje field, ttrzeba je trzymać w osobnych lambdach
            dp = dp * (1.0 / length(dp));
            dp = dp * acceleration;
            //std::clog << i << "dp: " << dp << std::endl;
            // kolejne pozycje po prouszaniu sie po polu
            currentVelocity = currentVelocity - dp;
            if (length(currentVelocity) > velocity)
                currentVelocity = (currentVelocity * (1.0 / (length(currentVelocity)))) * velocity;
            currentPosition = currentPosition + currentVelocity;
            //string p1 = to_string(currentPosition[0]);
            //string p2 = to_string(currentPosition[1]);
            outdata << currentPosition << "\n";
        }
        outdata.close();
    }else if(option == "segments"){
        //Wektor odcinków
        vector<pair<point2d, point2d>> segments{{{1.0, 0.0}, {1.0, 2.0}}, {{8.0, 0.0},{8.0, 2.0}}};



        //Wersja dla odcinków
        auto field = [&](point2d p) -> double
        {

            double obstaclefield = 0;

            for(const auto& segment : segments)
            {
                //cout << segment.first << " " << segment.second << endl;
                auto dist = pDistance(p, segment.first, segment.second);
                double distanceToObstacle = dist;
                obstaclefield += 0.01 / (distanceToObstacle * distanceToObstacle);
                //obstaclefield += 0.1 / (distanceToObstacle * distanceToObstacle);
            }
            return length(destination - p) + obstaclefield;
        };

        //    currentPosition
        point2d currentVelocity = {0.0, 0.0};
        // wyznaczanie ścieżki
        ofstream outdata("result.txt");
        if (!outdata)
        {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
        for (int i = 0; i < 1000; i++)
        {
            // metoda gradientu
            //std::cout << i << "\n";
            point2d dp = derivative(field, currentPosition); //mamy dwie funkcje field, ttrzeba je trzymać w osobnych lambdach
            dp = dp * (1.0 / length(dp));
            dp = dp * acceleration;
            //std::clog << i << "dp: " << dp << std::endl;
            // kolejne pozycje po prouszaniu sie po polu
            currentVelocity = currentVelocity - dp;
            if (length(currentVelocity) > velocity)
                currentVelocity = (currentVelocity * (1.0 / (length(currentVelocity)))) * velocity;
            currentPosition = currentPosition + currentVelocity;
            //string p1 = to_string(currentPosition[0]);
            //string p2 = to_string(currentPosition[1]);
            outdata << currentPosition << "\n";
        }
        outdata.close();
    }



    return 0;
}