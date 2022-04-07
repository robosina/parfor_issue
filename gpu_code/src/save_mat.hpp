#ifndef SAVE_MAT_H
#define SAVE_MAT_H

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

template<typename T>
class save_mat
{
public:
    save_mat(){};

    void insert(T *d_mat, int size);

    void savetofile();

    void setFilename(const std::string &newFilename);

private:
    std::string m_filename;

    std::vector<T> mat;

    int sz;
};

template<typename T>
void save_mat<T>::setFilename(const std::string &newFilename)
{
    m_filename = newFilename;
}

template<typename T>
void save_mat<T>::insert(T *d_mat, int size)
{
    sz = size;
    mat.resize(sz * sz);
    std::memcpy(mat.data(), d_mat, sz * sz * sizeof(T));
}

template<typename T>
void save_mat<T>::savetofile()
{
    spdlog::info("save to file initialized");
    std::ofstream myfile;
    myfile.open(m_filename);
    auto t1 = std::chrono::steady_clock::now();

    int row = 0;

    T max = 0;
    int im = 0;
    int jm = 0;
    for (int i = 0; i < sz; ++i)
    {
        for (int j = 0; j < sz; ++j)
        {
            auto val = mat[row];
            if (val > max)
            {
                max = val;
                im = i;
                jm = j;
            }
            //            myfile << row << "," << val << "\n";
            myfile << val << ",";
            row++;
        }
        myfile << "\n";
    }
    auto t2 = std::chrono::steady_clock::now();
    auto cpu_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    spdlog::info("write time:{}", cpu_time);
}
#endif // SAVE_MAT_H
