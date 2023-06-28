// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.

#pragma once

#include <opencv2/>
#include <algorithm>
#include <vector>

typedef unsigned int coord_t;
typedef unsigned int coord2_t;

struct point
{
    coord_t x;
    coord_t y;

    bool operator <(const point &p) const
    {
        return x < p.x || (x == p.x && y < p.x);
    }
};

// 3D cross product of OA and OB vectors, (i.e z-component of their "2D" cross product, but remember that it is not defined in "2D").
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
coord2_t zCrossProd(const point &O, const point &A, const point &B);

std::vector<point> monotoneChain(std::vector<point> P);