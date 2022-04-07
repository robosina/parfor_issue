//
// Created by isv on 19.07.21.
//

#ifndef PROJECT_PRINT_POINTS_H
#define PROJECT_PRINT_POINTS_H

#include "spdlog/spdlog.h"

class print_points {

private:
    float *xpoints;
public:
    void setXpoints(float *xpoints);

    void setYpoinys(float *ypoinys);

    void print_some_points(std::vector<int>&& numbers);

private:
    float *ypoinys;
};


#endif //PROJECT_PRINT_POINTS_H
