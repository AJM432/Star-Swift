#include <cmath>
double distance(double x0, double y0, double x1, double y1) {
  return sqrt(pow(x1-x0, 2) + pow(y1-y0, 2));
}

double convert_ranges(double oldValue, double oldMin, double oldMax, double newMin, double newMax) {
  double oldRange = (oldMax - oldMin);
  double newValue;
  if (oldRange == 0)
      newValue = newMin;
  else
  {
      double newRange = (newMax - newMin);
      newValue = (((oldValue - oldMin) * newRange) / oldRange) + newMin;
  }
  return newValue;
}