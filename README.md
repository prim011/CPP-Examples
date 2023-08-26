# Quick Reference for some C++ Example Class

This small code samples are composed of different C++ class topic of interest which have been organized by different folders. The folders contains a very simple structucture, normally as follow:
- 	main.cpp file: containing the exercise development
- 	CMakeLists.txt: containing the isntruction on how to compile with cmake under C++11
-	README.md: short decription in marked-up file to describe the excersice and how to compile

## Excercises 
The folders beneeth this llevel are the following:
- 	listExample	(single linked list structure, unordered)
-	listTemplate	(same as before, but using template in C++)


## Compiling
Assuming GCC v.11 and cmake already installed

Only for the first time run the following:

`$ cmake . `

Then successive build like that:

`$ cmake --build . `

the name of the file to compile and the project are identified in the `CMakeLists.txt` file. 
Output is the executable `myexample` generated after the build.

## Getting in touch
We would love to hear from you. If you fancy the code and would like to improve it feel free to raise a bug or get in contact with me, Francesco Fiaschi at *prim011@yahoo.com*
