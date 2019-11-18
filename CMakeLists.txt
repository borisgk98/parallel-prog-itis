cmake_minimum_required (VERSION 3.8)

project ("parallel-itis-2019")

# Dependecies
# OMP
find_package(OpenMP REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
# MPI
set(CMAKE_C_COMPILER "/usr/bin/mpicc")
set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")
include_directories(/usr/include/mpi/)


set(SOURCES_EXE src/main.cpp src/mpi/mpie.h src/mpi/mpi_ex1.cpp src/mpi/mpi_ex2.cpp src/mpi/map.cpp)
file(GLOB all_examples_SRCS
        "${PROJECT_SOURCE_DIR}/src/openmp/examples/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/openmp/examples/*.h"
        )
add_library(examples STATIC ${all_examples_SRCS})
file(GLOB all_openmp_SRCS
        "${PROJECT_SOURCE_DIR}/src/openmp/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/openmp/*.h"
        )
add_library(openmp STATIC ${all_openmp_SRCS} src/openmp/omp_ex1.cpp src/openmp/omp_ex2.cpp src/openmp/omp_ex3.cpp src/openmp/omp_ex4.cpp src/openmp/omp_ex5.cpp src/openmp/omp_ex6.cpp src/openmp/omp_ex7.cpp src/openmp/omp_ex8.cpp src/openmp/omp_ex9.cpp src/openmp/omp_ex10.cpp src/openmp/omp_ex11.cpp src/openmp/omp_ex12.cpp src/openmp/omp_ex13.cpp)

add_executable(main ${SOURCES_EXE})

target_link_libraries(main examples openmp mpi)