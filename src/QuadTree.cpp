#include "QuadTree.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

double distance(double x0, double y0, double x1, double y1) {
  return sqrt(pow(x1-x0, 2) + pow(y1-y0, 2));
}



QuadTree::QuadTree(double x0, double y0, double x1, double y1, QuadTree *upper) {
  this->x0 = x0;
  this->x1 = x1;
  this->y0 = y0;
  this->y1 = y1;

  this->upper = upper;
  if (this->upper == nullptr) {
    this->SCREEN_WIDTH = x1;
    this->SCREEN_HEIGHT = y1;
  }
  else {
    this->SCREEN_WIDTH = this->upper->SCREEN_WIDTH;
    this->SCREEN_HEIGHT = this->upper->SCREEN_HEIGHT;
  }

  // tunable
  // ========
  softening_factor = 1e2;
  theta_threshold = 0.5;
  point_mass = 100;
  max_speed = 10000.0;
  // ========

  max_size = 1;
  center_of_mass_x = 0;
  center_of_mass_y = 0;
  total_mass = 0;
  num_stars = 0;
  distance_x_sum = 0;
  distance_y_sum = 0;

  top_left = nullptr;
  top_right = nullptr;
  bottom_left = nullptr;
  bottom_right = nullptr;

  star = nullptr;
  split = false;

}

QuadTree::~QuadTree() {
    delete top_left;
    delete top_right;
    delete bottom_left;
    delete bottom_right;
}

void QuadTree::update_star_color(Point *p) {
  float v = (p->vx*p->vy)/(max_speed*max_speed);
  float r = 0.f, g = 0.f, b = 0.f;
  if (v < 0) {
      p->b = 1.f + v;
      p->g = -v;
  }
  else {
      p->r = v;
      p->g = 1.f - v;
  }
  // p->r = p->r*255;
  p->r = 255;
  p->g = p->g*255;
  // p->g = 255;
  p->b = 255;
  // p->b = p->b*255;
}

void QuadTree::calculate_motion(Point *p, double other_x, double other_y, double mass, double dt) {


    double dx = (other_x - p->x);
    double dy = (other_y - p->y);
    if (dx == 0 && dy == 0) {
      return;
    }

    // TODO: scale point mass for realism
    double radius_squared = dx*dx + dy*dy;
    double a_mag = mass/(radius_squared + softening_factor*softening_factor);
    double angle = atan2(dy, dx);
    p->ax += a_mag*cos(angle);
    p->ay += a_mag*sin(angle);

    p->vx += p->ax*dt;
    p->vy += p->ay*dt;

    if (p->vx !=0 ) {
      p->vx = std::min(abs(p->vx), max_speed)*(p->vx/abs(p->vx));
    }
    if (p->vy !=0 ) {
      p->vy = std::min(abs(p->vy), max_speed)*(p->vy/abs(p->vy));
    }

    p->x += p->vx*dt + (1.0/2.0)*p->ax*dt*dt;
    p->y += p->vy*dt + (1.0/2.0)*p->ay*dt*dt;

    if (p->x < 10 || p->x > (SCREEN_WIDTH - 10)) {
      p->vx = -p->vx;
    }
    if (p->y < 10 || p->y > (SCREEN_HEIGHT - 10)) {
        p->vy = -p->vy;
    }
    update_star_color(p);

}

void QuadTree::update_point_gravity(Point *p, double dt) {
  if (star != nullptr && p != star) {
    calculate_motion(p, star->x, star->y, point_mass, dt);
  }
  else{
  double s = x1-x0;
  double d = distance(p->x, p->y, center_of_mass_x, center_of_mass_y);
  if (d<=0 || std::isnan(d)) {
    return;
  }
  if (((double)s/d > theta_threshold)) {
    calculate_motion(p, center_of_mass_x, center_of_mass_y, point_mass*num_stars, dt);
  }
  else if (split){
    top_left->update_point_gravity(p, dt);
    top_right->update_point_gravity(p, dt);
    bottom_left->update_point_gravity(p, dt);
    bottom_right->update_point_gravity(p, dt);
  }

  }

}

void QuadTree::update_galaxy(QuadTree *root, double dt) {
  if (star != nullptr) { //only check leaf nodes
      star->ax = 0;
      star->ay = 0;
      root->update_point_gravity(star, dt);
  }
  else if (split){
    top_left->update_galaxy(root, dt);
    top_right->update_galaxy(root, dt);
    bottom_left->update_galaxy(root, dt);
    bottom_right->update_galaxy(root, dt);
  }

}

void QuadTree::print() {
  if (star != nullptr) { //only check leaf nodes
    std::cout << star->x << " " << star->y << std::endl;
  }
  else if (split){ // cause tree can be newly split and empty
    top_left->print();
    top_right->print();
    bottom_left->print();
    bottom_right->print();
  }

}

bool QuadTree::insert(Point *p) {
  // verify point before inserting
  if (p->x < x0 || p->x > x1 || p->y < y0 || p->y > y1) {
    return false;
  }

  num_stars++;
  distance_x_sum += p->x;
  distance_y_sum += p->y;
  center_of_mass_x = distance_x_sum/(num_stars);
  center_of_mass_y = distance_y_sum/(num_stars);

  double x_mid = (double)(x1+x0)/2;
  double y_mid = (double)(y1+y0)/2;

  if (!split && star == nullptr) {
    star = p;
    return true;
  }

    if (split) { // case when array if full but have not split
      // recursive insert!
      // search and insert at leaf node
      if (p->x <= x_mid && p->y <= y_mid) {
        return top_left->insert(p);
      }
      else if (p->x <= x1 && p->y <= y_mid) {
        return top_right->insert(p);
      }
      else if (p->x <= x_mid && p->y <= y1) {
        return bottom_left->insert(p);
      }
      else if (p->x <= x1 && p->y <= y1) {
        return bottom_right->insert(p);
      }
    }
    else {
      //split
      top_left = new QuadTree(x0, y0, x_mid, y_mid, this);
      top_right = new QuadTree(x_mid, y0, x1, y_mid, this);
      bottom_left = new QuadTree(x0, y_mid, x_mid, y1, this);
      bottom_right = new QuadTree(x_mid, y_mid, x1, y1, this);

      // copies existing points to child quadrants
      split = true;

      num_stars--;
      distance_x_sum -= star->x;
      distance_y_sum -= star->y;

      num_stars--;
      distance_x_sum -= p->x;
      distance_y_sum -= p->y;

      center_of_mass_x = distance_x_sum/(num_stars);
      center_of_mass_y = distance_y_sum/(num_stars);

      insert(star);
      star = nullptr;
      return insert(p);


    }
  return false;
}
