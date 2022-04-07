# CMakeLists.txt for G4CU project

project(project)

# required cmake version
cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(cuda)
add_subdirectory(spdlog)
link_directories( ${CMAKE_BINARY_DIR}/spdlog )
set(CMAKE_INSTALL_RPATH "")


set(EXTRA_LIBS ${EXTRA_LIBS} gpu)
ADD_EXECUTABLE(project main.cpp src/save_mat.hpp src/save_object.hpp src/writer_class.hpp src/print_points.cpp src/print_points.h)


target_link_libraries(project ${EXTRA_LIBS} ${PROJECT_LINK_LIBS})
if (WIN32)
    message("compile on WIN32")
    #target_link_libraries(${PROJECT_NAME} C:/Users/Administrator/CMakeBuilds/spdlogd.lib)
endif (WIN32)
#target_link_libraries(${PROJECT_NAME} /home/isv/nasirZadeh/gpu_code/spdlog/build/libspdlog.a)
target_include_directories(${PROJECT_NAME} PRIVATE ${CUDA_TOOLKIT_ROOT_DIR}/include
        ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
        "/usr/local/cuda-10.2/targets/x86_64-linux/include/")

target_include_directories(${PROJECT_NAME} PRIVATE ./spdlog/include)
target_include_directories(${PROJECT_NAME} PRIVATE ../progress-cpp/include)
