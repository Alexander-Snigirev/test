#ifndef DOUBLEDOUBLE_H
#define DOUBLEDOUBLE_H

#include<iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <quadmath.h>



namespace doubledouble {

class DoubleDouble
{
public:

    double upper{0.0};
    double lower{0.0};

    constexpr
    DoubleDouble() {}

    constexpr
    DoubleDouble(double x, double y)
    {
        if (std::isnan(x) || std::isnan(y)) {
            upper = NAN;
            lower = NAN;
            return;
        }
        bool xinf = std::isinf(x);
        bool yinf = std::isinf(y);
        if (xinf && yinf) {
            if (x != y) {
                upper = NAN;
                lower = NAN;
            }
            else {
                upper = x;
                lower = 0.0;
            }
        }
        else if (xinf) {
            upper = x;
            lower = 0.0;
        }
        else if (yinf) {
            upper = y;
            lower = 0.0;
        }
        else {
            double r = x + y;
            double t = r - x;
            double e = (x - (r - t)) + (y - t);
            upper = r;
            lower = e;
        }
    }

    constexpr
    DoubleDouble(double upper) : upper(upper)
    {
        if (std::isnan(upper)) {
            lower = NAN;
        }
    }
    DoubleDouble operator-() const;
    DoubleDouble operator+(double x) const;
    DoubleDouble operator+(const DoubleDouble& x) const;
    DoubleDouble operator-(double x) const;
    DoubleDouble operator-(const DoubleDouble& x) const;
    DoubleDouble operator*(double x) const;
    DoubleDouble operator*(const DoubleDouble& x) const;
    DoubleDouble operator/(double x) const;
    DoubleDouble operator/(const DoubleDouble& x) const;

    DoubleDouble& operator+=(double x);
    DoubleDouble& operator+=(const DoubleDouble& x);
    DoubleDouble& operator-=(double x);
    DoubleDouble& operator-=(const DoubleDouble& x);
    DoubleDouble& operator*=(double x);
    DoubleDouble& operator*=(const DoubleDouble& x);
    DoubleDouble& operator/=(double x);
    DoubleDouble& operator/=(const DoubleDouble& x);

    bool operator==(const DoubleDouble& x) const;
    bool operator==(double x) const;
    bool operator!=(const DoubleDouble& x) const;
    bool operator!=(double x) const;
    bool operator<(double x) const;
    bool operator<(const DoubleDouble& x) const;
    bool operator<=(double x) const;
    bool operator<=(const DoubleDouble& x) const;
    bool operator>(double x) const;
    bool operator>(const DoubleDouble& x) const;
    bool operator>=(double x) const;
    bool operator>=(const DoubleDouble& x) const;
    DoubleDouble sqrt() const;
    DoubleDouble custom_sqrt() const;

};

inline DoubleDouble two_sum_quick(double x, double y)
{
    double r = x + y;
    double e = y - (r - x);
    //printf("upper = %.20f\nlower = %.34f\n",r,e);

    return DoubleDouble(r, e);
}


inline DoubleDouble two_sum(double x, double y)
{
    double r = x + y;
    double t = r - x;
    double e = (x - (r - t)) + (y - t);
    //printf("upper = %.20f\nlower = %.34f\n",r,e);
    return DoubleDouble(r, e);
}


inline DoubleDouble two_difference(double x, double y)
{
    double r = x - y;
    double t = r - x;
    double e = (x - (r - t)) - (y + t);
    return DoubleDouble(r, e);
}


inline DoubleDouble two_product(double x, double y)
{
    double r = x*y;
    double e = fma(x, y, -r);
    return DoubleDouble(r, e);
}


inline DoubleDouble DoubleDouble::operator-() const
{
    return DoubleDouble(-upper, -lower);
}

inline DoubleDouble DoubleDouble::operator+(double x) const
{
    DoubleDouble re = two_sum(upper, x);
    re.lower += lower;
    return two_sum_quick(re.upper, re.lower);
}

inline DoubleDouble operator+(double x, const DoubleDouble& y)
{
    return y + x;
}

inline DoubleDouble DoubleDouble::operator+(const DoubleDouble& x) const
{
    DoubleDouble re;
    if(fabs(upper)<fabs(x.upper)){
    re = two_sum(upper, x.upper);
    re.lower = (re.lower + lower) + x.lower;
    }
    else{
    re = two_sum(x.upper, upper);
    re.lower = (re.lower + x.lower) + lower;
    }
    //return re;
    
    return two_sum(re.upper, re.lower);
}
/*inline DoubleDouble DoubleDouble::operator+(const DoubleDouble& x) const
{
    DoubleDouble re = two_sum(upper, x.upper);
    re.lower += lower + x.lower;
    return two_sum_quick(re.upper, re.lower);
}*/

inline DoubleDouble DoubleDouble::operator-(double x) const
{
    DoubleDouble re = two_difference(upper, x);
    re.lower += lower;
    return two_sum_quick(re.upper, re.lower);
}

inline DoubleDouble operator-(double x, const DoubleDouble& y)
{
    return -y + x;
}

inline DoubleDouble DoubleDouble::operator-(const DoubleDouble& x) const
{
    /*DoubleDouble re = two_difference(upper, x.upper);
    re.lower += lower - x.lower;
    return two_sum_quick(re.upper, re.lower);*/
    DoubleDouble re = *this + (-x);
    return re;
}

inline DoubleDouble DoubleDouble::operator*(double x) const
{
    DoubleDouble re = two_product(upper, x);
    re.lower += lower * x;
    return re;
    return two_sum_quick(re.upper, re.lower);
}

inline DoubleDouble operator*(double x, const DoubleDouble& y)
{
    return y * x;
}

inline DoubleDouble DoubleDouble::operator*(const DoubleDouble& x) const
{
    DoubleDouble re = two_product(upper, x.upper); 
    re.lower = re.lower + (upper*x.lower + lower*x.upper);
    return two_sum(re.upper, re.lower);
    return re;
}

inline DoubleDouble DoubleDouble::operator/(double x) const
{
    double r = upper/x;
    DoubleDouble sf = two_product(r, x);
    double e = (upper - sf.upper - sf.lower + lower)/x;
    DoubleDouble res(r,e);
    //return res;
    return two_sum(r, e);
}

inline DoubleDouble operator/(double x, const DoubleDouble& y)
{
    return DoubleDouble(x, 0.0) / y;
}

inline DoubleDouble DoubleDouble::operator/(const DoubleDouble& x) const
{
    double r = upper/x.upper;
    DoubleDouble sf = two_product(r, x.upper);
    double e = (upper - sf.upper - sf.lower + lower - r*x.lower)/x.upper;
    return two_sum(r, e);
}

inline DoubleDouble& DoubleDouble::operator+=(double x)
{
    DoubleDouble re = two_sum(upper, x);
    re.lower += lower;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator+=(const DoubleDouble& x)
{
    DoubleDouble re = two_sum(upper, x.upper);
    re.lower += lower + x.lower;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator-=(double x)
{
    DoubleDouble re = two_difference(upper, x);
    re.lower += lower;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator-=(const DoubleDouble& x)
{
    DoubleDouble re = two_difference(upper, x.upper);
    re.lower += lower - x.lower;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator*=(double x)
{
    DoubleDouble re = two_product(upper, x);
    re.lower += lower * x;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator*=(const DoubleDouble& x)
{
    DoubleDouble re = two_product(upper, x.upper);
    re.lower += upper*x.lower + lower*x.upper;
    *this = two_sum_quick(re.upper, re.lower);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator/=(double x)
{
    double r = upper/x;
    DoubleDouble sf = two_product(r, x);
    double e = (upper - sf.upper - sf.lower + lower)/x;
    *this = two_sum_quick(r, e);
    return *this;
}

inline DoubleDouble& DoubleDouble::operator/=(const DoubleDouble& x)
{
    double r = upper/x.upper;
    DoubleDouble sf = two_product(r, x.upper);
    double e = (upper - sf.upper - sf.lower + lower - r*x.lower)/x.upper;
    *this = two_sum_quick(r, e);
    return *this;
}

inline bool DoubleDouble::operator==(const DoubleDouble& x) const
{
    return (upper == x.upper) && (lower == x.lower);
}

inline bool DoubleDouble::operator==(double x) const
{
    return (upper == x) && (lower == 0.0);
}

inline bool operator==(double x, const DoubleDouble& y)
{
    return y == x;
}

inline bool DoubleDouble::operator!=(const DoubleDouble& x) const
{
    return (upper != x.upper) || (lower != x.lower);
}

inline bool DoubleDouble::operator!=(double x) const
{
    return (upper != x) || (lower != 0.0);
}

inline bool operator!=(double x, const DoubleDouble& y)
{
    return y != x;
}

inline bool DoubleDouble::operator<(const DoubleDouble& x) const
{
    return (upper < x.upper) || ((upper == x.upper) && (lower < x.lower));
}

inline bool DoubleDouble::operator<(double x) const
{

    return (upper < x) || ((upper == x) && (lower < 0.0));
}

inline bool operator<(double x, const DoubleDouble& y)
{
    return y >= x;
}

inline bool DoubleDouble::operator<=(const DoubleDouble& x) const
{
    return (upper < x.upper) || ((upper == x.upper) && (lower <= x.lower));
}

inline bool DoubleDouble::operator<=(double x) const
{
    return (upper < x) || ((upper == x) && (lower <= 0.0));
}

inline bool operator<=(double x, const DoubleDouble& y)
{
    return y >= x;
}

inline bool DoubleDouble::operator>(const DoubleDouble& x) const
{
    return (upper > x.upper) || ((upper == x.upper) && (lower > x.lower));
}

inline bool DoubleDouble::operator>(double x) const
{
    return (upper > x) || ((upper == x) && (lower > 0.0));
}

inline bool operator>(double x, const DoubleDouble& y)
{
    return y <= x;
}

inline bool DoubleDouble::operator>=(const DoubleDouble& x) const
{
    return (upper > x.upper) || ((upper == x.upper) && (lower >= x.lower));
}

inline bool DoubleDouble::operator>=(double x) const
{
    return (upper > x) || ((upper == x) && (lower >= 0.0));
}

inline bool operator>=(double x, const DoubleDouble& y)
{
    return y <= x;
}

inline DoubleDouble DoubleDouble::custom_sqrt() const {
    if (upper == 0.0 && lower == 0.0){
        return DoubleDouble(0.0, 0.0);
    }
    if (upper < 0.0){
        return DoubleDouble(NAN, NAN); 
    }    
    DoubleDouble x(1.0, 0.0); 
    if (upper > 1.0){
        x = DoubleDouble(upper / 2.0, 0.0);
    } else if (upper < 1.0){
        x = DoubleDouble(upper * 2.0, 0.0);
    }

    const DoubleDouble tolerance(1e-20, 1e-34); 
    DoubleDouble two(2.0, 0.0);
    int max_iterations = 200; 

    for (int i = 0; i < max_iterations; ++i) {
        DoubleDouble x2 = x * x;
        DoubleDouble fx = x2 - *this;
        DoubleDouble dfx = x * two;
        DoubleDouble delta = fx / dfx;
        x = x - delta;
        if(delta.upper < 0.0) delta = -delta; 
        printf("%d: upper = %.20f\n%d: lower = %.34f\n",i, x.upper,i, x.lower);

        if(delta.upper < tolerance.upper && std::abs(delta.lower) < tolerance.lower) {
            break;
        }
    }

    return two_sum_quick(x.upper, x.lower);
}


inline DoubleDouble DoubleDouble::sqrt() const
{
    if (upper == 0 && lower == 0) {
        return DoubleDouble(0.0, 0.0);
    }
    double r = std::sqrt(upper);
    DoubleDouble sf = two_product(r, r);
    double e = (upper - sf.upper - sf.lower + lower) * 0.5 / r;
    return two_sum_quick(r, e);
}

} 

#endif
void
print_doubledouble(const char *prefix, const doubledouble::DoubleDouble& x)
{
    printf("%s = (%.34f, %.34f)\n", prefix, x.upper, x.lower);
}

int main()
{
    //x = 2.123453647583675
    //y = 0.000000000000123467654
    //r = 2.123453647583798
    //2.123453647583675
    //t = 0.000000000000123
    //0.467654

    doubledouble::DoubleDouble num1(0.5);
    //doubledouble::DoubleDouble num2(-312.0865, -2e-16);
    //doubledouble::DoubleDouble num3 = num1 * num2;
    //print_doubledouble("x", num1);
    //print_doubledouble("y", num2);

    doubledouble::DoubleDouble num2 = num1.custom_sqrt();
    print_doubledouble("sqrt", num2);
    // 27.73084924772409456+0.00000000000000024661310508970917
    //111.108055513540506353820092+0.0000000000000048911843466983030844
    //993.2668322258626858456409536302089691-0.0000000000000266418017282923335001
    //111.1080555135405063538200920447707176+0.0000000000000048911843466983030844
    //27.7308492477240946527672349475324154+0.0000000000000002466131050897091743
    
}