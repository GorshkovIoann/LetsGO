#include <cmath>

#include "vec2d.h"

Vec2d& Vec2d::operator+= (const Vec2d& rhs)
{
  x += rhs.x;
  y += rhs.y;
  return *this;
}

Vec2d& Vec2d::operator-= (const Vec2d& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

Vec2d& Vec2d::operator*= (double rhs)
{
  x *= rhs;
  y *= rhs;
  return *this;
}

Vec2d operator+ (const Vec2d& lhs, const Vec2d& rhs)
{
  return Vec2d{lhs} += rhs;
}

Vec2d operator- (const Vec2d& lhs, const Vec2d& rhs)
{
  return Vec2d{lhs} -= rhs;
}

Vec2d operator* (const Vec2d& lhs, double rhs) { return Vec2d{lhs} *= rhs; }

Vec2d operator* (double lhs, const Vec2d& rhs) { return rhs * lhs; }

double length (const Vec2d& v) { return std::sqrt(v.x * v.x + v.y * v.y); }

Vec2d rotated (const Vec2d& v, double rotation)
{
  double sin_a = sin(rotation);
  double cos_a = cos(rotation);
  double new_x = cos_a * v.x - sin_a * v.y;
  double new_y = sin_a * v.x - cos_a * v.y;

  // Vec2d v{length(v) * cos_a, length(v) * sin_a};
  return Vec2d(new_x, new_y);
}
