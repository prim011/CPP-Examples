# Quick Reference for some C++ Example Classes

These small code samples are composed of different C++ class topics of interest which have been organized by different folders. The folders contain a very simple structure, normally as follows:
- 	main.cpp file: containing the exercise development
- 	CMakeLists.txt: containing the instructions on how to compile with cmake under C++11
-	README.md: short description in marked-up file to describe the exercise and how to compile

## Exercises 
The folders beneath this level are the following:
- 	listExample	(single linked list structure, unordered)
-	listTemplate	(same as before, but using template in C++)


## Compiling locally
Assuming GCC v.11 and cmake already installed

Only for the first time run the following:

`$ cmake . `

Then successive builds like that:

`$ cmake --build . `

The name of the file to compile and the project are identified in the `CMakeLists.txt` file. 
Output is the executable `myexample` generated after the build.

## Using *Actions* in gitHub to compile and test
A `.yml` file has been created under the usual repository place for actions, under `./github/workflows/..`. See file in here [cmake-single-platform.yml](/.github/workflows/cmake-single-platform.yml)

Use the Action to compile and test in CI (i.e. triggered at each commit/push)

## Getting in touch
We would love to hear from you. If you fancy the code and would like to improve it feel free to raise a bug or get in contact with me, Francesco Fiaschi at *prim011@yahoo.com*
