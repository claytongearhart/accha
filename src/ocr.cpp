// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.

//
// Created by Clayton Gearhart on 6/28/23.
//

#include "ocr.h"

coord2_t zCrossProd(const point &O, const point &A, const point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
};

std::vector<point> monotoneChain(std::vector<point> P) {
    size_t n = P.size(), k = 0;
    if (n <= 3) return P;
    std::vector<point> H(2 * n);

// Sort points lexicographically
    sort(P.begin(), P.end());

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


