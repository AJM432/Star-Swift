#include "Point.hpp"

Point::Point() {
}

Point::Point(double x, double y, double vx, double vy, double ax, double ay){
  this->x = x;
  this->y = y;
  this->vx = vx;
  this->vy = vy;
  this->ax = ax;
  this->ay = ay;
  this->r = 255;
  this->g = 255;
  this->b = 255;
}
