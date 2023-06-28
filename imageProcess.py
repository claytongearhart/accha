"""

"""

__author__      = "Clayton Gearhart"
__copyright__ = "Copyright 2023 Clayton Gearhart"
__license__     = "GNU Lesser General Public License v3.0"
__version__    = "1.0"

import cv2 as cv
import numpy as np

src_image = cv.imread('./mostRecent.jpg')
src_hsv = cv.cvtColor(src_image, cv.COLOR_BGR2HSV)
hsv_blurred = cv.blur(src_hsv, (5, 5), 1)
mask = cv.inRange(hsv_blurred, (20, 110, 120), (30, 242, 255))

k_close = np.ones((3, 3), dtype=np.uint8)
k_open = np.ones((5, 5), dtype=np.uint8)
closing = cv.morphologyEx(mask, cv.MORPH_CLOSE, k_close, iterations=2)
opening = cv.morphologyEx(closing, cv.MORPH_OPEN, k_open, iterations=6)

contours, _ = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_NONE)
blob = max(contours, key=lambda el: cv.contourArea(el))
brx, bry, brw, brh = cv.boundingRect(blob)

opening = cv.GaussianBlur(opening, (3,3), 1)
opening = np.float32(opening)
dst = cv.cornerHarris(opening, 2, 3, 0.04)

dst = cv.dilate(dst, None)
src_image[dst > 0.01 * dst.max()]=[0, 0, 255]

cv.imshow("", src_image)
cv.waitKey(0)
cv.destroyAllWindows()
