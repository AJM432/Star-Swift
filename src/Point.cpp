#include "Point.hpp"
#include "helper.h"

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

void Point::update_star_color(double root_center_mass_x, double root_center_mass_y, double max_distance, double galaxy_r, double galaxy_g, double galaxy_b) {
  double residual_dist_from_center_mass = max_distance - distance(x, y, root_center_mass_x, root_center_mass_y);
  double residual_r = 1-galaxy_r;
  double residual_g = 1-galaxy_g;
  double residual_b = 1-galaxy_b;

  r = (galaxy_r + convert_ranges(residual_dist_from_center_mass, 0, max_distance, 0, residual_r))*255;
  g = (galaxy_g + convert_ranges(residual_dist_from_center_mass, 0, max_distance, 0, residual_g))*255;
  b = (galaxy_b + convert_ranges(residual_dist_from_center_mass, 0, max_distance, 0, residual_b))*255;
}