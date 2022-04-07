//
// Created by isv on 14.07.21.
//

#ifndef PROJECT_WRITER_CLASS_H
#define PROJECT_WRITER_CLASS_H

#include <deque>
#include "save_object.hpp"
#include "mutex"
#include "thread"
#include "spdlog/spdlog.h"
#include <chrono>
#include <cuda.h>

template<typename T>
class writer_class
{
public:
    void addNewSaveObjects(std::unique_ptr<save_object<T>> saveObject);

    void run_save_thread();

private:
    std::deque<std::unique_ptr<save_object<T>>> m_saveObjects;

    bool stop{false};

    std::mutex _mutex;

public:
    void setStop(bool stop);
};

template<typename T>
void writer_class<T>::addNewSaveObjects(std::unique_ptr<save_object<T>> saveObject)
{
    spdlog::debug("new save object has been added");
    const std::lock_guard<std::mutex> lock(_mutex);
    m_saveObjects.push_back(std::move(saveObject));
}

template<typename T>
void writer_class<T>::run_save_thread()
{
    while (!stop)
    {
        if (!m_saveObjects.empty())
        {
            std::unique_ptr<save_object<T>> object;
            {
                const std::lock_guard<std::mutex> lock(_mutex);
                spdlog::info("founded new save object trying to save to system [number "
                             "of queue {}]",
                    m_saveObjects.size());
                object = std::move(m_saveObjects.front());
                m_saveObjects.pop_front();
            }
            object->savetofile();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

template<typename T>
void writer_class<T>::setStop(bool stop)
{
    writer_class::stop = stop;
}

#endif // PROJECT_WRITER_CLASS_H
