// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.

//
// Created by Clayton Gearhart on 7/3/23.
//

#include "processImg.hxx"

long double operator ""_deg(long double deg) {
  return deg;
};

constexpr long double operator ""_rad(long double rad) {
  long double Radians = (180 * rad) / std::numbers::pi_v<long double>;
  return Radians;
};

Cord2T zCrossProd(const cv::Point &O, const cv::Point &A, const cv::Point &B) {
  return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
};

std::vector<cv::Point> monotoneChain(std::vector<cv::Point> P) {
  size_t N = P.size(), K = 0;
  if (N <= 3) return P;
  std::vector<cv::Point> H(2 * N);

  struct {
	bool operator()(const cv::Point a, const cv::Point b) const { return a.x < b.x; }
  } CustomLess;

// Sort points lexicographically
  std::sort(P.begin(), P.end(), CustomLess);

// Build lower hull
  for (size_t i = 0; i < N; ++i) {
	while (K >= 2 && zCrossProd(H[K - 2], H[K - 1], P[i]) <= 0) K--;
	H[K++] = P[i];
  }

// Build upper hull
  for (size_t i = N - 1, t = K + 1; i > 0; --i) {
	while (K >= t && zCrossProd(H[K - 2], H[K - 1], P[i - 1]) <= 0) K--;
	H[K++] = P[i - 1];
  }

  H.resize(K - 1);
  return H;
};

AreaT fannedArea(std::vector<cv::Point> points) {
  AreaT CurrArea = 0;

  for (unsigned short i = 0; i < points.size() - 1; i++) {
	Eigen::Matrix<int, 3, 3> Matrix{
		{points[0].x, points[i + 1].x, points[i + 2].x},
		{points[0].y, points[i + 1].y, points[i + 2].y},
		{1, 1, 1}
	};

    CurrArea += abs(float(Matrix.determinant()));
  }

  return CurrArea;
}

cv::Rect boundingBox(std::vector<cv::Point> &Points, float Angle) {
  float SinTheta = sin(Angle);
  float CosTheta = cos(Angle);
  std::vector<cv::Point> NewPoints;

  for (auto &&point : Points) {
	// Use the following rotation matrix: \begin{bmatrix}
	//\cos \theta & -\sin \theta \\
        //\sin \theta & \cos \theta
	//\end{bmatrix}
	NewPoints.emplace_back(cv::Point(int(point.x * CosTheta - point.y * SinTheta),
	                                 int(point.x * SinTheta - point.y * CosTheta)));
  }

  return cv::boundingRect(NewPoints);
}

void processImg()
{
  cv::Mat srcImage = cv::imread("./mostRecent.jpg");
  cv::Mat srcHSV, hsvBlurred, mask;

  cv::cvtColor(srcImage, srcHSV, cv::COLOR_BGR2HSV);
  cv::blur(srcHSV, hsvBlurred, cv::Size(5, 5));
  cv::inRange(hsvBlurred, cv::Mat({20, 110, 120}), cv::Mat({30, 242, 255}), mask);

  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);

  auto Blob = *std::max_element(contours.begin(), contours.end(),
                                [](std::vector<cv::Point> a, std::vector<cv::Point> b) {
								  return cv::contourArea(a) < cv::contourArea(b);
								});

  std::vector<cv::Point> OuterPoints;
  cv::convexHull(Blob, OuterPoints);
//boundingBox(outerPoints, 10)
  cv::RotatedRect RRect = cv::minAreaRect(OuterPoints);

  auto M = cv::getRotationMatrix2D(RRect.center, RRect.angle, 1);

  cv::Mat WarpDst, Cropped, Eroded;
  auto NewSize = RRect.size;
  NewSize.height /= 1.1;
  auto RRectCenter = RRect.center;
  auto NewSrcImgSize = srcImage.size();
  NewSrcImgSize.width -= int(NewSize.width * 0.35);
  RRectCenter.x += NewSize.width * 0.15;
  NewSize.width *= 0.65;

  cv::warpAffine(srcImage, WarpDst, M, NewSrcImgSize);
  cv::getRectSubPix(WarpDst, NewSize, RRectCenter, Cropped);

  ////  cv::morphologyEx(inverseMask, eroded, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,
////																												3)),
////				   cv::Point(-1, -1), 1);


  cv::Mat Acetylcholine, Androstadienone, Estratetraenol, DendriticAborization, Monozygotic;
  cv::inRange(Cropped, cv::Scalar({0, 0, 0}), cv::Scalar({64, 32, 32}), Acetylcholine);
  cv::inRange(Cropped, cv::Scalar({16, 16, 16}), cv::Scalar({100, 100, 100}), DendriticAborization);

  auto StructEle = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
  cv::morphologyEx(DendriticAborization, Estratetraenol, cv::MORPH_CLOSE, StructEle, cv::Point(-1, -1), 1);

  cv::blur(Estratetraenol, Androstadienone, cv::Size(1, 1));

  cv::subtract(Androstadienone, Acetylcholine, Monozygotic);

  cv::imwrite("test.jpg", Cropped);
}