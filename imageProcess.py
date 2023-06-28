"""

"""

__author__ = "Clayton Gearhart"
__copyright__ = "Copyright 2023 Clayton Gearhart"
__license__ = "GNU Lesser General Public License v3.0"
__version__ = "1.0"

import cv2 as cv
import numpy as np
import numbers
import numpy.typing as npt
from math import atan2
from collections import namedtuple
Point_2D = namedtuple('Point_2D', ['x', 'y'])

def convex_hull(points):
    """Computes the convex hull of a set of 2D points.

    Input: an iterable sequence of (x, y) pairs representing the points.
    Output: a list of vertices of the convex hull in counter-clockwise order,
      starting from the vertex with the lexicographically smallest coordinates.
    Implements Andrew's monotone chain algorithm. O(n log n) complexity.
    """

    # Sort the points lexicographically (tuples are compared lexicographically).
    # Remove duplicates to detect the case we have just one unique point.
    points = sorted(set(points))

    # Boring case: no points or a single point, possibly repeated multiple times.
    if len(points) <= 1:
        return points

    # 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
    # Returns a positive value, if OAB makes a counter-clockwise turn,
    # negative for clockwise turn, and zero if the points are collinear.
    def cross(o, a, b):
        return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])

    # Build lower hull
    lower = []
    for p in points:
        while len(lower) >= 2 and cross(lower[-2], lower[-1], p) <= 0:
            lower.pop()
        lower.append(p)

    # Build upper hull
    upper = []
    for p in reversed(points):
        while len(upper) >= 2 and cross(upper[-2], upper[-1], p) <= 0:
            upper.pop()
        upper.append(p)

    # Concatenation of the lower and upper hulls gives the convex hull.
    # Last point of each list is omitted because it
    # Last point of each list is omitted because it is repeated at the beginning of the other list.
    return lower[:-1] + upper[:-1]

def fanned_area(points: list[Point_2D]) -> numbers.Real:
    """
    Computes the area of a convex polygon using fan triangulation
    :param points: A list of named tuples ('x', 'y')
    :return: A scalar...
    """

    curr_area =  0
    for i in range(len(points) - 2):
        # Computed using the absolute val of the determinante
        # The high computational complexity of det doesn't matter because the matrices are only 3x3
        matrix = np.array([[points[0].x, points[i+1].x , points[i+2].x],
                           [points[0].y, points[i+1].y , points[i+2].y],
                           [1          , 1             ,1            ]])
        print([points[0], points[i+1], points[i+2]])
        curr_area += abs(np.linalg.det(matrix)) / 2

    return curr_area

def bounding_box(points: list[Point_2D], theta: float) -> list[Point_2D]:
    """
    Find the bounding box of the inputted points in a rotated cord system
    :param points: A list of named tuples ('x', 'y') representing points on a Cartesian plane.
    :param theta: The angle that the source image is to be rotated in degrees
    :return: The points that define the bounding box
    """



src_image = cv.imread('./mostRecent.jpg')
src_hsv = cv.cvtColor(src_image, cv.COLOR_BGR2HSV)
hsv_blurred = cv.blur(src_hsv, (5, 5), 1)
mask = cv.inRange(hsv_blurred, (20, 110, 120), (30, 242, 255))

contours, _ = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_NONE)
blob = np.reshape(max(contours, key=lambda el: cv.contourArea(el)), [-1, 2])
outer_points = convex_hull([Point_2D(x = dsp[0], y = dsp[1]) for dsp in blob.tolist()])
# print(fanned_area(outer_points))
pppppp = [[-2, -1], [0,1], [2,0], [3,-1],[2,-3],[-1,-2]]
ppppppp = [Point_2D(x=ns[0], y=ns[1]) for ns in pppppp]
print(fanned_area(ppppppp))
# for point in outer_points:
#     cv.circle(src_image, point, 5, [255, 200, 128], thickness=2)
#
# cv.imshow("", src_image)
# cv.waitKey(0)
# cv.destroyAllWindows()
