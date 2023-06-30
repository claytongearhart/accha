// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.

//

#pragma once

#include <opencv2/core/mat.hpp>
#include <cmath>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

typedef cv::Point_<double> kPoint;

class Segment {
 public:
  Segment(kPoint a, kPoint b) : pointA(a), pointB(b) {}

  [[nodiscard]] double length() const {
	return sqrt(std::pow(pointA.x - pointB.x, int(2)) + std::pow(pointA.y - pointB.y, int(2)));
  }

  double angle() const {
	return atan((pointA.y - pointB.y) / (pointA.x - pointB.x));
  }

  double angle(const Segment &seg) const {
	return seg.angle() - this->angle();
  }

 private:
  kPoint pointB, pointA;
  friend class Line;
};

class LinFunc
{
 public:
  double slope;
  double yInt;

  LinFunc(kPoint a, kPoint b)
  {
	slope = (a.y - b.y)/(a.x - b.x);
	yInt = a.y - a.x * slope;
  }

  [[nodiscard]] double f(double x) const
  {
	return slope * x + yInt;
  }
};

class Line {
 public:
  Line(const kPoint &p, const kPoint &q) : pointP(p), pointQ(q) {

  }

  explicit Line(const Segment &s) : pointP(s.pointA), pointQ(s.pointB), funcRep(s.pointA, s.pointB) {}

  Segment perpendicular(const kPoint &p) const {
	double pSlope = (pointQ.x - pointP.x)/(pointQ.y - pointP.y);

  }

 private:
  kPoint pointQ, pointP;
  LinFunc funcRep;
};

class evtype {
  Segment e;
  kPoint v;
  friend class Pargram;

 public:
  Segment Edge() { return e; }
  kPoint Vertex() { return v; }
  double angle(evtype &p) { return (e.angle(p.e)); }
  double width() const { return (Line(e).perpendicular(v).length()); }

};
class Pargram {
  kPoint a, b, c;

  kPoint fourth() const {
	kPoint d(a);
	return (d + vector(c.x - b.x, c.y - b.y()));
  }

  double
  angle() {
	return (fabs(Segment(a, b).angle(Segment(b, c))));
  }

 public:
  Pargram(evtype &z1, evtype &z2) {
	double z1dir = z1.e.direction();
	double z2dir = z2.e.direction();
	Line(z1.e).intersection(Line(z2.e), a);
	Line(z1.e).intersection(Line(z2.v, z2dir), b);
	Line(z2.v, z2dir).intersection(Line(z1.v, z1dir), c);
  }

  double
  area() {
	return (Segment(a, b).length() * Segment(b, c).length() * sin(angle()));
  }
};

