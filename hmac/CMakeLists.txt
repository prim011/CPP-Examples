cmake_minimum_required(VERSION 3.15...3.25)

project(MyProject
  VERSION
    1.0
  DESCRIPTION
    "C++ excercise"
  LANGUAGES
    CXX
)


# Define STD_HMAC to include externally defined HMAC
# Example:
#  option(STD_HMAC "Include HMAC in External Library" TRUE)
# or use the internal implementation with this:
#  option(STD_HMAC "Include HMAC in External Library" FALSE)
option(STD_HMAC "Include HMAC in External Library" TRUE)

if (STD_HMAC)
  # instruct hmac.hpp to take the proper #ifdef
  add_compile_definitions(STD_HMAC)
endif()

# Create the executable
add_executable(myHMACExample main.cpp)

if (STD_HMAC)
  # include the compilation of the subtree
  add_subdirectory(hmac_sha256)
  # include the library into the linking process
  target_link_libraries(myHMACExample LINK_PUBLIC hmac_sha256)
endif()

set(CMAKE_BUILD_TYPE Debug)

enable_testing()

add_test(NAME HMACTest
            COMMAND myHMACExample)
unset(STD_HMAC CACHE)
