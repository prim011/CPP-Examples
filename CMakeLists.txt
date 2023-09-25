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
add_subdirectory(listExample)
add_subdirectory(listTemplate)
add_subdirectory(treeTemplate)
add_subdirectory(abstClass)
add_subdirectory(hashTree)
add_subdirectory(blockchain)
add_subdirectory(hmac)

enable_testing()


