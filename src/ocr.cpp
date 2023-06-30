// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.


#include "ocr.h"

 long double operator ""_deg(long double deg)
{
    return deg;
};

constexpr long double operator ""_rad(long double rad)
{
    long double radians = (180 * rad) / std::numbers::pi_v<long double>;
    return radians;
};

cord2_t zCrossProd(const cv::Point &O, const cv::Point &A, const cv::Point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
};

std::vector<cv::Point> monotoneChain(std::vector<cv::Point> P) {
    size_t n = P.size(), k = 0;
    if (n <= 3) return P;
    std::vector<cv::Point> H(2 * n);


    struct
    {
        bool operator()(const cv::Point a, const cv::Point b) const {return a.x < b.x ; }
    } customLess;

// Sort points lexicographically
    std::sort(P.begin(), P.end(), customLess);

// Build lower hull
    for (size_t i = 0; i < n; ++i) {
        while (k >= 2 && zCrossProd(H[k - 2], H[k - 1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }

// Build upper hull
    for (size_t i = n - 1, t = k + 1; i > 0; --i) {
        while (k >= t && zCrossProd(H[k - 2], H[k - 1], P[i - 1]) <= 0) k--;
        H[k++] = P[i - 1];
    }

    H.resize(k - 1);
    return H;
};

area_t fannedArea(std::vector<cv::Point> points) {
    area_t currArea = 0;

    for (unsigned short i = 0; i < points.size() - 1; i++) {
        Eigen::Matrix<int, 3, 3> matrix{
                {points[0].x, points[i + 1].x, points[i + 2].x},
                {points[0].y, points[i + 1].y, points[i + 2].y},
                {1,           1,               1}
        };

        currArea += abs(float(matrix.determinant()));
    }

    return currArea;
}

cv::Rect boundingBox(std::vector<cv::Point> &points, float angle)
{
    float sinTheta = sin(angle);
    float cosTheta = cos(angle);
    std::vector<cv::Point> newPoints;

    for (auto&& point : points)
    {
        // Use the following rotation matrix: \begin{bmatrix}
        //\cos \theta & -\sin \theta \\
        //\sin \theta & \cos \theta
        //\end{bmatrix}
        newPoints.emplace_back(cv::Point(int(point.x * cosTheta - point.y * sinTheta),  int(point.x * sinTheta - point.y * cosTheta)));
    }

    return cv::boundingRect(newPoints);
}

int main() {
    cv::Mat srcImage = cv::imread("./mostRecent.jpg");
    cv::Mat srcHSV, hsvBlurred, mask;

    cv::cvtColor(srcImage, srcHSV, cv::COLOR_BGR2HSV);
    cv::blur(srcHSV, hsvBlurred, cv::Size(5, 5));
    cv::inRange(hsvBlurred, cv::Mat({20, 110, 120}), cv::Mat({30, 242, 255}), mask);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);

    auto blob = *std::max_element(contours.begin(), contours.end(),
                                 [](std::vector<cv::Point> a, std::vector<cv::Point> b) {
        return cv::contourArea(a) < cv::contourArea(b);
    });

    std::vector<cv::Point> outerPoints;
    cv::convexHull(blob, outerPoints);
//boundingBox(outerPoints, 10)
	cv::RotatedRect rRect = cv::minAreaRect(outerPoints);

	auto M = cv::getRotationMatrix2D(rRect.center, rRect.angle, 1);

	cv::Mat warpDst, cropped, eroded;
	auto newSize = rRect.size;
	newSize.height /= 1.1;
  	auto rRectCenter = rRect.center;
  	auto newSrcImgSize = srcImage.size();
  	newSrcImgSize.width -= int(newSize.width * 0.35);
  	rRectCenter.x += newSize.width * 0.15;
  	newSize.width *= 0.65;

	cv::warpAffine(srcImage, warpDst, M, newSrcImgSize);
	cv::getRectSubPix(warpDst, newSize, rRectCenter, newIdk);

	cv::Mat yellowMask, newIdk(cropped), inverseMask;
	cv::inRange(newIdk, cv::Mat({10, 10, 10}), cv::Mat({255, 255, 255}), yellowMask);
	newIdk.setTo(cv::Scalar({255,255,255}), yellowMask);
	cv::inRange(newIdk, cv::Scalar({0,0,0}), cv::Scalar({250,250,250}), inverseMask);
	cv::morphologyEx(inverseMask, eroded, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3)),
					 cv::Point(-1, -1), 1);




//	cv::Point2f rectPoints[4];
//	rRect.points(rectPoints);
//	for (unsigned int i = 0; i < 4; i++)
//	{
//	  cv::line(srcImage, rectPoints[i], rectPoints[(i+1) % 4], cv::Scalar({255, 200, 128}), 3);
//	}

    cv::imshow("मैं नीना से प्यार करता हूँ",  inverseMask);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}