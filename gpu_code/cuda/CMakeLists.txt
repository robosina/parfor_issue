# CMakeLists.txt for G4CU project

project(gpu)

# required cmake version
cmake_minimum_required(VERSION 2.8)


# packages
find_package(CUDA)

#include_directories ("${PROJECT_SOURCE_DIR}")

# nvcc flags
set(CMAKE_CUDA_ARCHITECTURES 50 60 61 75)
#set(CMAKE_CUDA_)
#set(CMAKE_CUDA_ARCH 3 3.5 5 6.1 7.5)
#set(CMAKE_CUDA_CODE 3 3.5 5 6.1 7.5)
#set(CUDA_NVCC_FLAGS -gencode arch=CMAKE_CUDA_ARCH,code=CMAKE_CUDA_CODE;-G;-g)
#set(CUDA_NVCC_FLAGS -gencode arch=compute_61,code=sm_61;-G;-g)
#string(APPEND CMAKE_CUDA_FLAGS " -gencode arch=compute_50,code=sm_50")
#string(APPEND CMAKE_CUDA_FLAGS " -gencode arch=compute_61,code=sm_61")
#string(APPEND CMAKE_CUDA_FLAGS " -gencode arch=compute_75,code=sm_75")

file(GLOB_RECURSE CURRENT_HEADERS  *.h *.hpp *.cuh)
file(GLOB CURRENT_SOURCES  *.cpp *.cu)

source_group("Include" FILES ${CURRENT_HEADERS}) 
source_group("Source" FILES ${CURRENT_SOURCES}) 

#cuda_add_library(gpu SHARED ${CURRENT_HEADERS} ${CURRENT_SOURCES})
cuda_add_library(gpu STATIC ${CURRENT_HEADERS} ${CURRENT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CUDA_TOOLKIT_ROOT_DIR}/include ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} "/usr/local/cuda-10.2/targets/x86_64-linux/include/")
target_include_directories(${PROJECT_NAME} PRIVATE ../../progress-cpp/include)
