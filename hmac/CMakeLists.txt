cmake_minimum_required(VERSION 3.15...3.25)

project(MyProject
  VERSION
    1.0
  DESCRIPTION
    "C++ excercise"
  LANGUAGES
    CXX
)


# Include this definition for the externally defined HMAC
# Example of subtree inclution:
#   $ git subtree add --prefix hmac/hmac_sha256 <URL> <branch> --squash
#add_compile_definitions(STD_HMAC)


if (STD_HMAC)
  # include the compilation of the subtree
  add_subdirectory(hmac_sha256)
  target_link_libraries(myHMACExample LINK_PUBLIC hmac_sha256)
endif()  
# Create the executable
add_executable(myHMACExample main.cpp)

set(CMAKE_BUILD_TYPE Debug)

enable_testing()

add_test(NAME HMACTest
            COMMAND myHMACExample)
