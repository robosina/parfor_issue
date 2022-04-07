include <chrono>
#include <cstdio>
#include <cstring>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "cmath"
#include "eigen/src/print_points.h"
#include "progresscpp/ProgressBar.hpp"
#include "eigen/src/save_object.h"
#include "spdlog/spdlog.h"
#include "eigen/src/writer_class.h"
#include "eigen/src/save_mat.h"
#include <regex>
extern int Min{0};
extern int Max{0};
extern float step{0.1};
extern int Size{0};
#if __unix__
//#    include <sys/stat.h>
//#    include <sys/types.h>
#else
#    include <fileapi.h>
#endif
typedef unsigned long long count_t;

using namespace std::chrono;

extern "C" float findValues(float *in_xpoints, float *in_ypoints, int num_pts,
    int batch_size, int start, float *xout, float *yout, count_t *mat);

void readTextFile(
    std::vector<float> &xs, std::vector<float> &ys, std::string &filepath, int num_pts);

void testTwoIsSame(std::vector<float> &values1, const float *values2);

void writeResultToFile(
    int n, int batch, int start, float *x, float *y, count_t *row, count_t *col);

int main(int argc, char *argv[])
{
    spdlog::warn("{}", argv[0]);

    std::string file_path = "/home/supercomputer/Documents/parfor_issue/cor.txt";
    int num_pts_to_read = 80000;
    int batch = 1000;
    Min = -47;
    Max = 47;
    step = 1;

    Size = (Max - Min) / step;

#ifdef _WIN32
    std::replace(file_path.begin(), file_path.end(), '\\', '/');
#endif
    writer_class<count_t> m_writer;
    //    print_points pr_p{};
    std::vector<float> xpoints;
    std::vector<float> ypoints;
    high_resolution_clock::now();

    auto start_time = high_resolution_clock::now();
    readTextFile(xpoints, ypoints, file_path, num_pts_to_read);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start_time);

    spdlog::info("time for read dataset : {} milisecond", duration.count());

    int number_of_Points = xpoints.size();

    // convert data to float*
    auto *points_x = new float[number_of_Points]();
    auto *points_y = new float[number_of_Points]();

    memcpy(points_x, xpoints.data(), number_of_Points * sizeof(float));
    memcpy(points_y, ypoints.data(), number_of_Points * sizeof(float));

    auto *x_inSec = (float *)malloc(sizeof(float) * number_of_Points * batch);
    auto *y_inSec = (float *)malloc(sizeof(float) * number_of_Points * batch);
    std::vector<count_t> m(Size * Size);
    auto *mat = (count_t *)malloc(sizeof(count_t) * m.size());
    std::memcpy(mat, m.data(), m.size() * sizeof(count_t));

#ifdef __unix__
    if (mkdir("result", 0777) == -1) {}
#else
    if (CreateDirectory("result", NULL))
    {
        std::cout << "file created" << std::endl;
    }
#endif
    save_mat<count_t> saveObject;

    saveObject.setFilename("result/result.csv");
    float time =
        findValues(points_x, points_y, number_of_Points, batch, 0, x_inSec, y_inSec, mat);

    saveObject.insert(mat, Size);
    saveObject.savetofile();

    m_writer.setStop(true);

    // delete arrays from memory
    //   delete[] points;
    delete x_inSec;
    delete y_inSec;
    stop = high_resolution_clock::now();
    auto duration_totoal = duration_cast<std::chrono::seconds>(stop - start_time);

    spdlog::info("number of intersections:{} totoal GPU computation time : {}sec ",
        number_of_Points * batch, time / 1000);

    spdlog::info("number of intersections:{} totoal CPU+GPU computation time : {}sec ",
        number_of_Points * batch, duration_totoal.count());

#ifdef _WIN32
    char answer;
    std::cout << "do you want to open result folder:?[Y,n]:";
    std::cin >> answer;
    if (answer == 'n')
    {
        return 0;
    }
    else
    {
        std::string openCommand = "explorer.exe " + std::string("result");
        system(openCommand.c_str());
    }
#else
    std::string openCommand = "nautilus " + std::string("result");
    system(openCommand.c_str());
#endif
}

void writeResultToFile(
    int n, int batch, int start, float *x, float *y, count_t *row, count_t *col)
{
    std::ofstream myfile;

    myfile.open("result/result" + std::to_string(start) + "_" +
                std::to_string(start + batch) + ".csv");

    for (int i = 0; i < n; ++i)
    {
        if ((row[i] == 0) && (col[i] == 0) && (std::abs(x[i]) == 0) &&
            (std::abs(y[i]) == 0))
        {
            continue;
        }
        //        static int prow = -1;
        //        if (prow != row[i])
        //        {
        //            prow = row[i];
        //            std::cout << "storing result of row:" << row[i] << std::endl;
        //            myfile.close();
        //            std::string filename = "result" + std::to_string(row[i]) +
        //            ".csv"; myfile.open(filename);
        //        }
        myfile << (row[i] + 1 + start) << "," << col[i] + 1 << "," << x[i] << "," << y[i]
               << std::endl;
    }
    myfile.close();
}

// testTwoIsSame(values,points);

void testTwoIsSame(std::vector<float> &values1, const float *values2)
{
    for (size_t i = 0; i < values1.size(); i++)
    {
        if (values1[i] != values2[i])
        {
            std::cout << "is not same" << std::endl;
            return;
        }
    }
    std::cout << "is same" << std::endl;
}

/// this function will read the text file and return result in seperate vectors
/// \param xs x in vector
/// \param ys y in vector
void readTextFile(
    std::vector<float> &xs, std::vector<float> &ys, std::string &filepath, int num_pts)
{
    std::ifstream inFile;

    inFile.open(filepath);
    if (!inFile)
    {
        spdlog::error("Unable to open file {}", filepath);
#ifdef _WIN32
        getchar();
#endif
        exit(1);
    }

    std::string x;
    float value;

    if (num_pts != -1)
    {
        xs.reserve(num_pts);
        ys.reserve(num_pts);
    }
    else
    {
        xs.reserve(100000);
        ys.reserve(100000);
    }
    int i = 0, j = 0;
    int break_in = num_pts; // determines how many points to saved

    while (inFile >> x)
    {
        if (j == 0)
        {
            std::stringstream converter(x);
            converter >> value;
            if (i % 2 == 0)
            {
                xs.emplace_back(value);
            }
            else
            {
                ys.emplace_back(value);
            }
            i++;
        }
        else
        {
            j++;
        }
        if (break_in > 0 && i > 2 * break_in && j == 0)
        {
            j = i;
            spdlog::warn("threshold has reached");
        }
    }
    spdlog::info("number of points is {} but remember that total number can be {}",
        xs.size(), j / 2);
}
