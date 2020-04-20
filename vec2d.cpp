#include "vec2d.h"

vec2d::vec2d()
    : x_(0), y_(0)
{}

vec2d::vec2d(double x, double y)
    : x_(x), y_(y)
{}

// double vec2d::get_x() const
// {
//     return x_;
// }

// double vec2d::get_y() const
// {
//     return y_;
// }

double vec2d::operator[](int i) const
{
    if (i == 0) return x_;
    else return y_;
}

double & vec2d::operator[](int i)
{
    if (i == 0) return x_;
    else return y_;
}

vec2d & vec2d::operator+=(const vec2d & v)
{
    x_ += v.x_; y_ += v.y_; return (*this);
}

vec2d vec2d::operator+(const vec2d & v) const
{
    return (vec2d(*this) += v);
}

vec2d & vec2d::operator*=(double c)
{
    x_ *= c; y_ *= c; return (*this);
}
vec2d vec2d::operator*(double c) const
{
    return (vec2d(*this) *= c);
}
vec2d operator*(double c, const vec2d & v)
{
    return v * c;
}

bool vec2d::operator==(const vec2d & v) const
{
    return (x_ == v.x_ && y_ == v.y_);
}
bool vec2d::operator!=(const vec2d & v) const
{
    return !((*this) == v);
}

std::ostream &
operator<<(std::ostream & cout, const vec2d & v)
{
    cout << '<' << v[0]
         << ", " << v[1] << '>';
    return cout;
}
