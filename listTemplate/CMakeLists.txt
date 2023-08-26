cmake_minimum_required(VERSION 3.15...3.25)

project(MyProject
  VERSION
    1.0
  DESCRIPTION
    "C++ excercise"
  LANGUAGES
    CXX
)

# Create the executable
add_executable(myexample main.cpp)

set(CMAKE_BUILD_TYPE Debug)
