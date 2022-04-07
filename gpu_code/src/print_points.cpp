//
// Created by isv on 19.07.21.
//

#include "print_points.h"

void print_points::setXpoints(float *xpoints) {
    print_points::xpoints = xpoints;
}

void print_points::setYpoinys(float *ypoinys) {
    print_points::ypoinys = ypoinys;
}

void print_points::print_some_points(std::vector<int> &&numbers) {
    for (auto i : numbers) {
//        i = i + 1;
        for (int j = -5; j < 5; j++) {
            spdlog::info("i:{},j:{},print near points({}) : ({},{})", i, j, (2 * i) + j, xpoints[(2 * i) + j],
                         ypoinys[(2 * i) + j]);
        }
        spdlog::info("line:p1({},{}) ,p2({},{})", xpoints[(2 * i) - 1], ypoinys[(2 * i) - 1],
                     xpoints[2 * i], ypoinys[2 * i]);
    }
}
