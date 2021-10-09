#include <iostream>

using namespace std;

//
//auto obliczenia =[](auto f1,auto f2,int n){
//    return make_pair(2,"abc");
//};
//auto f1=[](int x){return x;};
//auto f2=[](int x){return x;};
//int main() {
//    obliczenia(f1(2),f2,22);
//    cout << "Hello, World!" << endl;
//    return 0;
//}
pair<int,float> obliczenia(float (*f1)(int),int (*f2)(int), int n){
    int max_x = f2(0);
    float max_y = f1(max_x);
    int i=1;
    while(i<=n){
       int x=f2(i);
       float y=f1(x);
       if(y >max_y){
           max_x=x;
           max_y=y;
       }
       i++;
    }
    return make_pair(max_x,max_y);
}

float f1Test(int x){
    return 20- abs(x);
}
int f2Test(int i){
    int a =(2*(i%2)-1)*i/2;
    return a;
}
int  main(){
//    float (*f1)(int) ;//tworze wskaznik na funkcje
//    f1=f1Test;
//    int (*f2)(int);
//    f2=f2Test;

    pair<int,float> ret = obliczenia(f1Test,f2Test,22);

    cout << ret.first  << " " << ret.second << endl;
}