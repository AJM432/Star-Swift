class Point{
public:
  Point();
  Point(double x, double y, double vx, double vy, double ax, double ay);
  void update_star_color(double root_center_mass_x, double root_center_mass_y, double max_distance, double max_speed, double galaxy_r, double galaxy_g, double galaxy_b, int color_mode);
  double x;
  double y;
  double vx;
  double vy;
  double ax;
  double ay;
  float r;
  float g;
  float b;
};
