//
// Created by isv on 14.07.21.
//

#ifndef PROJECT_SAVE_OBJECT_H
#define PROJECT_SAVE_OBJECT_H

#include "vector"
#include "memory"
#include <iostream>
#include <cstdint>
#include <cstring>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include "spdlog/spdlog.h"
//#include "Eigen/Core"

// import most common Eigen types
//const static Eigen::IOFormat CSVFormat(
//    Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", "\n");
template<typename T>
class save_object
{
public:
    save_object() = delete;

    save_object(float *x_inSec, float *y_inSec, T *_row, T *_col, int size,
        std::string mFilename, int startnumber);

private:
    std::vector<T> m_row;
    std::vector<T> m_col;
    std::vector<float> m_x;
    std::vector<float> m_y;
    int m_startNumber;
    std::string m_filename;

public:
    const std::string &getMFilename() const;

    void savetofile();

    void savetofile2();
};

template<typename T>
save_object<T>::save_object(float *x_inSec, float *y_inSec, T *_row, T *_col, int size,
    std::string mFilename, int startnumber)
    : m_filename(std::move(mFilename))
    , m_startNumber(startnumber)
{
    auto start_time = std::chrono::high_resolution_clock::now();
    m_row.resize(size);
    m_col.resize(size);
    m_x.resize(size);
    m_y.resize(size);
    std::memcpy(m_row.data(), _row, size * sizeof(T));
    std::memcpy(m_col.data(), _col, size * sizeof(T));
    std::memcpy(m_x.data(), x_inSec, size * sizeof(float));
    std::memcpy(m_y.data(), y_inSec, size * sizeof(float));
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    spdlog::debug("copying finished elapsed time in msec:{}", duration.count());
}

template<typename T>
const std::string &save_object<T>::getMFilename() const
{
    return m_filename;
}

template<typename T>
void save_object<T>::savetofile2()
{
    char delimiter = ',';
    for (int i = 0; i < m_row.size(); ++i)
    {
        m_row[i] = m_row[i] + 1 + m_startNumber;
    }
    for (int i = 0; i < m_row.size(); ++i)
    {
        m_col[i]++;
    }

    std::vector<std::string> lines;
    lines.reserve(m_row.size());
    auto t1 = std::chrono::steady_clock::now();
    std::ofstream myfile;
    myfile.open(m_filename);

    for (int i = 0; i < m_row.size() / 2; ++i)
    {
        if (m_x[i] == 0 && m_y[i] == 0)
        {
            continue;
        }

        myfile << m_row[i] << delimiter << m_col[i] << delimiter << m_x[i] << delimiter
               << m_y[i] << "\n";
    }
    auto t2 = std::chrono::steady_clock::now();
    auto cpu_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    spdlog::info("write time:{}", cpu_time);
    myfile.close();
}

template<typename T>
void save_object<T>::savetofile()
{
    //    USING_PART_OF_NAMESPACE_EIGEN
    //using namespace Eigen;
    //MatrixXi m = MatrixXi::Zero(940, 940);
    //float min = -47;
    //float max = 47;
    //std::vector<std::string> lines;
    //lines.reserve(m_row.size());
    //auto t1 = std::chrono::steady_clock::now();
    //std::ofstream myfile;
    //myfile.open(m_filename);

    //for (int i = 0; i < m_row.size() / 2; ++i)
    //{
    //    if (m_x[i] == 0 && m_y[i] == 0)
    //    {
    //        continue;
    //    }

    //    int x = m_x[i] * 10 + 10 * max;
    //    int y = m_y[i] * 10 + 10 * max;
    //    if (y < 0 || y > 920 || x < 0 || x > 920)
    //    {
    //        continue;
    //    }
    //    int val = m.coeff(x, y);
    //    m.coeffRef(x, y) = val + 1;

    //    //        myfile << m_row[i] << delimiter << m_col[i] << delimiter << m_x[i] <<
    //    //        delimiter
    //    //               << m_y[i] << "\n";
    //}
    //myfile << m.format(CSVFormat);
    ////    std::cout << m << std::endl;
    //auto t2 = std::chrono::steady_clock::now();
    //auto cpu_time =
    //    std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    //spdlog::info("write time:{}", cpu_time);
    //    myfile.close();
}

#endif // PROJECT_SAVE_OBJECT_H
