#include <iostream>
using namespace std;
#include <cmath>
#include <tuple>

#define PI 3.141592653589793238462643383279502884197



class Vec2f{
public:
    float x;
    float y;

    Vec2f(float a = 0, float b = 0){x=a; y=b;}

    Vec2f operator + (Vec2f const &other){ Vec2f v3(x+other.x,y+other.y); return v3;}
    Vec2f operator - (Vec2f const &other){ Vec2f v3(x-other.x,y-other.y); return v3;}


    void vprint() {
        cout << x << "," << y << '\n';

    }

};


std::tuple<float, float> quadratic(float a,float b,float c){
    bool solution_is_real = true;
    if (pow(b,2)-4*a*c<0) {bool solution_is_real = false; return std::tuple<float,float>(-1,-1);}
    float x1 = (-b + sqrt(pow(b,2)-4*a*c))/(2*a);
    float x2 = (-b - sqrt(pow(b,2)-4*a*c))/(2*a);
    return std::tuple<float, float>(x1,x2);

}

string roundy(float var)
{
    // we use array of chars to store number
    // as a string.
    char str[40]; 
  
    // Print in string the value of var 
    // with two decimal point
    sprintf(str, "%.2f", var);

  
    return str; 
}