#ifndef VEC2D_H
#define VEC2D_H

#include <iostream>

class vec2d
{
public:
    vec2d();
    vec2d(double, double);
//     double get_x() const;
//     double get_y() const;
    double   operator[](int) const;
    double & operator[](int);
    vec2d & operator+=(const vec2d &);
    vec2d   operator+ (const vec2d &) const;
    vec2d & operator*=(double);
    vec2d   operator*(double) const;
   
    bool operator==(const vec2d &) const;
    bool operator!=(const vec2d &) const;
private:
    double x_;
    double y_;
};

std::ostream &
operator<<(std::ostream &, const vec2d &);
vec2d operator*(double, const vec2d &);

#endif
