// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.

//
// Created by Clayton Gearhart on 7/3/23.
//

#ifndef DROIDCONTROL_SRC_PROCESSIMG_HXX_
#define DROIDCONTROL_SRC_PROCESSIMG_HXX_

#include <algorithm>
#include <vector>
#include <Eigen/Eigen>
#include <Eigen/LU>
#include <opencv2/imgcodecs.hpp>
#include <numbers>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


typedef unsigned int CordT;
typedef unsigned int Cord2T;
typedef float AreaT;

struct point
{
  CordT x;
  CordT y;

  bool operator <(const point &p) const
  {
	return x < p.x || (x == p.x && y < p.x);
  }
};

long double operator ""_deg(long double);
constexpr long double operator ""_rad(long double);


// 3D cross product of OA and OB vectors, (i.e z-component of their "2D" cross product, but remember that it is not defined in "2D").
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
Cord2T zCrossProd(const cv::Point &O, const cv::Point &A, const cv::Point &B);

std::vector<cv::Point> monotoneChain(std::vector<cv::Point> P);

cv::Rect boundingBox(std::vector<cv::Point> &Points, float Angle);
void processImg();

#endif //DROIDCONTROL_SRC_PROCESSIMG_HXX_
