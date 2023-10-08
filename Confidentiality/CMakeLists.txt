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

# Define AES_ENCRYPT to inclde symmetric encryption and
# decryption of data/nodes at-rest
option(AES_ENCRYPT "Include Encryption and Decryption" TRUE)

if (STD_HMAC)
  # instruct hmac.hpp to take the proper #ifdef
  add_compile_definitions(STD_HMAC)
endif()

if (AES_ENCRYPT)
  # instruct to use proper #ifdef
  option(AES256 "select AES256 option" TRUE)
  add_compile_definitions(AES_ENCRYPT)
endif()

# Create the executable
add_executable(myConfExample main.cpp)

if (STD_HMAC)
  # include the compilation of the subtree
  add_subdirectory(hmac_sha256)
  # include the library into the linking process
  target_link_libraries(myConfExample LINK_PUBLIC hmac_sha256)
endif()

if (AES_ENCRYPT)
  # include subdirectory compilation
  add_subdirectory(AES)
  # include the library into the linking process
  target_link_libraries(myConfExample LINK_PUBLIC tiny-aes)
endif ()
  
set(CMAKE_BUILD_TYPE Debug)

enable_testing()

add_test(NAME ConfTest
            COMMAND myConfExample)
unset(STD_HMAC CACHE)
unset(AES_ENCRYPT CACHE)
unset(AES256 CACHE)
