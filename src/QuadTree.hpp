#include <iostream>
#include "Point.hpp"

class QuadTree{

private:
  int max_size; //size of points array
  double center_of_mass_x;
  double center_of_mass_y;
  double total_mass;
  double theta_threshold;
  double point_mass;
  double num_stars;
  double distance_x_sum;
  double distance_y_sum;
  double softening_factor;
  double max_speed;

  bool split;
  double x0;
  double x1;
  double y0;
  double y1;
  Point first_point;

  Point *star;

  QuadTree* top_left;
  QuadTree* top_right;
  QuadTree* bottom_left;
  QuadTree* bottom_right;

public:
  QuadTree(double x0, double y0, double x1, double y1);
  ~QuadTree();
  bool insert(Point *p);
  void update_star_color(Point *p);
  void update_galaxy(QuadTree *root, double dt);
  void update_point_gravity(Point *p, double dt);
  void calculate_motion(Point *p, double other_x, double other_y, double mass, double dt);
  void print();
};
