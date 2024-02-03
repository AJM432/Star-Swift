#include <iostream>
#include "Point.hpp"

class QuadTree{

public:
  double center_of_mass_x;
  double center_of_mass_y;
  double total_mass;
  double num_stars;
  double distance_x_sum;
  double distance_y_sum;

  float theta_threshold;
  double point_mass;
  int softening_factor;
  double max_speed;

  bool split;
  double x0;
  double x1;
  double y0;
  double y1;
  Point first_point;

  double SCREEN_WIDTH;
  double SCREEN_HEIGHT;

  Point *star;

  QuadTree* upper;
  QuadTree* top_left;
  QuadTree* top_right;
  QuadTree* bottom_left;
  QuadTree* bottom_right;

public:
  QuadTree(double x0, double y0, double x1, double y1, QuadTree *upper, float gravity_strength, float max_speed, float theta, int soft_power);
  ~QuadTree();
  bool insert(Point *p);
  // void update_star_color(Point *p);
  void update_galaxy(QuadTree *root, double dt);
  void update_point_gravity(Point *p, double dt);
  void calculate_gravity(Point *p, double other_x, double other_y, double mass, double dt);
  void calculate_motion(Point *p, double dt);
  void print();
};
